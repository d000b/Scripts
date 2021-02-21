#pragma once;

#include <iostream> 
#include <cwctype>
#include <Windows.h>
#include <wininet.h>
#pragma comment(lib, "Wininet.lib")

typedef unsigned __int32 ERROR_CODE;

namespace UltimaAPI
{
	enum eProtocols
	{
		ftp,
		http,
		https,
		smtp,
		pop3,
		imap,
		nntp
	};
	const wchar_t* Protocols[7] = {
		L"ftp",
		L"http",
		L"https",
		L"smtp",
		L"pop3",
		L"imap",
		L"nntp"
	};

	typedef bool(*BreakerFn)(DWORD);

	ERROR_CODE	LastError;
	DWORD		szBuffer = 8192;

	ERROR_CODE		GetFile(HINTERNET, const wchar_t*, const wchar_t*, BreakerFn, DWORD = 8192);
	ERROR_CODE		GetFile(HINTERNET, const wchar_t*, const wchar_t*, DWORD = 8192);
	ERROR_CODE		GetFile(HINTERNET, const wchar_t*, void*&, BreakerFn, DWORD = 8192);
	ERROR_CODE		GetFile(HINTERNET, const wchar_t*, void*&, DWORD = 8192);
	HINTERNET		OpenHandle();
	void			CloseHandle(HINTERNET);
	ERROR_CODE		URLDownloadFileW(const wchar_t*, const wchar_t*, BreakerFn);
	ERROR_CODE		URLDownloadFileW(const wchar_t*, const wchar_t*);
	ERROR_CODE		URLDownloadFileToMemoryW(const wchar_t*, void*&, BreakerFn);
	ERROR_CODE		URLDownloadFileToMemoryW(const wchar_t*, void*&);

	void SetBufferSize(DWORD);

	bool Protocol(eProtocols, int&, int&);
}

enum eERROR : ERROR_CODE
{
	SUCCESS = 0,
	INTERNET_HANDLE_INVALID,
	FILE_HANDLE_INVALID,
	READFILE_INVALID,
	HINTERNET_HANDLE_INVALID,
	USER_BREAK
};

ERROR_CODE	UltimaAPI::GetFile(HINTERNET hOpen, const wchar_t* szUrl, const wchar_t* szFileName, BreakerFn fn, DWORD szBuffer)
{
	DWORD		dwSize = 0;
	wchar_t		szHead[] = L"Accept: */*\r\n\r\n";
	HINTERNET	hConnect;
	FILE* pFile;
	wchar_t* buffer;

	if (!(hConnect = InternetOpenUrlW(hOpen, szUrl, szHead, lstrlenW(szHead), INTERNET_FLAG_DONT_CACHE, 0)))
		return eERROR::INTERNET_HANDLE_INVALID;
	if (!(pFile = _wfopen(szFileName, L"wb")))
		return eERROR::FILE_HANDLE_INVALID;

	buffer = new wchar_t[szBuffer];

	do
	{
		if (fn(dwSize))
		{
			fclose(pFile);
			delete[] buffer;
			_wremove(szFileName);
			return eERROR::USER_BREAK;
		}

		if (!InternetReadFile(hConnect, buffer, szBuffer * sizeof(wchar_t), &dwSize))
		{
			fclose(pFile);
			delete[] buffer;
			return eERROR::READFILE_INVALID;
		}
		if (dwSize)
			fwrite(buffer, 1, dwSize, pFile);
		else break;
	} while (true);

	fflush(pFile);
	fclose(pFile);
	delete[] buffer;
	return eERROR::SUCCESS;
}

ERROR_CODE	UltimaAPI::GetFile(HINTERNET hOpen, const wchar_t* szUrl, const wchar_t* szFileName, DWORD szBuffer)
{
	DWORD		dwSize;
	wchar_t		szHead[] = L"Accept: */*\r\n\r\n";
	HINTERNET	hConnect;
	FILE*		pFile;
	wchar_t*	buffer;

	if (!(hConnect = InternetOpenUrlW(hOpen, szUrl, szHead, lstrlenW(szHead), INTERNET_FLAG_DONT_CACHE, 0)))
		return eERROR::INTERNET_HANDLE_INVALID;
	if (!(pFile = _wfopen(szFileName, L"wb")))
		return eERROR::FILE_HANDLE_INVALID;

	buffer = new wchar_t[szBuffer];
	
	do
	{
		if (!InternetReadFile(hConnect, buffer, szBuffer * sizeof(wchar_t), &dwSize))
		{
			fclose(pFile);
			delete[] buffer;
			return eERROR::READFILE_INVALID;
		}
		if (dwSize)
			fwrite(buffer, 1, dwSize, pFile);
		else break;
	} while (true);

	fflush(pFile);
	fclose(pFile);
	delete[] buffer;
	return eERROR::SUCCESS;
}

HINTERNET	UltimaAPI::OpenHandle()
{
	return InternetOpenW(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
}

void		UltimaAPI::CloseHandle(HINTERNET i)
{
	InternetCloseHandle(i);
}

bool		UltimaAPI::Protocol(eProtocols e, int& port, int& protocol)
{
	switch (e)
	{
		case UltimaAPI::ftp:
			protocol = INTERNET_SERVICE_FTP;
			port = INTERNET_DEFAULT_FTP_PORT;
			return true;
		case UltimaAPI::http:
			protocol = INTERNET_SERVICE_HTTP;
			port = INTERNET_DEFAULT_HTTP_PORT;
			return true;
		case UltimaAPI::https:
			protocol = INTERNET_SERVICE_HTTP;
			port = INTERNET_DEFAULT_HTTPS_PORT;
			break;
		case UltimaAPI::smtp:
		case UltimaAPI::pop3:
		case UltimaAPI::imap:
		case UltimaAPI::nntp:
		default: return false;
	}
}

ERROR_CODE	UltimaAPI::URLDownloadFileW(const wchar_t* url, const wchar_t* file, BreakerFn fn)
{
	HINTERNET i;

	if (!(i = InternetOpenW(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0)))
		return eERROR::HINTERNET_HANDLE_INVALID;

	ERROR_CODE err = GetFile(i, url, file, fn, szBuffer);
	InternetCloseHandle(i);
	return err;
}

ERROR_CODE	UltimaAPI::URLDownloadFileW(const wchar_t* url, const wchar_t* file)
{
	HINTERNET i;

	if (!(i = InternetOpenW(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0)))
		return eERROR::HINTERNET_HANDLE_INVALID;

	ERROR_CODE err = GetFile(i, url, file, szBuffer);
	InternetCloseHandle(i);
	return err;
}

void		UltimaAPI::SetBufferSize(DWORD set)
{
	szBuffer = set;
}
