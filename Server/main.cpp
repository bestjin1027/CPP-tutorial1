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

    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
        showerrormessage("WSAStartup()");

    serversocket = socket(PF_INET, SOCK_STREAM, 0);

    if (serversocket == INVALID_SOCKET)
        showerrormessage("socket()");

    memset(&serveraddress, 0, sizeof(serveraddress));
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(serverport);

    if (bind(serversocket, (SOCKADDR*)&serveraddress, sizeof(serveraddress)) == SOCKET_ERROR)
        showerrormessage("bind()");
    cout << "now bind()\n";

    if (listen(serversocket, 5) == SOCKET_ERROR)
        showerrormessage("listen()");
    cout << "now listen()\n";

    int sizeClientAddress = sizeof(clientaddress);
    clientsocket = accept(serversocket, (SOCKADDR*)&clientaddress, &sizeClientAddress);
    cout << "now accept()\n";

    if (clientsocket == INVALID_SOCKET)
        showerrormessage("accept()");

    while (1) {
        int length = recv(clientsocket, received, sizeof(received), 0);
        received[length] = NULL;
        cout << "[receive message] : " << received << '\n';
        cout << "[send message ] : " << received << '\n';

        if (strcmp(received, "[exit]") == 0) {
            send(clientsocket, received, sizeof(received) - 1, 0);
            cout << "end\n";
            break;
        }
        send(clientsocket, received, sizeof(received) - 1, 0);
    }

    closesocket(clientsocket);
    closesocket(serversocket);
    WSACleanup;
    system("pause");
    return 0;

}