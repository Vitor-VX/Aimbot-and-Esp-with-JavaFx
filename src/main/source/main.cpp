#include <iostream>
#include <Windows.h>
#include "Tools.h"
#include "Funcoes.h"

using namespace std;
#define M_PI 3.14159

TCHAR gameModule[] = _T("ac_client.exe");
HWND janela = FindWindowA(NULL, "AssaultCube");

volatile bool aimbotAutomaticActivate = false, noRecoilActivate = false, aimbotFromShot = false, HackingLoopActive = false, bulletInfinityActivate = false, espLinhaActivate = false;

volatile int valorTypeEsp = 0;

extern "C" __declspec(dllexport) bool EspType(int valor)
{
    static bool activate = true;

    if (activate)
    {
        valorTypeEsp = valor;
    }

    activate = !activate;
    return !activate;
}

extern "C" __declspec(dllexport) void ModFunctions();

extern "C" __declspec(dllexport) bool StopHacking()
{
    return HackingLoopActive = !HackingLoopActive;
}

extern "C" __declspec(dllexport) bool StopAimbotAutomatic()
{
    return aimbotAutomaticActivate = !aimbotAutomaticActivate;
}

extern "C" __declspec(dllexport) bool StopAimbotFromShot()
{
    return aimbotFromShot = !aimbotFromShot;
}

extern "C" __declspec(dllexport) bool StopNoRecoil()
{
    return noRecoilActivate = !noRecoilActivate;
}

extern "C" __declspec(dllexport) boolean StopBalletInfity()
{
    return bulletInfinityActivate = !bulletInfinityActivate;
}

extern "C" __declspec(dllexport) boolean EspLinhaActivate()
{
    return espLinhaActivate = !espLinhaActivate;
}

void ModFunctions()
{
    if (!janela)
    {
        cout << "Erro ao tentar encontrar a janela!" << endl;
        return;
    }

    DWORD processId;
    GetWindowThreadProcessId(janela, &processId);
    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, true, processId);
    
    Tools tools = Tools(processId, janela, handle, gameModule);
    Esp::ViewMatrix viewMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    Esp esp;

    if (handle)
    {
        while (HackingLoopActive)
        {
            if (aimbotFromShot)
            {
                AimbotFromTiro();
            }

            if (aimbotAutomaticActivate)
            {
                AimbotAutomatic();
            }

            if (noRecoilActivate)
            {
                NoRecoil();
            }

            if (bulletInfinityActivate)
            {
                InfiniteAmmo();
            }

            if (espLinhaActivate)
            {
                EspLinha(valorTypeEsp, viewMatrix, esp);
            }

            Sleep(1);
        }
    }
}