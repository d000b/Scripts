#pragma once

#include "../Vector/Vector_old.h"

#include "detour/detours.h"
#if defined(_WIN64) && _WIN64
#pragma comment(lib, "D:/Github/Scripts/Detours/detour/detours_x64")
#else
#pragma comment(lib, "D:/Github/Scripts/Detours/detour/detours_x86")
#endif

namespace UltimaAPI
{
	class Detours;
}

class	UltimaAPI::Detours
{
public:
	enum eCodes
	{
		code_original = -1,
		code_successful = NO_ERROR,
		code_null = ERROR_INVALID_HANDLE,
		code_small = ERROR_INVALID_BLOCK,
		code_memory = ERROR_NOT_ENOUGH_MEMORY,
		code_pending = ERROR_INVALID_OPERATION
	};
	struct element
	{
		bool  b;
		long  c;
		void* h;
		void* o;
	};

	Vector<bool>  need;
	Vector<long>  code;
	Vector<void*> hook;
	Vector<void*> orig;

	decltype(auto) attach() noexcept
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		for (size_t i = 0; i < hook.size(); i++)
			if (need.data()[i])
				code[i] = DetourAttach(&orig.data()[i], hook.data()[i]);
		DetourTransactionCommit();
	}
	decltype(auto) detach() noexcept
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		for (size_t i = 0; i < orig.size(); i++)
			if (need.data()[i])
				code[i] = DetourDetach(&orig.data()[i], hook.data()[i]);
		DetourTransactionCommit();
	}
	decltype(auto) shrink_to_fit()
	{
		need.shrink_to_fit();
		code.shrink_to_fit();
		hook.shrink_to_fit();
		orig.shrink_to_fit();
	}
	decltype(auto) push_back(void* original_function, void* hooker_function, bool b = true) noexcept
	{
		need.push_back(b);
		code.push_back(eCodes::code_original);
		hook.push_back(hooker_function);
		orig.push_back(original_function);
	}
	decltype(auto) free() noexcept
	{
		need.free();
		code.free();
		hook.free();
		orig.free();
	}
	decltype(auto) reserve(size_t al) noexcept
	{
		need.reserve(al);
		code.reserve(al);
		hook.reserve(al);
		orig.reserve(al);
	}
	decltype(auto) test_size() noexcept
	{
		return hook.size() != orig.size();
	}
	decltype(auto) test_code() noexcept
	{
		for (auto&& _ : code)
			if (!_) return true; // _ != eCodes::code_successful
		return false;
	}
	decltype(auto) operator()()
	{
		if (test_size())
			throw(Detours());

		attach();
	}
	decltype(auto) operator()(void* org, void* hk, bool b = true) noexcept
	{
		push_back(org, hk, b);
	}
	decltype(auto) operator[](size_t i) noexcept
	{
		return element{ need[i], code[i], hook[i], orig[i] };
	}

	template <typename Fn> Fn o(size_t i)
	{
		return reinterpret_cast<Fn>(orig[i]);
	}
	template <typename Fn, typename...Args> Fn O(size_t i, Args...args)
	{
		return reinterpret_cast<Fn>(orig[i])(args...);
	}

	Detours() = default;
	Detours(size_t al) noexcept
	{
		reserve(al);
	}
	~Detours()
	{
		detach();
		free();
	}
};
