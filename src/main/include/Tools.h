#pragma once

#include <vector>
#include <Windows.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <iostream>

using namespace std;

class Tools
{
public:
    static DWORD GetProcess();
    static TCHAR* GetModule();
    static HANDLE GetHandle();
    static HWND GetJanela();
    static DWORD GetModuleBase(TCHAR* module, DWORD process);
    static DWORD ReadMemory(DWORD address, vector<DWORD> offsets);
    static vector<unsigned char> ReadBytes(DWORD addy, SIZE_T bytes);
    static vector<float> ReadMatrix(DWORD address);
    
    static DWORD ReadProcessMemoryWrapper(DWORD address);
    static float ReadFloatProcessMemoryWrapper(DWORD address);

    Tools(DWORD process, HWND janela, HANDLE handle, TCHAR *module) {
        this->process = process;
        this->module = module;
        this->handle = handle;
        this->Janela = janela;
    }

private:
    static DWORD process;
    static TCHAR* module;
    static HANDLE handle;
    static HWND Janela;
};