#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h> // POSIX Header
#include <netinet/in.h> // POSIX Header
#include <unistd.h>     // POSIX Header (for close, read)
#include <arpa/inet.h>  // POSIX Header

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    std::string message;

    // ... (rest of the original server logic using socket, bind, listen, accept, read, close)
    // ... (Your original POSIX server code will compile correctly here)
    
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
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Accept failed" << std::endl;
        return -1;
    }

    std::cout << "Client connected!" << std::endl;

    while (true) {
        int valread = read(new_socket, buffer, 1024);
        if (valread <= 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }
        std::string received(buffer);
        if (received == "EXIT") {
            std::cout << "Client requested exit" << std::endl;
            break;
        }
        std::cout << "Received from client: " << received << std::endl;

        send(new_socket, received.c_str(), received.length(), 0);

        std::cout << "Enter message to send to client (or 'EXIT' to quit): ";
        std::getline(std::cin, message);
        if (message == "EXIT") {
            send(new_socket, message.c_str(), message.length(), 0);
            break;
        }
        send(new_socket, message.c_str(), message.length(), 0);
        std::cout << "Sent to client: " << message << std::endl;

        memset(buffer, 0, sizeof(buffer));
    }

    close(new_socket);
    close(server_fd);
    return 0;
}