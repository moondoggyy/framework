#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>

static const int PORT = 8080;
static const int BUFFER_SIZE = 1024;

int main() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(sock_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return 1;
    }

    std::cout << "Connected to server. Type messages (Ctrl+D to quit):\n";

    std::string line;
    char buf[BUFFER_SIZE];
    while (std::getline(std::cin, line)) {
        line += '\n';
        write(sock_fd, line.c_str(), line.size());

        ssize_t n = read(sock_fd, buf, sizeof(buf) - 1);
        if (n <= 0) {
            std::cout << "Server closed connection\n";
            break;
        }
        buf[n] = '\0';
        std::cout << "Echo: " << buf;
    }

    close(sock_fd);
    return 0;
}
