#include <iostream>
#include <Windows.h>
#include <fstream>

BOOL write(DWORD process_id, char* dll_path);
DWORD get_mc_proccess_id();
BOOL exists(char* dll);

int main(int argc, char** argv)
{
    char* dll_path = argv[1];
    if (exists(dll_path)) {
        write(get_mc_proccess_id(), dll_path);
        Sleep(5000);
        exit(0);
        return 0;
    }
    else {
        std::cout << "[Inj] dll doesnt exists in " << dll_path << std::endl;
        Sleep(5000);
        exit(-1);
        return -1;
    }
    return -1;
}

BOOL exists(char* dll) {
    std::ifstream infile(dll);
    return infile.good();
}

DWORD get_mc_proccess_id()
{
    DWORD proccessID = NULL;
    HWND wind_handle = FindWindowA(NULL, "Minecraft 1.8.9");
    GetWindowThreadProcessId(wind_handle, &proccessID);
    return proccessID;
}

BOOL write(DWORD process_id, char* dll_path)
{
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, process_id);
    if (process)
    {
        LPVOID load = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
        LPVOID alloc = VirtualAllocEx(process, NULL, strlen(dll_path), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(process, alloc, dll_path, strlen(dll_path), NULL);
        HANDLE thread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)load, alloc, 0, NULL);
        WaitForSingleObject(thread, INFINITE);
        VirtualFreeEx(process, alloc, strlen(dll_path), MEM_RELEASE);
        CloseHandle(thread);
        CloseHandle(process);
        std::cout << "[Inj] Injected" << std::endl;
        return true;
    }
    return false;
}
