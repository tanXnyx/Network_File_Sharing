#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    std::string message;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Convert IPv4 address
    if (inet_pton(AF_INET, "server", &serv_addr.sin_addr) <= 0) {  // Use 'server' hostname in Docker
        std::cerr << "Invalid address" << std::endl;
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    std::cout << "Connected to server!" << std::endl;

    while (true) {
        // Send message from client
        std::cout << "Enter message to send to server (or 'EXIT' to quit): ";
        std::getline(std::cin, message);
        if (message == "EXIT") {
            send(sock, message.c_str(), message.length(), 0);
            break;
        }
        send(sock, message.c_str(), message.length(), 0);
        std::cout << "Sent to server: " << message << std::endl;

        // Receive from server
        int valread = read(sock, buffer, 1024);
        if (valread <= 0) {
            std::cout << "Server disconnected" << std::endl;
            break;
        }
        std::string received(buffer);
        if (received == "EXIT") {
            std::cout << "Server requested exit" << std::endl;
            break;
        }
        std::cout << "Received from server: " << received << std::endl;

        memset(buffer, 0, sizeof(buffer));
    }

    close(sock);
    return 0;
}