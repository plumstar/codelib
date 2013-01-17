
static void Ansi2UTF8(CString& src, CString& desc)
{
	// ansi to unicode
	int wcsLen = MultiByteToWideChar(CP_ACP, 0, src.GetBuffer(), -1, NULL, 0); //获取宽字节所需的缓冲区长度
	wchar_t* wbuff = new wchar_t[wcsLen + 1];
	MultiByteToWideChar(CP_ACP, 0, src.GetBuffer(), -1, wbuff, wcsLen);
	wbuff[wcsLen] = 0;

	// unicode to UTF8
	int u8Len = ::WideCharToMultiByte(CP_UTF8, 0, wbuff, wcslen(wbuff), NULL, 0, NULL, NULL);
	char* szU8 = new char[u8Len + 1];	// UTF8虽然是Unicode的压缩形式，但也是多字节字符串，所以可以以char的形式保存
	::WideCharToMultiByte(CP_UTF8, 0, wbuff, wcslen(wbuff), szU8, u8Len, NULL, NULL);
	szU8[u8Len] = 0;

	desc.Format("%s", szU8);
}

static void UTF82Ansi(CString& src, CString& desc)
{
	// UTF8 to unicode
	int wcsLen = MultiByteToWideChar(CP_UTF8, 0, src.GetBuffer(), -1, NULL, 0);
	wchar_t* wbuff = new wchar_t[wcsLen + 1];
	MultiByteToWideChar(CP_UTF8, 0, src.GetBuffer(), -1, wbuff, wcsLen);

	// unicode to ansi
	int ansilen = ::WideCharToMultiByte(CP_ACP, 0, wbuff, wcslen(wbuff), NULL, 0, NULL, NULL);
	char* szAnsi = new char[ansilen + 1];
	::WideCharToMultiByte(CP_ACP, 0, wbuff, wcslen(wbuff), szAnsi, ansilen, NULL, NULL);
	szAnsi[ansilen] = 0;

	desc.Format("%s", szAnsi);
}
