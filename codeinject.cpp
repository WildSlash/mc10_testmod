#include <Windows.h>
#include <stdint.h>
#include <Psapi.h>

// base address
static uintptr_t BaseAddress;
HANDLE process;
uintptr_t SlideAddress(uintptr_t offset) {
	return BaseAddress + offset;
}

uintptr_t** VTBlock;

uint32_t changeBedrockColor()
{
	return 0x00FF00FF; // purple
}

bool bedrockBlocksChests()
{
	return false;
}

bool removeBedrockCollision()
{
	return false;
}



bool minecraftH4x0r()
{
	VTBlock = (uintptr_t**)SlideAddress(0x99AFE8);

	VTBlock[5] = (uintptr_t*)&bedrockBlocksChests;
	VTBlock[14] = (uintptr_t*)&removeBedrockCollision;
	VTBlock[59] = (uintptr_t*)&changeBedrockColor;

	return true;
}

// find base ptr dynamically
DWORD_PTR GetProcessBaseAddress(DWORD processID)
{
	DWORD_PTR baseAddress = 0;
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	HMODULE* moduleArray;
	LPBYTE moduleArrayBytes;
	DWORD bytesRequired;

	if(processHandle)
	{
		if(EnumProcessModules(processHandle, NULL, 0, &bytesRequired))
		{
			if(bytesRequired)
			{
				moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

				if(moduleArrayBytes)
				{
					unsigned int moduleCount;

					moduleCount = bytesRequired / sizeof(HMODULE);
					moduleArray = (HMODULE*)moduleArrayBytes;

					if(EnumProcessModules(processHandle, moduleArray, bytesRequired, &bytesRequired))
					{
						baseAddress = (DWORD_PTR)moduleArray[0];
					}

					LocalFree(moduleArrayBytes);
				}
			}
		}

		CloseHandle(processHandle);
	}

	return baseAddress;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	DWORD procId = GetCurrentProcessId();
	process = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, FALSE, procId);
	BaseAddress = (uintptr_t)GetProcessBaseAddress(procId);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		return minecraftH4x0r();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
