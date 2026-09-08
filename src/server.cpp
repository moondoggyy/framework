#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

static const int PORT = 8080;
static const int BUFFER_SIZE = 1024;

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    // Allow reuse of the port immediately after restart
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 1) < 0) {
        perror("listen");
        return 1;
    }

    std::cout << "Listening on port " << PORT << "...\n";

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        std::cout << "Client connected\n";

        char buf[BUFFER_SIZE];
        while (true) {
            ssize_t n = read(client_fd, buf, sizeof(buf));
            if (n <= 0) break;          // disconnected or error

            buf[n] = '\0';
            std::cout << "Received: " << buf;

            write(client_fd, buf, n);   // echo back
        }

        std::cout << "Client disconnected\n";
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
