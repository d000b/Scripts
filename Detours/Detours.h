#pragma once

#define USE_NEW_DETOURS 0
#define AUTO_VERSION_CHANGE 1

#include "../Vector/Vector_old.h"

#if defined(_WIN64) && _WIN64 && (defined(USE_NEW_DETOURS) && USE_NEW_DETOURS || defined(AUTO_VERSION_CHANGE) && AUTO_VERSION_CHANGE)

#define USED_NEW_DETOURS 1

#include "detour/detours.h"
#if defined(_WIN64) && _WIN64
#pragma comment(lib, "D:/Github/Scripts/Detours/detour/detours_x64")
#else
#pragma comment(lib, "D:/Github/Scripts/Detours/detour/detours_x86")
#endif

#else
#if defined(_WIN64) && _WIN64
#error Unable to use the library for the current version!
#endif
#pragma comment(user, "The use of this version is limited to x86 processes only")

#include "detour/old/detours.h"
#pragma comment(lib, "D:/Github/Scripts/Detours/detour/old/detours.lib")

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
#if defined(USED_NEW_DETOURS) && USED_NEW_DETOURS
		long  c;
#else
		void* a;
#endif
		void* h;
		void* o;
	};

	Vector<bool>  need;
#if defined(USED_NEW_DETOURS) && USED_NEW_DETOURS
	Vector<long>  code;
#else
	Vector<void*> adrs;
#endif
	Vector<void*> hook;
	Vector<void*> orig;

	decltype(auto) attach() noexcept
	{
#if defined(USED_NEW_DETOURS) && USED_NEW_DETOURS
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		for (size_t i = 0; i < hook.size(); i++)
			if (need.data()[i])
				code[i] = DetourAttach(&orig.data()[i], hook.data()[i]);
		DetourTransactionCommit();
#else
		for (size_t i = 0; i < adrs.size(); i++)
			if (need.data()[i] && adrs.data()[i])
				orig[i] = DetourFunction((PBYTE)adrs.data()[i], (PBYTE)hook.data()[i]);
#endif
	}
	decltype(auto) detach() noexcept
	{
#if defined(USED_NEW_DETOURS) && USED_NEW_DETOURS
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		for (size_t i = 0; i < orig.size(); i++)
			if (need.data()[i])
				code[i] = DetourDetach(&orig.data()[i], hook.data()[i]);
		DetourTransactionCommit();
#else
		for (size_t i = 0; i < adrs.size(); i++)
			if (need.data()[i] && adrs.data()[i])
				DetourFunction((PBYTE)adrs.data()[i], (PBYTE)orig[i]);
#endif
	}
	decltype(auto) shrink_to_fit()
	{
		need.shrink_to_fit();
#if defined(USED_NEW_DETOURS) && USED_NEW_DETOURS
		code.shrink_to_fit();
#else
		adrs.shrink_to_fit();
#endif
		hook.shrink_to_fit();
		orig.shrink_to_fit();
	}
	decltype(auto) push_back(void* original_function, void* hooker_function, bool b = true) noexcept
	{
		need.push_back(b);
#if defined(USED_NEW_DETOURS) && USED_NEW_DETOURS
		code.push_back(eCodes::code_original);
		orig.push_back(original_function);
#else
		adrs.push_back(original_function);
#endif
		hook.push_back(hooker_function);
	}
	decltype(auto) free() noexcept
	{
		need.free();
#if defined(USED_NEW_DETOURS) && USED_NEW_DETOURS
		code.free();
#else
		adrs.free();
#endif
		hook.free();
		orig.free();
	}
	decltype(auto) reserve(size_t al) noexcept
	{
		need.reserve(al);
#if defined(USED_NEW_DETOURS) && USED_NEW_DETOURS
		code.reserve(al);
#else
		adrs.reserve(al);
#endif
		hook.reserve(al);
		orig.reserve(al);
	}
	decltype(auto) test_size() noexcept
	{
		return hook.size() != orig.size();
	}
	decltype(auto) test_code() noexcept
	{
#if defined(USED_NEW_DETOURS) && USED_NEW_DETOURS
		for (auto&& _ : code)
			if (_ != eCodes::code_successful) return true;
		return false;
#else
		for (auto&& _ : orig)
			if (!_) return true; // orig[i] == nullptr
		return false;
#endif
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
#if defined(USED_NEW_DETOURS) && USED_NEW_DETOURS
		return element{ need[i], code[i], hook[i], orig[i] };
#else
		return element{ need[i], adrs[i], hook[i], orig[i] };
#endif
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
