#pragma once
#include <Windows.h>

#define DLL L"dll\\funnydll.dll"

namespace injection {
	bool injectDLL(const DWORD PID);
}