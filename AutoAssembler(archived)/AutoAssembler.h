#include "windows.h"
#include "hde/hde32.h"
#pragma once;

class AutoAssembler
{
public:
	AutoAssembler() {}
	AutoAssembler(DWORD origAddr, DWORD FuncAddr, PBYTE _FuncBytes) 
	{
		Constructor(origAddr, FuncAddr, _FuncBytes);
	}
	inline void	Constructor(DWORD origAddr, DWORD funcAddr, PBYTE funcBytes)
	{
		OriginalAddress = origAddr;
		FuncLength = GetFuncSize(funcBytes);
		for (int i = 0; i < FuncLength; i++)
			FuncBytes[i] = funcBytes[i];
		FuncAddress = funcAddr;
	}
	void 	PrintOriginalBytes() 
	{
		for (int i = 0; i < OriginalLength; i++)
			printf("%X ", OriginalBytes[i]);
		printf("\n");
	};
	void	Assembler()
	{
		if (!cpyOriginal)
		{
			DWORD oldProt = 0;
			hde32s hde;
			bool first = true;
			if (WithoutJump)
			{
				do
				{
					hde32_disasm((PVOID)(OriginalAddress + hde.len), &hde);
					OriginalLength += hde.len;
				} while (OriginalLength < FuncLength);
			}
			else
			{
				do
				{
					hde32_disasm((PVOID)(OriginalAddress + hde.len), &hde);
					OriginalLength += hde.len;
					if (hde.len > FuncLength && first)
					{
						WithoutJump = true;
						break;
					}
					first = false;
				} while (OriginalLength < 5);
			}
			VirtualProtect((PVOID)OriginalAddress, OriginalLength, PAGE_EXECUTE_READWRITE, &oldProt);
			for (int i = 0; i < OriginalLength; i++)
				memcpy(&OriginalBytes[i], (BYTE*)(OriginalAddress + i), sizeof(BYTE));
			VirtualProtect((PVOID)OriginalAddress, OriginalLength, oldProt, &oldProt);
			printf("Copy orig bytes\n");
			cpyOriginal = true;
		}
		if (OriginalLength >= FuncLength)
		{
			printf("OriginalLength >= FuncLength\n");
			DWORD oldProt = 0;
			PBYTE func = new BYTE[OriginalLength];
			for (int i = 0; i < OriginalLength; i++)
			{
				if (i < FuncLength)
					func[i] = *(BYTE*)(FuncBytes + i);
				else func[i] = 0x90;
			}
			VirtualProtect((PVOID)OriginalAddress, OriginalLength, PAGE_EXECUTE_READWRITE, &oldProt);
			memcpy((PBYTE)OriginalAddress, func, OriginalLength);
			VirtualProtect((PVOID)OriginalAddress, OriginalLength, oldProt, &oldProt);
			delete[] func;
		}
		else {
			if (OriginalLength >= 5 && FuncLength > OriginalLength)
			{
				printf("OriginalLength >= 5 && FuncLength > OriginalLength\n");
				DWORD oldProt = 0;
				PBYTE jump = new BYTE[OriginalLength];
				DWORD _jmpAddress = FuncAddress - OriginalAddress - 5;
				DWORD _retrAddress = OriginalAddress - FuncAddress - OriginalLength + 5;
				DWORD returns = FuncAddress + FuncLength - 5;
				BYTE reth[5];
				jump[0] = reth[0] = 0xE9;

				for (int i = 5; i < OriginalLength; i++)
					jump[i] = 0x90;

				for (int i = 0; i < 4; i++)
					jump[i + 1] = _jmpAddress >> 8 * i;

				for (int i = 0; i < 4; i++)
					reth[i + 1] = _retrAddress >> 8 * i;

				VirtualProtect((PVOID)returns, 5, PAGE_EXECUTE_READWRITE, &oldProt);
				memcpy((PVOID)returns, reth, 5);
				VirtualProtect((PVOID)returns, 5, oldProt, &oldProt);

				VirtualProtect((PVOID)OriginalAddress, OriginalLength, PAGE_EXECUTE_READWRITE, &oldProt);
				memcpy((PVOID)OriginalAddress, jump, OriginalLength);
				VirtualProtect((PVOID)OriginalAddress, OriginalLength, oldProt, &oldProt);
				delete[] jump;
			}
			else if (WithoutJump)
			{
				printf("WithoutJump");
				DWORD oldProt = 0;
				PBYTE func = new BYTE[OriginalLength];
				for (int i = 0; i < OriginalLength; i++)
				{
					if (i < FuncLength)
						func[i] = *(BYTE*)(FuncLength + i);
					else func[i] = 0x90;
				}
				VirtualProtect((PVOID)OriginalAddress, OriginalLength, PAGE_EXECUTE_READWRITE, &oldProt);
				memcpy((PBYTE)OriginalAddress, func, OriginalLength);
				VirtualProtect((PVOID)OriginalAddress, OriginalLength, oldProt, &oldProt);
				delete[] func;
			} 
		}
	}
	void	ReturnBytes()
	{
		DWORD oldProt = 0;
		VirtualProtect((PVOID)OriginalAddress, OriginalLength, PAGE_EXECUTE_READWRITE, &oldProt);
		for (int i = 0; i < OriginalLength; i++)
			memcpy((BYTE*)(OriginalAddress + i), (BYTE*)(OriginalBytes + i), sizeof(BYTE));
		VirtualProtect((PVOID)OriginalAddress, OriginalLength, oldProt, &oldProt);
	}

	void	SetCode(DWORD Address, BYTE Code[], int Length)
	{
		DWORD oldProt = 0;
		hde32s hde;

		CodeAddress = Address;
		for (int i = 0; i < Length; i++)
			CodeOrigBytes[i] = Code[i];
		CodeLength = Length;

		do
		{
			hde32_disasm((PVOID)Address, &hde);
			CodeOriginalLength += hde.len;
		} while (CodeOriginalLength < Length);
		PBYTE func = new BYTE[CodeOriginalLength];
		for (int i = 0; i < CodeOriginalLength; i++)
		{
			memcpy(&CodeOrigBytes[i], (BYTE*)(Address + i), sizeof(BYTE));
			if (i < Length)
				func[i] = *(BYTE*)(Code + i);
			else func[i] = 0x90;
		}
		VirtualProtect((PVOID)Address, CodeOriginalLength, PAGE_EXECUTE_READWRITE, &oldProt);
		memcpy((PBYTE)Address, func, CodeOriginalLength);
		VirtualProtect((PVOID)Address, CodeOriginalLength, oldProt, &oldProt);
		delete[] func;
	}
	void	SetCode(DWORD Address, BYTE Code[])
	{
		DWORD oldProt = 0;
		hde32s hde;
		int Length = sizeof(Code);

		CodeAddress = Address;
		for (int i = 0; i < Length; i++)
			CodeOrigBytes[i] = Code[i];
		CodeLength = Length;

		do
		{
			hde32_disasm((PVOID)Address, &hde);
			CodeOriginalLength += hde.len;
		} while (CodeOriginalLength < Length);
		PBYTE func = new BYTE[CodeOriginalLength];
		for (int i = 0; i < CodeOriginalLength; i++)
		{
			memcpy(&CodeOrigBytes[i], (BYTE*)(Address + i), sizeof(BYTE));
			if (i < Length)
				func[i] = *(BYTE*)(Code + i);
			else func[i] = 0x90;
		}
		VirtualProtect((PVOID)Address, CodeOriginalLength, PAGE_EXECUTE_READWRITE, &oldProt);
		memcpy((PBYTE)Address, func, CodeOriginalLength);
		VirtualProtect((PVOID)Address, CodeOriginalLength, oldProt, &oldProt);
		delete[] func;
	}
	void	SetCode(DWORD Address)
	{
		DWORD oldProt = 0;
		hde32s hde;
		int AddrLength = 0;

		CodeAddress = Address;
		for (int i = 0; i < OriginalLength; i++)
		{
			CodeFuncBytes[i] = OriginalBytes[i];
		}
		CodeLength = OriginalLength;

		do
		{
			hde32_disasm((PVOID)Address, &hde);
			AddrLength += hde.len;
		} while (AddrLength < OriginalLength);
		OriginalLength = AddrLength;
		PBYTE func = new BYTE[AddrLength];
		for (int i = 0; i < AddrLength; i++)
		{
			memcpy(&CodeOrigBytes[i], (BYTE*)(Address + i), sizeof(BYTE));
			if (i < AddrLength)
				func[i] = *(BYTE*)(OriginalBytes + i);
			else func[i] = 0x90;
		}
		VirtualProtect((PVOID)Address, AddrLength, PAGE_EXECUTE_READWRITE, &oldProt);
		memcpy((PBYTE)Address, func, AddrLength);
		VirtualProtect((PVOID)Address, AddrLength, oldProt, &oldProt);
		delete[] func;
	}
	void	ReturnCode()
	{
		DWORD oldProt = 0;
		VirtualProtect((PVOID)CodeAddress, CodeLength, PAGE_EXECUTE_READWRITE, &oldProt);
		for (int i = 0; i < CodeLength; i++)
			memcpy((BYTE*)(CodeAddress + i), (BYTE*)(CodeOrigBytes + i), sizeof(BYTE));
		VirtualProtect((PVOID)CodeAddress, CodeLength, oldProt, &oldProt);
	}
private:
	int		GetFuncSize(BYTE* f)
	{
		for(int i = 0; i < 256; i++)
			if ((BYTE)*(f + i) == 0xC3)
				return i;
		return 0;
	};

	BYTE	OriginalBytes[64];
	DWORD	OriginalAddress;
	int		OriginalLength;
	bool	cpyOriginal = false;
	bool	WithoutJump = false;

	BYTE	FuncBytes[64];
	DWORD	FuncAddress;
	int		FuncLength;

	BYTE	CodeOrigBytes[64];
	BYTE	CodeFuncBytes[64];
	DWORD	CodeAddress;
	int		CodeOriginalLength;
	int		CodeLength;
};
