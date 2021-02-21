#pragma once;

#include <string>
#include <stringapiset.h>
#include <vector> // include std::vector MS because Vector corruption heap

namespace UltimaAPI
{
	bool ut8toWide(std::wstring&, std::string&);
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

bool  UltimaAPI::fread(const wchar_t* name, std::vector<std::wstring>& wcs)
{
	std::ifstream f(name, std::ios::out);
	if (f.is_open())
	{
		std::string str;
		std::wstring wc;
		while (std::getline(f, str))
			if (ut8toWide(wc, str))
				wcs.push_back(wc);
		return true;
	}
	else return false;
}
