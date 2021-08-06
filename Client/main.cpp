#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void showerrormessage(string message) {
    cout << "Error!!" << message << '\n';
    system("pause");
    exit(1);
}

int main(void) {
    WSADATA wsadata;
    SOCKET serversocket, clientsocket;
    SOCKADDR_IN serveraddress, clientaddress;

    int serverport = 9876;
    char received[256];
    string sent;

    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
        showerrormessage("WSAStartup()");

    clientsocket = socket(PF_INET, SOCK_STREAM, 0);

    if (clientsocket == INVALID_SOCKET)
        showerrormessage("socket()");

    memset(&serveraddress, 0, sizeof(serveraddress));
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddress.sin_port = htons(serverport);

    if (connect(clientsocket, (SOCKADDR*)&serveraddress, sizeof(serveraddress)) == SOCKET_ERROR)
        showerrormessage("connet()");
    cout << "now connect()\n";

    while (1) {
        cout << "[message sending] : ";
        getline(cin, sent);

        if (sent == "") continue;
        send(clientsocket, sent.c_str(), sent.length(), 0);
        int length = recv(clientsocket, received, sizeof(received), 0);
        received[length] = '\n';
        if (strcmp(received, "[exit]") == 0) {
            send(clientsocket, received, sizeof(received) - 1, 0);
            cout << "end\n";
            break;
        }
        cout << "[send message] : " << sent << '\n';
        cout << "[receive message] : " << received << '\n';
    }

    closesocket(clientsocket);
    WSACleanup;
    system("pause");
    return 0;

}