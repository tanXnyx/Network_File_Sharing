#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio> // For long long support

#define BUFFER_SIZE 1024
#define SERVER_PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    // --- 1. Connection Setup ---
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, "server", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }
    std::cout << "Connected to server!" << std::endl;

    // --- 2. File Input ---
    std::string filename;
    std::cout << "Enter the filename to send: ";
    std::getline(std::cin, filename);

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        close(sock);
        return -1;
    }

    long long file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // --- 3. Send Metadata (Size and Name) ---
    std::string metadata = filename + ":" + std::to_string(file_size);
    send(sock, metadata.c_str(), metadata.length() + 1, 0); 
    
    std::cout << "Sending file: " << filename << " (" << file_size << " bytes)..." << std::endl;

    // --- 4. Send File Data in Chunks ---
    char file_buffer[BUFFER_SIZE];
    long long bytes_sent = 0;
    
    while (file.read(file_buffer, BUFFER_SIZE)) {
        int n = send(sock, file_buffer, file.gcount(), 0);
        if (n < 0) {
            std::cerr << "Send failed during transfer." << std::endl;
            break;
        }
        bytes_sent += n;
    }
    if (file.gcount() > 0) {
        int n = send(sock, file_buffer, file.gcount(), 0);
        bytes_sent += n;
    }

    file.close();
    std::cout << "Transfer complete. Total bytes sent: " << bytes_sent << std::endl;

    // --- 5. Cleanup ---
    close(sock);
    return 0;
}