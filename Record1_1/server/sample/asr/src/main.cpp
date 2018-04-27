#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <pthread.h>
#include "TcpSocket.h"
#include "FileHandle.h"
#include "Translate.h"

using namespace std;

void * Work(void *data)
{
   
    int client = *((int *)data);
    while(1)
    {
        FileHandle filehandle(client);
        if(!filehandle.FileTransmit())
            break;  

        string filename = filehandle.GetFileName();
        string result = Translate(filename.c_str());
		if(result.empty())
		    result = " ";
        //cout << "result:" << endl << result << endl;
        send(client, result.c_str(), result.size(), 0);

    }   

    return NULL;
}


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
	cout << "Server Start ..." << endl;
    TcpSocket server(8000, "172.16.252.189");

    while(1)
    {
        pthread_t threadID;
		cout << "Server Accept ..." << endl;
		int client = server.getClient();
		cout << "Client = " << client << endl;

		pthread_create(&threadID, NULL, Work, (void *)&client);

    }

	return 0;
}
