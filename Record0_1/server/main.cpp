#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "TcpSocket.h"
using namespace std;
int main(int argc, char const *argv[])
{
/*
    if(argc != 3)
    {
        printf("Please input Server Bind IP and Port");
        exit(-1);
    }

    int port = atoi(argv[2]);
    if(port < 1000 || port > 65535)
    {
        printf("Please input Port: 1000 ---- 65535");
        exit(-1);
    }
*/
    TcpSocket server(8000, "192.168.10.134");
    int client = server.start();
    cout << "client = " << client << endl;
    char *buff = new char[100]();
    while(1)
    {
        memset(buff, 0,100);
        recv(client, buff, 100, 0);
        printf("buff = %s\n", buff);
        send(client, buff, 100, 0);
    }

	return 0;
}
