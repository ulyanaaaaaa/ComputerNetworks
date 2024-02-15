#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

int main() {
    // Инициализация Winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOK = WSAStartup(ver, &wsData);
    if (wsOK != 0) {
        std::cerr << "Can't initialize winsock! Quitting" << std::endl;
        return 1;
    }

    // Создание сокета
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Can't create a socket! Quitting" << std::endl;
        return 1;
    }

    // Подключение к серверу
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        std::cerr << "Can't connect to server! Quitting" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Передача данных
    char buf[4096];
    std::string userInput;

    do {
        std::cout << "> ";
        std::getline(std::cin, userInput);
        if (userInput.size() > 0) {
            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR) {
                ZeroMemory(buf, 4096);
                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived > 0) {
                    std::cout << "SERVER> " << std::string(buf, 0, bytesReceived) << std::endl;
                }
            }
        }
    } while (userInput.size() > 0);// Закрытие сокета клиента
    closesocket(sock);

    // Очистка Winsock
    WSACleanup();

    return 0;
}