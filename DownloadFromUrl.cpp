#include <iostream>
#include <Windows.h>
#include <WinInet.h>
#include <tchar.h>
#include <string>
#include <direct.h>

#pragma comment(lib, "wininet.lib")

class CVodStreamCache
{
public:
	HANDLE Create(const char *token, const char *path);
	int ReadBlock(long offset, int len, char *buffer);
	int WriteBlock();
	void Destory();
	void CacheInfo();
protected:
private:
	HINTERNET internetOpen;
	HINTERNET internetOpenUrl;

	DWORD dwStatusCode;
	DWORD dwContentLength;

	HANDLE createFile;
	static const int BUFFERSIZE = 1024;
};

HANDLE CVodStreamCache::Create(const char *token, const char *path)
{
	internetOpen = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (internetOpen == NULL)
	{
		cout << "Internet open failed!" << endl;
		return NULL;
	}

	internetOpenUrl = InternetOpenUrl(internetOpen, path,
		NULL, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_PRAGMA_NOCACHE, 0);
	if (internetOpenUrl == NULL)
	{
		cout << "Internet open url failed! Error code = " << GetLastError() << endl;
		InternetCloseHandle(internetOpen);
		return NULL;
	}

	DWORD dwStatusSize = sizeof(dwStatusCode);
	HttpQueryInfo(internetOpenUrl, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatusCode, &dwStatusSize, NULL);
	cout << "Status: " << dwStatusCode << endl;

	DWORD dwLengthSize = sizeof(dwContentLength);
	HttpQueryInfo(internetOpenUrl, HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER, &dwContentLength, &dwLengthSize, NULL);
	cout << "File size: " << dwContentLength << endl;

	createFile = CreateFile(_T(token), GENERIC_WRITE|GENERIC_READ, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (createFile == INVALID_HANDLE_VALUE)
	{
		cout << "Create file failed!" << endl;
		InternetCloseHandle(internetOpenUrl);
		return NULL;
	}

	return createFile;
}

int CVodStreamCache::ReadBlock(long offset, int len, char *buffer)
{
	return 0;
}

int CVodStreamCache::WriteBlock()
{
	BOOL internetReadFile;
	char buffer[BUFFERSIZE];
	memset(buffer, 0, sizeof(buffer));
	DWORD byteRead = 0;

	BOOL hwrite;
	DWORD written;

	DWORD byteDown = 0;
	int a = 0;
	cout << "ÒÑÏÂÔØ£º" << a << "%";
	while (1)
	{
		internetReadFile = InternetReadFile(internetOpenUrl, buffer, sizeof(buffer), &byteRead);
		if (byteRead == 0)
		{
			break;
		}

		byteDown += byteRead;
		cout << "\b";
		do 
		{
			cout << "\b";
			a /= 10;
		} while (a);
		a = 100 * byteDown / dwContentLength;
		cout << a << "%";

		//SetFilePointer(createFile, a, NULL, FILE_BEGIN);
		hwrite = WriteFile(createFile, buffer, sizeof(buffer), &written, NULL);
		if (hwrite == 0)
		{
			cout << "Write to file failed!" << endl;
			CloseHandle(createFile);
			return 0;
		}
		//FlushFileBuffers(createFile);
	}
	CloseHandle(createFile);

	cout << endl << "Finished downloading!" << endl;

	return 1;
}

void CVodStreamCache::Destory()
{
	;
}

void CVodStreamCache::CacheInfo()
{
	;
}

int main()
{
	string strPath = "http://wvw.baidu.com/logo.jpg";
	int strPathLen = strPath.length();
	string token = "E:\\download\\";
	int tokenLen = token.length();

	mkdir(token.c_str());

	CVodStreamCache vsc;
	vsc.Create(token.c_str(), strPath.c_str());
	vsc.WriteBlock();

	return 0;
}
