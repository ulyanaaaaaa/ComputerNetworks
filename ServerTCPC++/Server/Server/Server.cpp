#include <winsock2.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<string>
#include<algorithm>

using namespace std;

int main() {
    WSADATA wsData;

    int start = WSAStartup(MAKEWORD(2, 2), &wsData);

    SOCKET  server = socket(AF_INET, SOCK_STREAM, 0); //создание сокета

    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(1234);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    int c = bind(server, (sockaddr*)&local, sizeof(local)); //связывание адреса с сокетом

    int r = listen(server, 2); //установка соединения с клиентом

    sockaddr_in clientInfo;
    int clientInfo_size = sizeof(clientInfo);
    SOCKET ClientConn = accept(server, (sockaddr*)&clientInfo, &clientInfo_size); //открытие соединения

    while (true) {
        char buffer[255];
        int bytesReceived = recv(ClientConn, buffer, sizeof(buffer) - 1, 0); //прием данных через сокет

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';  
            string reversed(buffer);
            reverse(reversed.begin(), reversed.end());
            send(ClientConn, reversed.c_str(), reversed.length(), 0); //отправление данных клиенту
        }
        else if (bytesReceived == 0) {
        
            break;
        }
        else {
            cout << "Error receiving data: " << WSAGetLastError() << endl;
            break;
        }
    }

    closesocket(ClientConn); //закрытие сокета

    WSACleanup();
    return 0;
}