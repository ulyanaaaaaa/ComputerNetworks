#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define MAX_BUFFER_SIZE 1024

using namespace std;

int main() {
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        cerr << "WSAStartup failed." << endl;
        return -1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Error in socket creation: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    char buffer[MAX_BUFFER_SIZE];
    string inputData;

    while (true) {
        cerr << "Enter a string: ";
        getline(cin, inputData);

        // Отправляем введенную строку серверу
        sendto(clientSocket, inputData.c_str(), inputData.length(), 0,
            (sockaddr*)&serverAddr, sizeof(serverAddr));

        sockaddr_in fromAddr;
        int fromAddrLen = sizeof(fromAddr);
        int bytesReceived = recvfrom(clientSocket, buffer, MAX_BUFFER_SIZE, 0,
            (sockaddr*)&fromAddr, &fromAddrLen);
        if (bytesReceived == SOCKET_ERROR) {
            cerr << "recvfrom failed with error: " << WSAGetLastError() << endl;
            continue;
        }

        buffer[bytesReceived] = '\0';
        cout << "Server response: " << buffer << endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
