
char szPath[260] = "0";	// #define MAX_PATH 260
_getcwd(szPath, sizeof(szPath)-1);
	
void FindFileInDir(const char* _path, vector<string>& files)
{
	_finddata_t fileInfo;

	long handle = _findfirst(_path, &fileInfo);
	if (handle == -1L)
	{
		return;
	}

	do 
	{
		files.push_back(fileInfo.name);
	} while (_findnext(handle, &fileInfo) == 0);

	_findclose(handle);
}
