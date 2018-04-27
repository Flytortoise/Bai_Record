#include "FileHandle.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>

using namespace std;

#define LOADBYTES 1024  //每次发送的大小

FileHandle::FileHandle(int socket)
{
	m_socket = socket;
	
	char *buff = new char[sizeof(int64)]();
	recv(m_socket, buff, sizeof(int64), 0);
	AllSize = Str2Int(buff);
	//cout << "AllSize:" << AllSize << endl;

	recv(m_socket, buff, sizeof(int64), 0);
	fileNameSize = Str2Int(buff);
	delete []buff;
	//cout << fileNameSize << endl;

	buff = new char[fileNameSize]();
	recv(m_socket, buff, fileNameSize, 0);
	GetFileName(buff, fileNameSize);
	delete []buff;
	//cout << buff2 << endl;
}

string FileHandle::GetFileName(char *filename, int64 size)
{
	m_filename.clear();
	for(int i = 5; i < size; i+=2)	//QString前4个字节忽略
	{
	 	//if(filename[i] != 0)
	    m_filename.push_back(filename[i]);
	}
	m_filename = string("./wav/") + m_filename;

	return m_filename;
}

string FileHandle::GetFileName()
{
	return m_filename;
}

int64 FileHandle::Str2Int(char *str)
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

bool FileHandle::FileTransmit()
{
	m_outfile.open(m_filename.c_str(), ofstream::out | ofstream::binary);
	if(!m_outfile.is_open())
	{
		cout << "Open " << m_filename << "failed !" << endl;
		return false;
	}

	char buff[LOADBYTES];
	int64 RestSize = AllSize - fileNameSize - 2 * sizeof(int64);
	while(RestSize > 0)
	{
		//cout << "RestSize1:" << RestSize << endl;
		memset(buff, 0, LOADBYTES);
		int64 tmp = RestSize < LOADBYTES? RestSize : LOADBYTES;
		
		int tt = recv(m_socket, buff, tmp, 0);
		RestSize -=	tt;
	/*	for(int i = 0; i <= tmp; i++)
		{
			cout << i << ": " << buff[i] << endl; 
		}
	*/
	//	string data = buff;
	//	cout << data.c_str() << " " << data.size() << endl;
		m_outfile.write(buff, tmp);
		
	//	cout << "RestSize2:" << RestSize << endl;
	}
	m_outfile.close();

	return true;
}
