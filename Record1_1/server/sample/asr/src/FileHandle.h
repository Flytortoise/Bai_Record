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
	int64 Str2Int(char *str);
	string GetFileName(char *filename, int64 size);
	string GetFileName();
	bool FileTransmit();

private :
	int m_socket;
	string m_filename;
	int64 AllSize;
	int64 fileNameSize;
	ofstream m_outfile;
};



#endif 