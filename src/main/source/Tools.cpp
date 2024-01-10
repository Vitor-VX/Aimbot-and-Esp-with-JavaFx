#include "Tools.h"

DWORD Tools::process = 0;
TCHAR* Tools::module = nullptr;
HANDLE Tools::handle = nullptr;
HWND Tools::Janela = nullptr;

DWORD Tools::GetProcess() {
    return process;
}

TCHAR* Tools::GetModule() {
    return module;
}

HANDLE Tools::GetHandle() {
    return handle;
}

HWND Tools::GetJanela() {
    return Janela;
}

DWORD Tools::GetModuleBase(TCHAR* module, DWORD process)
{
    DWORD dwBaseAdrress = 0;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process);
    MODULEENTRY32 ModuleEntry = { 0 };
    ModuleEntry.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &ModuleEntry))
    {
        do
        {
            if (_tcscmp(ModuleEntry.szModule, module) == 0)
            {
                dwBaseAdrress = reinterpret_cast<DWORD_PTR>(ModuleEntry.modBaseAddr);
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry));
    }

    CloseHandle(hSnapshot);
    return dwBaseAdrress;
}

DWORD Tools::ReadMemory(DWORD address, vector<DWORD> offsets)
{
    HANDLE pHandle = NULL;
    DWORD gameModule = GetModuleBase(Tools::GetModule(), Tools::GetProcess());

    pHandle = OpenProcess(PROCESS_ALL_ACCESS, true, Tools::GetProcess());

    const DWORD ERROR_OPEN_PROCESS_FAILED = 1;

    if (pHandle == INVALID_HANDLE_VALUE || pHandle == NULL)
    {
        return ERROR_OPEN_PROCESS_FAILED;
    }

    DWORD offset_Null = NULL;
    ReadProcessMemory(pHandle, (LPVOID)(gameModule + address), &offset_Null, sizeof(offset_Null), 0);

    DWORD pointerMemoryAddress = offset_Null;

    for (int i = 0; i < offsets.size() - 1; i++)
    {
        ReadProcessMemory(pHandle, (LPVOID)(pointerMemoryAddress + offsets.at(i)), &pointerMemoryAddress, sizeof(pointerMemoryAddress), 0);
    }

    return pointerMemoryAddress += offsets.at(offsets.size() - 1);
}

vector<unsigned char> Tools::ReadBytes(DWORD addy, SIZE_T bytes)
{
    vector<unsigned char> buffer(bytes);
    SIZE_T bytesRead;
    ReadProcessMemory(Tools::GetHandle(), (LPCVOID)addy, &buffer[0], bytes, &bytesRead);
    return buffer;
}

DWORD Tools::ReadProcessMemoryWrapper(DWORD address) {
    DWORD value = 0;

    if (ReadProcessMemory(Tools::GetHandle(), (LPVOID*)address, &value, sizeof(value), 0)) {
        return value;
    }
}

float Tools::ReadFloatProcessMemoryWrapper(DWORD address) {
    float value = 0.0f;

    if (ReadProcessMemory(Tools::GetHandle(), (LPVOID*)address, &value, sizeof(value), 0)) {
        return value;
    }
}

vector<float> Tools::ReadMatrix(DWORD address)
{
    const SIZE_T bytes = 4 * 16;
    vector<unsigned char> buffer = ReadBytes(address, bytes);
    vector<float> matrix(bytes / sizeof(float));

    for (int i = 0; i < matrix.size(); ++i)
    {
        matrix[i] = *reinterpret_cast<float*>(&buffer[i * 4]);
    }

    return matrix;
}