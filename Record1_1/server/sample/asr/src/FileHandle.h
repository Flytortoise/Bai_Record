#ifndef FILEHANDLE
#define FILEHANDLE

#include <string>
#include <fstream>

using namespace std;
typedef long long int int64;

class FileHandle
{
public :
	FileHandle(int socket);
	int64 Str2Int(char *str);		//获取总大小，文件名大小
	string GetFileName();			//获取文件名
	bool FileTransmit();			//获取音频文件

private:
	string GetFileName(char *filename, int64 size);	//获取文件名


private :
	int m_socket;
	string m_filename;
	int64 AllSize;
	int64 fileNameSize;
	ofstream m_outfile;
};



#endif 