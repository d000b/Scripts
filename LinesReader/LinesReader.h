#pragma once

#include <string>
#include <stringapiset.h>
#include <vector> // include std::vector MS because Vector corruption heap

namespace UltimaAPI
{
	bool  ut8toWide(std::wstring&, std::string&);
	bool  ut8toWide(std::wstring& to, char* from);
	
	bool  widetoUt8(std::wstring&, std::string&);
	bool  widetoUt8(std::wstring&, char*&);

	bool  fread(const wchar_t*, std::vector<std::string>&);
	bool  fread(const wchar_t*, std::string&);
	bool  fread(const wchar_t*, std::vector<std::wstring>&);
	bool  fread(const wchar_t*, std::wstring&);

	bool  wfread(const wchar_t*, std::vector<std::wstring>&);
	bool  wfread(const wchar_t*, std::wstring&);

	bool  fwrite(const wchar_t*, std::string&);
	bool  fwrite(const wchar_t*, std::wstring&);
};

bool  UltimaAPI::ut8toWide(std::wstring& to, std::string& from)
{
	auto i = MultiByteToWideChar(CP_UTF8, 0, from.data(), from.length(), NULL, 0);
	LPWSTR wch = new WCHAR[i + 1];
	int b;
	if (b = MultiByteToWideChar(CP_UTF8, 0, from.data(), from.length(), wch, i))
	{
		wch[i] = 0;
		to = wch;
	}
	delete[] wch;
	return !!b;
}

bool  UltimaAPI::ut8toWide(std::wstring& to, char* from)
{
	auto len = strlen(from);
	auto i = MultiByteToWideChar(CP_UTF8, 0, from, len, NULL, 0);
	LPWSTR wch = new WCHAR[i + 1];
	int b;
	if (b = MultiByteToWideChar(CP_UTF8, 0, from, len, wch, i))
	{
		wch[i] = 0;
		to = wch;
	}
	delete[] wch;
	return !!b;
}

bool  UltimaAPI::widetoUt8(std::wstring& from, std::string& to)
{
	auto i = WideCharToMultiByte(CP_UTF8, 0, from.data(), from.length(), NULL, 0, nullptr, nullptr);
	LPSTR wch = new CHAR[i + 1];
	int b;
	if (b = WideCharToMultiByte(CP_UTF8, 0, from.data(), from.length(), wch, i, nullptr, nullptr))
	{
		wch[i] = 0;
		to = wch;
	}
	delete[] wch;
	return !!b;
}

bool  UltimaAPI::widetoUt8(std::wstring& from, char*& to)
{
	auto i = WideCharToMultiByte(CP_UTF8, 0, from.data(), from.length(), NULL, 0, nullptr, nullptr);
	LPSTR wch = new CHAR[i + 1];
	int b;
	if (b = WideCharToMultiByte(CP_UTF8, 0, from.data(), from.length(), wch, i, nullptr, nullptr))
	{
		wch[i] = 0;
		to = wch;
	}
	return !!b;
}

bool  UltimaAPI::fread(const wchar_t* name, std::vector<std::string>& wcs)
{
	FILE* f;
	_wfopen_s(&f, name, L"r");
	if (f)
	{
		char b[256];
		while (fgets(b, sizeof(b), f) == b)
			wcs.push_back(b);
		fclose(f);
		return true;
	}
	else return false;
}

bool  UltimaAPI::fread(const wchar_t* name, std::string& wcs)
{
	FILE* f;
	_wfopen_s(&f, name, L"r");
	if (f)
	{
		char b[256];
		while (fgets(b, sizeof(b), f) == b)
			wcs += b;
		fclose(f);
		return true;
	}
	else return false;
}

bool  UltimaAPI::fread(const wchar_t* name, std::vector<std::wstring>& wcs)
{
	FILE* f;
	_wfopen_s(&f, name, L"r");
	if (f)
	{
		char b[256];
		std::wstring wc;
		while (fgets(b, sizeof(b), f) == b)
			if (ut8toWide(wc, b))
				wcs.push_back(wc);
		fclose(f);
		return true;
	}
	else return false;
}

bool  UltimaAPI::fread(const wchar_t* name, std::wstring& wcs)
{
	FILE* f;
	_wfopen_s(&f, name, L"r");
	if (f)
	{
		char b[256];
		std::wstring wc;
		while (fgets(b, sizeof(b), f) == b)
			if (ut8toWide(wc, b))
				wcs += wc;
		fclose(f);
		return true;
	}
	else return false;
}


bool  UltimaAPI::wfread(const wchar_t* name, std::vector<std::wstring>& wcs)
{
	FILE* f;
	_wfopen_s(&f, name, L"r");
	if (f)
	{
		wchar_t b[256];
		while (fgetws(b, sizeof(b), f) == b)
			wcs.push_back(b);
		fclose(f);
		return true;
	}
	else return false;
}

bool  UltimaAPI::wfread(const wchar_t* name, std::wstring& wcs)
{
	FILE* f;
	_wfopen_s(&f, name, L"r");
	if (f)
	{
		wchar_t b[256];
		while (fgetws(b, sizeof(b), f) == b)
			wcs += b;
		fclose(f);
		return true;
	}
	else return false;
}

bool  UltimaAPI::fwrite(const wchar_t* name, std::string& wcs)
{
	FILE* f;
	_wfopen_s(&f, name, L"w");
	return f && fwrite(wcs.data(), sizeof(char), wcs.size(), f) == wcs.size();
}

bool  UltimaAPI::fwrite(const wchar_t* name, std::wstring& wcs)
{
	FILE* f;
	_wfopen_s(&f, name, L"w");
	return f && fwrite(wcs.data(), sizeof(wchar_t), wcs.size(), f) == wcs.size();
}