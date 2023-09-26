#pragma comment(lib, "ws2_32/lib")
#include <WinSock2.h>
#include <iostream>
#include<string>
#pragma warning(disable: 4996)

using namespace std;

int Counter = 0;
SOCKET Connections[100];

void ToClient(int index) {
    int msg_size;
    while (true) {
        recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv(Connections[index], msg, msg_size, NULL);
        for (int i = 0; i < Counter; i++) {
            if (i == index) { continue; }
            send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
            send(Connections[i], msg, msg_size, NULL);
        }
        delete [] msg;
    }
}

int main(int args, char* argv[])
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        cout << "Error" << endl;
        exit(1);
    }
    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    SOCKET slisten = socket(AF_INET, SOCK_STREAM, NULL);
    bind(slisten, (SOCKADDR*)&addr, sizeof(addr));
    listen(slisten, SOMAXCONN);

    SOCKET newConnection;
    for (int i = 0; i < 100; i++)
    {
        newConnection = accept(slisten, (SOCKADDR*)&addr, &sizeofaddr);
        if (newConnection == 0)
        {
            cout << "Error #2" << endl;
        }
        else {
            cout << "Client Connected" << endl;
            string msg = "Hello!\n";
            int msg_size = msg.size();
            send(newConnection, (char*)&msg_size, sizeof(int), NULL);
            send(newConnection, msg.c_str(), msg_size, NULL);
            Connections[i] = newConnection;
            Counter++;
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ToClient, (LPVOID)(i), NULL, NULL);
        }

    system("Pause");
    return 0;
}