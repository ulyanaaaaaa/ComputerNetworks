#include <winsock2.h>
#include <iostream>
#include <stdlib.h>
#include<Ws2tcpip.h>
#include<tchar.h>
using namespace std;


int main() {
  setlocale(LC_ALL, "ru");

  WORD wVersionRequested;
  WSADATA wsaData;
  wVersionRequested = MAKEWORD(2, 2);
  WSAStartup(wVersionRequested, &wsaData);

  sockaddr_in client;
  client.sin_family = AF_INET;
  client.sin_port = htons(1234);
  
  InetPton(AF_INET, _T("127.0.0.1"), &client.sin_addr.s_addr);//IP из текста в числовую двоичную 

  SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

  if (connect(s, (struct sockaddr*)&client, sizeof(client)) != 0)
  {
    cout << "Failed " << WSAGetLastError() << endl;
    closesocket(s);
    WSACleanup();
    return 1;
  }
  
  char buf[255], b[255];
  while (true) {
    cout << "Введите строку..." << endl;
    cin.getline(buf, sizeof(buf));

    int bytesToSend = strlen(buf);
    send(s, buf, bytesToSend, 0);

    int bytesReceived = recv(s, b, sizeof(b), 0);
    if (bytesReceived > 0) {
      b[bytesReceived] = '\0';
      cout << b << endl;
    }
  }
  

  closesocket(s);

  WSACleanup();

  return 0;
}