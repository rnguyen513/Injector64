#include "injection.h"
#include <iostream>

namespace injection {
	bool injectDLL(const DWORD PID) {
		DWORD TID = 0;
		size_t BytesWritten = 0;
		PVOID Buffer = NULL;
		PVOID p_LoadLibrary = NULL;
		HANDLE hThread = NULL;
		HANDLE hProcess = NULL;
		HMODULE hKernel32 = NULL;

		//specify absolute path of .dll
		wchar_t wPath[MAX_PATH] = L"E:\\proj\\LibLoader\\dll\\funnydll.dll";




		//open target process & load Kernel32 LoadLibraryW
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
		if (hProcess == NULL) {
			std::cout << "No handle to process. Check PID" << std::endl;
			return false;
		}
		std::cout << "Got handle to process: " << std::dec << hProcess << std::endl;

		hKernel32 = GetModuleHandleW(L"Kernel32");
		if (hKernel32 == NULL) {
			std::cout << "No handle to Kernel32 module..." << std::endl;
			return false;
		}
		std::cout << "Got handle to Kernel32.dll: " << std::dec << hKernel32 << std::endl;
		p_LoadLibrary = GetProcAddress(hKernel32, "LoadLibraryW");
		if (p_LoadLibrary == NULL) {
			std::cout << "No address for LoadLibraryW..." << std::endl;
			return false;
		}
		std::cout << "Got address to LoadLibraryW: 0x" << std::hex << p_LoadLibrary << std::endl;

		//reserve memory in the process (call this "Buffer"), then write our payload DLL to it.
		//finally, spawn a new thread that invokes the DLL
		Buffer = VirtualAllocEx(hProcess, NULL, sizeof(wPath), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (Buffer == NULL) {
			std::cout << "Buffer is null..." << std::endl;
			return false;
		}
		std::cout << "Allocated buffer with PAGE_READWRITE [RW-] permissions! " << Buffer << std::endl;

		
		if (!WriteProcessMemory(hProcess, Buffer, wPath, sizeof(wPath), &BytesWritten)) {
			std::cout << "Couldn't write to buffer..." << std::endl;
			return false;
		}
		std::cout << "Successfully wrote [" << std::dec << BytesWritten << "] bytes to the buffer" << std::endl;

		hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)p_LoadLibrary, Buffer, 0, &TID);
		if (hThread == NULL) {
			std::cout << "Couldn't initialize thread..." << std::endl;
			return false;
		}
		std::cout << "Thread successfully created: " << hThread << std::endl;



		//at this point, thread is executing
		std::cout << "Waiting for thread to finish execution..." << std::endl;
		WaitForSingleObject(hThread, INFINITE);



		//all done, cleanup now
		std::cout << "Thread finished execution!" << std::endl;
		if (hThread) CloseHandle(hThread);
		if (hProcess) CloseHandle(hProcess);
		if (Buffer) VirtualFree(Buffer, 0, MEM_RELEASE);


		std::cout << "Exiting..." << std::endl;
		return true;
	}
}