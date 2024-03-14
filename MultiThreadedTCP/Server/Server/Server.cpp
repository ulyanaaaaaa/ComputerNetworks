#include <iostream>
#include <vector>
#include <string>
#include <winsock2.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Student {
    string name;
    string group;
    float scholarship;
    vector<int> grades;
};

vector<Student> students = {
    {"Steve", "Group 1", 1000, {4, 5, 4, 5, 3}},
    {"Ulyana", "Group 3", 1000, {10, 9, 8, 7, 6}},
};

void handleClient(SOCKET clientSocket, float averageGrade) {
    vector<Student> filteredStudents;

    for (const auto& student : students) {
        float sum = 0;
        for (int grade : student.grades) {
            sum += grade;
        }
        float avg = sum / student.grades.size();
        if (avg > averageGrade) {
            filteredStudents.push_back(student);
        }
    }

    string response;
    for (const auto& student : filteredStudents) {
        response += student.name + " " + student.group + " " + std::to_string(student.scholarship) + "\n";
    }

    send(clientSocket, response.c_str(), response.length(), 0);
    closesocket(clientSocket);
}

int main() {
    // Инициализация Winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        cerr << "Can't initialize Winsock! Quitting" << endl;
        return -1;
    }

    // Создание сокета
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Can't create socket! Quitting" << endl;
        WSACleanup();
        return -1;
    }

    // Настройка адреса сервера
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Привязка сокета
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Can't bind to port! Quitting" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Ожидание подключений
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Can't listen on socket! Quitting" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    cout << "Server is listening..." << std::endl;

    while (true) {
        // Принятие подключения
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Can't accept client connection! Quitting" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return -1;
        }

        // Получение среднего балла от клиента
        float averageGrade;
        recv(clientSocket, (char*)&averageGrade, sizeof(float), 0);
        cout << "Received average grade: " << averageGrade << std::endl;

        // Создание отдельного потока для обработки клиента
        thread clientThread(handleClient, clientSocket, averageGrade);
        clientThread.detach(); // Отсоединяем поток, чтобы не блокировать основной поток
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
