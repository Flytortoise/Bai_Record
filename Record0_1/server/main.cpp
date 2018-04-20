#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include "TcpSocket.h"

using namespace std;

typedef long long int int64;

int64 Str2Int(char *str)
{
    int64 result = 0;
	int64 lop = 0x01;
	char lop2 = 0x01;
	for(int i = 7; i >= 0; i--)
	{
		char temp = str[i];
		lop2 = 0x01;
	    for(int j  = 0; j < 8; j ++)
		{
		    if(temp & lop2)
			{
				result |= lop;
			}
			lop <<= 1;
			lop2 <<= 1;
		}
	}

	return result;
}

string GetFileName(char *filename, int64 size)
{
     string result;
	 for(int i = 4; i < size; i++)	//QString前4个字节忽略
	 {
	 	if(filename[i] != 0)
	    	result.push_back(filename[i]);
	 }
	 printf("result = %s\n", result.c_str());
	 return result;
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
    TcpSocket server(8000, "192.168.10.134");
	char buff[1024];
	char *file = nullptr;
	ofstream outfile;
    while(1)
    {
		delete file;
		int64 AllSize = 0;
		int64 fileNameSize = 0;
		memset(buff, 0, 1024);
		cout << "Server Accept ..." << endl;
		int client = server.getClient();
		cout << "Client = " << client << endl;
		recv(client, buff, sizeof(int64), 0);
		AllSize = Str2Int(buff);
		cout << AllSize << endl;
		cout << "-------------" << endl;

		recv(client, buff, sizeof(int64), 0);
		fileNameSize = Str2Int(buff);
		cout << fileNameSize << endl;
		cout << "-------------" << endl;

		file = new char[fileNameSize + 1]();
		recv(client, file, fileNameSize, 0);
		string name = GetFileName(file, fileNameSize);
		cout << name << endl;
		cout << "-------------" << endl;

		outfile.open(name.c_str(), ofstream::out|ofstream::binary);
		if(!outfile.is_open())
		{
			cout << "open fail" << endl;
			return 0;
		}
		int64 RestSize = AllSize - fileNameSize - 2 * sizeof(int64);
		while(RestSize > 0)
		{
			cout << "RestSize1:" << RestSize << endl;
			memset(buff, 0, 1024);
			int64 tmp = RestSize < 1024? RestSize : 1024;
			
			int tt = recv(client, buff, tmp, 0);
			RestSize -=	tt;
		/*	for(int i = 0; i <= tmp; i++)
			{
				cout << i << ": " << buff[i] << endl; 
			}
		*/
			string data = buff;
			cout << data.c_str() << " " << data.size() << endl;
			outfile.write(buff, tmp);
			
			cout << "RestSize2:" << RestSize << endl;
		}
		outfile.close();

    }

	return 0;
}
