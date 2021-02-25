#pragma once;

#include <string>
#include <stringapiset.h>
#include <vector> // include std::vector MS because Vector corruption heap

namespace UltimaAPI
{
	bool  ut8toWide(std::wstring&, std::string&);
	bool  ut8toWide(std::wstring& to, char* from);
	bool  fread(const wchar_t*, std::vector<std::wstring>&);
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

bool  UltimaAPI::fread(const wchar_t* name, std::vector<std::wstring>& wcs)
{
	FILE* f;
	_wfopen_s(&f, name, L"r");
	if (f)
	{
		char b[256];
		std::wstring wc;
		while (fgets(b, sizeof(b), f))
			if (ut8toWide(wc, b))
				wcs.push_back(wc);
		fclose(f);
		return true;
	}
	else return false;
}