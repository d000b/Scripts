#pragma once

namespace UltimaAPI
{
	namespace Memory
	{
		template <typename type>  bool memset(void*, void*, type);
		template <typename type>  bool memcpy(type*, type*, size_t);
	}
}

template <typename type>
bool	UltimaAPI::Memory::memset(void* begin, void* end, type value)
{
	if (begin > end)
		return false;
	while (begin != end)
		*static_cast<type*>(begin++) = value;
	return true;
}

template <typename type>
bool	UltimaAPI::Memory::memcpy(type* to, type* from, size_t count)
{
	if (from == to)
		return true;
	if (count > 1)
	{
		if (from < to && from + count >= to)
			for (size_t i = count - 1; i != -1; i--)
				to[i] = from[i];
		else for (size_t i = 0; i < count; i++)
				to[i] = from[i];
	}
	else *to = *from;
	return true;
}
