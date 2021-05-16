#pragma once

namespace UltimaAPI
{
	namespace Memory
	{
		template <typename type>  bool memset(void*, void*, type);
	}
}

template <typename type>
bool UltimaAPI::Memory::memset(void* begin, void* end, type value)
{
	if (begin > end)
		return false;
	while (begin != end)
	{
		*static_cast<type*>(begin) = value;
		++begin;
	}
	return true;
}

