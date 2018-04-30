#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <pthread.h>
#include "TcpSocket.h"
#include "FileHandle.h"
#include "Translate.h"

using namespace std;
pthread_mutex_t thread_mutexes;

void * Work(void *data)
{
   
    int client = *((int *)data);
    while(1)
    {
        FileHandle filehandle(client);      
        if(!filehandle.FileTransmit())  //接收文件
            break;  

        string filename = filehandle.GetFileName();     //获取文件名

        pthread_mutex_lock(&thread_mutexes);
        string result = Translate(filename.c_str());    //翻译音频文件
        pthread_mutex_unlock(&thread_mutexes);
		if(result.empty())
		    result = " ";
        //cout << "result:" << endl << result << endl;
        send(client, result.c_str(), result.size(), 0);     //发送结果

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
