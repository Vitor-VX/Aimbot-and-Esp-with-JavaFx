#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>
#include <cmath>
#include "Tools.h"
#include "Esp.h"

#define M_PI 3.14159

using namespace std;

void InfiniteAmmo()
{
    vector<DWORD> offsetsBala = { 0x140 };
    DWORD bala = Tools::ReadMemory(0x0018AC00, offsetsBala);
    DWORD valorBala = 100;

    WriteProcessMemory(Tools::GetHandle(), (LPVOID*)bala, &valorBala, sizeof(valorBala), 0);
}

DWORD GetAddressMouse()
{
    vector<DWORD> OffesetsMouse = { 0x38 };
    DWORD mousePointer = Tools::ReadMemory(0x0017E0A8, OffesetsMouse);

    return mousePointer;
}

DWORD GetAddressCordVFeet()
{
    vector<DWORD> offsetsCordPlayer = { 0x28 };
    DWORD cordPlayer = Tools::ReadMemory(0x0018AC00, offsetsCordPlayer);

    return cordPlayer;
}

DWORD GetAddressCordPlayer()
{
    vector<DWORD> offsetsCordPlayer = { 0x4 };
    DWORD cordPlayer = Tools::ReadMemory(0x0018AC00, offsetsCordPlayer);

    return cordPlayer;
}

int GetAmountPlayers()
{
    vector<DWORD> offsetsQuantidadePlayer = { 0 };
    DWORD pointerPlayersValue = Tools::ReadMemory(0x0005C434, offsetsQuantidadePlayer);

    return pointerPlayersValue;
}

DWORD GetAddressLocalPlayer()
{
    vector<DWORD> OffesetsPlayerBase = { 0xEC };
    DWORD playerBasePointer = Tools::ReadMemory(0x0018AC00, OffesetsPlayerBase);

    return playerBasePointer;
}

DWORD GetAddressTeam()
{
    vector<DWORD> OffesetsPointerEquipe = { 0x30C };
    DWORD pointerPosition = Tools::ReadMemory(0x0018AC00, OffesetsPointerEquipe);

    return pointerPosition;
}

DWORD GetAddressVisibleSight()
{
    vector<DWORD> offesetsVisible = { 0x30C };
    DWORD pointerVisible = Tools::ReadMemory(0x0018AC04, offesetsVisible);

    return pointerVisible;
}

void NoRecoil()
{
    DWORD valorNoRecoil = 2425393296;

    WriteProcessMemory(Tools::GetHandle(), (LPVOID*)(0x400000 + 0xC2EC3), &valorNoRecoil, sizeof(valorNoRecoil), 0);
}

void AimbotSettings() {
    DWORD PositonPlayer = GetAddressCordPlayer();
    DWORD quantidadePlayer = GetAmountPlayers();
    DWORD mouseXYAddress = GetAddressMouse();

    float abspos_playerX = 0, abspos_playerY = 0, abspos_playerZ = 0, abspos_botX = 0, abspos_botY = 0, abspos_botZ = 0;
    float closest_player = -1.0f;
    float closest_yaw = 0.0f;
    float closest_pitch = 0.0f;

    DWORD quantidadesPlayers = Tools::ReadProcessMemoryWrapper(quantidadePlayer);

    for (DWORD i = 0; i < quantidadesPlayers; i++) {
        vector<DWORD> entityOffsets = { (i * 4), 0 };

        DWORD entityBots = Tools::ReadMemory(0x00191FCC, entityOffsets);
        DWORD equipe = GetAddressTeam();

        DWORD equipePlayer = 0, equipeBots = 0, vidaBot = 0;
        DWORD botVida = entityBots + 0xEC;
        DWORD timeBot = entityBots + 0x30C;

        equipeBots = Tools::ReadProcessMemoryWrapper(timeBot);
        equipePlayer = Tools::ReadProcessMemoryWrapper(equipe);
        vidaBot = Tools::ReadProcessMemoryWrapper(botVida);


        DWORD visibleBot = entityBots + 0x3FC;
        float visible = Tools::ReadFloatProcessMemoryWrapper(visibleBot);

        if ((equipeBots != equipePlayer) && (vidaBot > 0 && vidaBot <= 100))
        {
            if (visible != 0 && visible != -1)
            {
                DWORD playerX = PositonPlayer;
                DWORD playerY = PositonPlayer + 0x4;
                DWORD playerZ = PositonPlayer + 0xC;

                DWORD botX = entityBots + 0x4;
                DWORD botY = entityBots + 0x8;
                DWORD botZ = entityBots + 0xC;

                abspos_playerX = Tools::ReadFloatProcessMemoryWrapper(playerX);
                abspos_playerY = Tools::ReadFloatProcessMemoryWrapper(playerY);
                abspos_playerZ = Tools::ReadFloatProcessMemoryWrapper(playerZ);

                abspos_botX = Tools::ReadFloatProcessMemoryWrapper(botX);
                abspos_botY = Tools::ReadFloatProcessMemoryWrapper(botY);
                abspos_botZ = Tools::ReadFloatProcessMemoryWrapper(botZ);

                float CalcX = abspos_botX - abspos_playerX;
                float CalcY = abspos_botY - abspos_playerY;
                float CalcZ = abspos_botZ - abspos_playerZ;

                float distance = sqrt((CalcX * CalcX) + (CalcY * CalcY));

                if ((closest_player == -1.0f || distance < closest_player))
                {
                    closest_player = distance;

                    float yaw = (float)(atan2f(CalcY, CalcX) * (180.0 / M_PI)) + 90;
                    closest_yaw = yaw;

                    float pitch = (float)(atan2f(CalcZ, distance) * (180.0 / M_PI));

                    if (pitch) {
                        pitch = 0;
                    }

                    WriteProcessMemory(Tools::GetHandle(), (LPVOID*)(mouseXYAddress - 0x4), &yaw, sizeof(yaw), 0);
                    WriteProcessMemory(Tools::GetHandle(), (LPVOID*)mouseXYAddress, &pitch, sizeof(pitch), 0);
                }
            }
        }
    }
}

void AimbotAutomatic()
{
    AimbotSettings();
}

void AimbotFromTiro()
{
    if (GetAsyncKeyState(VK_LBUTTON))
    {
        AimbotSettings();
    }
}

void EspLinha(int valorTypeEsp, Esp::ViewMatrix viewMatrix, Esp esp)
{
    if (valorTypeEsp != 0)
    {
        float abspos_playerX = 0, abspos_playerY = 0, abspos_playerZ = 0, abspos_botX = 0, abspos_botY = 0, abspos_botZ = 0;

        DWORD addressViewMatrix = Tools::GetModuleBase(Tools::GetModule(), Tools::GetProcess());

        DWORD PositonPlayer = GetAddressCordPlayer();

        DWORD quantidadePlayer = GetAmountPlayers();

        DWORD equipe = GetAddressTeam();

        DWORD quantidadesPlayers = 0, equipePlayer = 0, equipeBots = 0;

        ReadProcessMemory(Tools::GetHandle(), (LPVOID*)quantidadePlayer, &quantidadesPlayers, sizeof(quantidadesPlayers), nullptr);

        vector<float> mtx = Tools::ReadMatrix(addressViewMatrix + (0x17DFFC - 0x6C + 0x4 * 16));

        vector<int> scrennSizeJanela = esp.ScreenSize(Tools::GetJanela());

        viewMatrix.m11 = mtx[0];
        viewMatrix.m12 = mtx[1];
        viewMatrix.m13 = mtx[2];
        viewMatrix.m14 = mtx[3];

        viewMatrix.m21 = mtx[4];
        viewMatrix.m22 = mtx[5];
        viewMatrix.m23 = mtx[6];
        viewMatrix.m24 = mtx[7];

        viewMatrix.m31 = mtx[8];
        viewMatrix.m32 = mtx[9];
        viewMatrix.m33 = mtx[10];
        viewMatrix.m34 = mtx[11];

        viewMatrix.m41 = mtx[12];
        viewMatrix.m42 = mtx[13];
        viewMatrix.m43 = mtx[14];
        viewMatrix.m44 = mtx[15];

        for (DWORD i = 0; i < quantidadesPlayers; i++)
        {
            vector<DWORD> entityOffsets = { (i * 4), 0 };
            DWORD entityBots = Tools::ReadMemory(0x00191FCC, entityOffsets);

            DWORD botVida = entityBots + 0xEC;
            DWORD timeBot = entityBots + 0x30C;
            DWORD vidaBot = 0;

            equipeBots = Tools::ReadProcessMemoryWrapper(timeBot);
            equipePlayer = Tools::ReadProcessMemoryWrapper(equipe);
            vidaBot = Tools::ReadProcessMemoryWrapper(botVida);

            DWORD playerX = PositonPlayer;
            DWORD playerY = PositonPlayer + 0x4;
            DWORD playerZ = PositonPlayer + 0xC;

            DWORD botX = entityBots + 0x4;
            DWORD botY = entityBots + 0x8;
            DWORD botZ = entityBots + 0xC;

            if (
                abspos_playerX = Tools::ReadFloatProcessMemoryWrapper(playerX),
                abspos_playerY = Tools::ReadFloatProcessMemoryWrapper(playerY),
                abspos_playerZ = Tools::ReadFloatProcessMemoryWrapper(playerZ),

                abspos_botX = Tools::ReadFloatProcessMemoryWrapper(botX),
                abspos_botY = Tools::ReadFloatProcessMemoryWrapper(botY),
                abspos_botZ = Tools::ReadFloatProcessMemoryWrapper(botZ)
              )
            {
                POINT wtsFeet = esp.WorldToScreen(viewMatrix, abspos_botX, abspos_botY, abspos_botZ, scrennSizeJanela.at(0), scrennSizeJanela.at(1));
                HDC hdc = GetDC(Tools::GetJanela());

                if (wtsFeet.x > 0)
                {
                    if (equipeBots != equipePlayer && vidaBot > 0 && vidaBot <= 100)
                    {
                        COLORREF corEsp = RGB(255, 0, 0);

                        esp.DrawCenteredLine(hdc, scrennSizeJanela.at(0), scrennSizeJanela.at(1), wtsFeet, corEsp, valorTypeEsp);
                    }
                    else if (vidaBot > 0 && vidaBot <= 100)
                    {
                        COLORREF corEsp = RGB(0, 255, 0);
                        esp.DrawCenteredLine(hdc, scrennSizeJanela.at(0), scrennSizeJanela.at(1), wtsFeet, corEsp, valorTypeEsp);
                    }

                    ReleaseDC(Tools::GetJanela(), hdc);
                }
            }
        }
    }
    else
    {
        return;
    }
}