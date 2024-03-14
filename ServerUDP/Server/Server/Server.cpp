#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

using namespace std;

int main() {
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        cerr << "WSAStartup failed." << endl;
        return -1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error in socket creation: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddr, clientAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Привязка сокета к адресу сервера
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Bind failed with error: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    cout << "Server is listening on port " << PORT << endl;

    char buffer[MAX_BUFFER_SIZE];
    int clientAddrSize = sizeof(clientAddr);

    while (true) {
        // Прием данных от клиента
        int bytesReceived = recvfrom(serverSocket, buffer, MAX_BUFFER_SIZE, 0,
            (sockaddr*)&clientAddr, &clientAddrSize);
        if (bytesReceived == SOCKET_ERROR) {
            cerr << "recvfrom failed with error: " << WSAGetLastError() << endl;
            continue;
        }

        string receivedData(buffer, bytesReceived);
        if (receivedData.empty()) {
            cerr << "Received empty data." << endl;
            continue;
        }

        // Логика обработки строки
        if (receivedData.back() == '\n') {
            receivedData.pop_back();  // Убираем символ новой строки, если есть
        }

        if (receivedData.length() % 2 == 0) {
            receivedData.erase(0, 3);
            receivedData.erase(receivedData.length() - 2, 2);
        }

        // Отправляем результат обратно клиенту
        sendto(serverSocket, receivedData.c_str(), receivedData.length(), 0,
            (sockaddr*)&clientAddr, clientAddrSize);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
