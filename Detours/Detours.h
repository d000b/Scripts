#pragma once

#include "../Vector/Vector_old.h"

#include "detour/detours.h"
#if defined(WIN64) && WIN64
#pragma comment(lib, "detour/detour_x64")
#else
#pragma comment(lib, "detour/detour_x86")
#endif

namespace UltimaAPI
{
	class Detours
	{
	public:
		enum eCodes
		{
			code_original = -1,
			code_successful = NO_ERROR,
			code_null =    ERROR_INVALID_HANDLE,
			code_small =   ERROR_INVALID_BLOCK,
			code_memory =  ERROR_NOT_ENOUGH_MEMORY,
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

		decltype(auto) hook()
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			for (size_t i = 0; i < hook.size(); i++)
				if (static_cast<const decltype(need)>(need)[i])
					code[i] = DetourAttach(&static_cast<const decltype(orig)>(orig)[i], static_cast<const decltype(hook)>(hook)[i]);
			DetourTransactionCommit();
		}
		decltype(auto) release()
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			for (size_t i = 0; i < orig.size(); i++)
				if (static_cast<const decltype(need)>(need)[i])
					code[i] = DetourDetach(&static_cast<const decltype(orig)>(orig)[i], static_cast<const decltype(hook)>(hook)[i]);
			DetourTransactionCommit();
		}
		decltype(auto) push_back(void* original_function, void* hooker_function, bool b = true) noexcept
		{
			need.push_back(b);
			code.push_back(eCodes::code_original);
			hook.push_back(hooker_function);
			orig.push_back(original_function);
		}
		decltype(auto) free()
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
		decltype(auto) test_code()
		{
			for (auto&& _ : code)
				if (!_) return true;
			return false;
		}
		decltype(auto) operator()()
		{
			if (test_size())
				throw;

			hook();
		}
		decltype(auto) operator()(void* org, void* hk, bool b = true)
		{
			push_back(org, hk, b);
		}
		decltype(auto) operator[](size_t i) noexcept
		{
			return element{ need[i], code[i], hook[i], orig[i] };
		}
			
		Detours() = default;
		Detours(size_t al) noexcept
		{
			reserve(al);
		}
		~Detours()
		{
			release();
			free();
		}
	};
}
