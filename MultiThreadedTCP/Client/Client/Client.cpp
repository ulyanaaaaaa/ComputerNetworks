#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        cerr << "Can't initialize Winsock! Quitting" << endl;
        return -1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Can't create socket! Quitting" << endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    wchar_t ipAddr[] = L"127.0.0.1";
    if (InetPton(AF_INET, ipAddr, &serverAddr.sin_addr) != 1) {
        cerr << "Invalid address! Quitting" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Can't connect to server! Quitting" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    float averageGrade;
    cout << "Enter average grade: ";
    cin >> averageGrade;

    send(clientSocket, (char*)&averageGrade, sizeof(float), 0);

    // Получение и вывод ответа от сервера
    char buffer[1024];
    int n = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (n <= 0) {
        cerr << "Error receiving from server" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    cout << "Students with average grade above " << averageGrade << ":" << endl;
    cout << buffer << endl;

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
