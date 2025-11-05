#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio> // For long long support

#define BUFFER_SIZE 1024
#define SERVER_PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // --- 1. Setup and Listen ---
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket failed" << std::endl;
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "Setsockopt failed" << std::endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }

    std::cout << "Server listening on port " << SERVER_PORT << "..." << std::endl;

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Accept failed" << std::endl;
        return -1;
    }
    std::cout << "Client connected! Waiting for file metadata..." << std::endl;

    // --- 2. Receive Metadata (Filename and Size) ---
    long long file_size = 0;
    std::string filename;
    
    int valread = read(new_socket, buffer, BUFFER_SIZE);
    if (valread > 0) {
        buffer[valread] = '\0';
        std::string metadata(buffer);
        
        size_t delimiter_pos = metadata.find(":");
        if (delimiter_pos != std::string::npos) {
            filename = "received_" + metadata.substr(0, delimiter_pos);
            try {
                file_size = std::stoll(metadata.substr(delimiter_pos + 1));
            } catch (const std::exception& e) {
                std::cerr << "Error parsing file size." << std::endl;
                close(new_socket); close(server_fd); return -1;
            }
        }
    } else {
        std::cerr << "Failed to receive metadata." << std::endl;
        close(new_socket); close(server_fd); return -1;
    }

    std::cout << "Preparing to receive file: " << filename << " (" << file_size << " bytes)..." << std::endl;

    // --- 3. Open Output File ---
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not create output file: " << filename << std::endl;
        close(new_socket); close(server_fd); return -1;
    }

    // --- 4. Receive File Data in Chunks ---
    long long bytes_received = 0;
    
    while (bytes_received < file_size) {
        long long remaining_bytes = file_size - bytes_received;
        int bytes_to_read = (remaining_bytes < BUFFER_SIZE) ? (int)remaining_bytes : BUFFER_SIZE;
        
        valread = read(new_socket, buffer, bytes_to_read);
        
        if (valread <= 0) {
            std::cerr << "Connection interrupted or read error." << std::endl;
            break;
        }
        
        outfile.write(buffer, valread);
        bytes_received += valread;
    }

    outfile.close();

    if (bytes_received == file_size) {
        std::cout << "Successfully received file: " << filename << std::endl;
    } else {
        std::cout << "Transfer failed. Expected " << file_size << " bytes, received " << bytes_received << " bytes." << std::endl;
    }

    // --- 5. Cleanup ---
    close(new_socket);
    close(server_fd);
    return 0;
}