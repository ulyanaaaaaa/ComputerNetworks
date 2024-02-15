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
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Can't create a socket! Quitting" << std::endl;
        return 1;
    }

    // Привязка сокета к IP-адресу и порту
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Прослушивание входящих соединений
    listen(listening, SOMAXCONN);

    // Принятие входящих соединений
    sockaddr_in client;
    int clientSize = sizeof(client);
    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];      // Имя клиента
    char service[NI_MAXSERV];    // Порт, к которому подключен клиент

    ZeroMemory(host, NI_MAXHOST); // Очистка памяти
    ZeroMemory(service, NI_MAXSERV); // Очистка памяти

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        std::cout << host << " connected on port " << service << std::endl;
    }
    else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " <<
            ntohs(client.sin_port) << std::endl;
    }

    // Закрытие сокетов
    closesocket(listening);

    // Передача данных
    char buf[4096];
    while (true) {
        ZeroMemory(buf, 4096);
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Error in recv(). Quitting" << std::endl;
            break;
        }

        if (bytesReceived == 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        std::cout << std::string(buf, 0, bytesReceived) << std::endl;
        send(clientSocket, buf, bytesReceived + 1, 0);
    }

    // Закрытие сокета клиента
    closesocket(clientSocket);

    // Очистка Winsock
    WSACleanup();

    return 0;
}