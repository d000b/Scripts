#include <string>
#include <vector>

namespace UltimaAPI
{
	std::wstring TagFind(
		std::wstring,
		wchar_t*,
		const wchar_t*, 
		const wchar_t*);
	std::wstring TagContent(
		std::wstring,
		std::wstring, 
		size_t = 0,
		size_t* = NULL,
		bool DeleteTag = true);
	bool TagContent(
		std::wstring,
		std::wstring,
		size_t = 0,
		size_t = -1,
		std::vector<std::wstring>* = NULL,
		bool = true,
		short = 0);
}

std::wstring	UltimaAPI::TagFind(std::wstring tag, wchar_t* br, const wchar_t* tag_breaks_l, const wchar_t* tag_breaks_r)
{
	size_t i = 0;
	size_t find = -1;
	size_t l_len = wcslen(tag_breaks_l) + 1;
	size_t r_len = wcslen(tag_breaks_r) + 1;
	for (auto i = 0u; i < tag.size(); i++)
	{
		if (find == -1)
		{
			for (auto l = 0u; l < l_len; l++)
			{
				auto tb_l = tag_breaks_l[l];
				if (tag[i] == tb_l)
				{
					if (br) *br = tb_l;
					find = i + 1;
					break;
				}
			}
			continue;
		}
		for (auto r = 0u; r < r_len; r++)
			if (tag[i] == tag_breaks_r[r])
				return tag.substr(find, i - find);
	}
	return L"";
}

std::wstring	UltimaAPI::TagContent(std::wstring source, std::wstring tag, size_t index, size_t* ÑountMatches, bool DeleteTag)
{
	size_t find = 0;
	size_t rfind = 0;
	size_t counts = 0;
	wchar_t br;
	const wchar_t* tag_breaks_l = L"<";
	const wchar_t* tag_breaks_r = L"> ";
	//	const wchar_t* tag_breaks_l = L"[{<(";
	//	const wchar_t* tag_breaks_r = L"]}>) ";
	std::wstring raw_tag = TagFind(tag, &br, tag_breaks_l, tag_breaks_r);
	std::vector<std::wstring> matches;
	while (true)
	{
		size_t f = (find = source.find(tag, rfind)) + tag.size();
		if (find == std::wstring::npos)
			break;
		rfind = f + 1;
		if (index != -1 && index > counts)
		{
			counts++;
			continue;
		}
		size_t count = 1;
		while (true)
		{
			f = source.find(raw_tag, f);
			if (f == std::wstring::npos)
				break;
			const wchar_t ch = source[f - 1];
			if (ch == br)
				count++;
			else if (ch == L'/' && source[f - 2] == br)
				count--;
			if (count == 0)
				break;
			f++;
		}
		size_t _s;
		size_t _e;
		if (DeleteTag)
		{
			_s = find + tag.size();
			_e = f - 2;
		}
		else
		{
			_s = find;
			_e = f + raw_tag.size() + 1;
		}
		rfind = _e;
		if (index != -1)
		{
			if (ÑountMatches) *ÑountMatches = counts;
			return source.substr(_s, _e - _s);
		}
		else matches.push_back(source.substr(_s, _e - _s));
	}
	counts = matches.size();
	if (ÑountMatches) *ÑountMatches = counts;
	if (counts > 1)
	{
		std::wstring buffer;
		for (auto& it : matches)
			buffer.append(it + L'\n');
		return buffer;
	}
	else if (counts)
	{
		return matches[0];
	}
	else return L"";
}

bool			UltimaAPI::TagContent(std::wstring source, std::wstring tag, size_t index_start, size_t index_end, std::vector<std::wstring>* ret, bool DeleteTag, short expression);