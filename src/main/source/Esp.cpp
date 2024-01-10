#include "Esp.h"

POINT Esp::WorldToScreen(ViewMatrix mtx, float posX, float posY, float posZ, int width, int height)
{
    POINT twoD;

    float screenW = (mtx.m14 * posX) + (mtx.m24 * posY) + (mtx.m34 * posZ) + mtx.m44;

    if (screenW > 0.001f)
    {
        float screenX = (mtx.m11 * posX) + (mtx.m21 * posY) + (mtx.m31 * posZ) + mtx.m41;
        float screenY = (mtx.m12 * posX) + (mtx.m22 * posY) + (mtx.m32 * posZ) + mtx.m42;

        float camX = static_cast<float>(width) / 2.0f;
        float camY = static_cast<float>(height) / 2.0f;

        float X = camX + (camX * screenX / screenW);
        float Y = camY - (camY * screenY / screenW);

        twoD.x = static_cast<int>(X);
        twoD.y = static_cast<int>(Y);

        return twoD;
    }
    else
    {
        twoD.x = -99;
        twoD.y = -99;
        return twoD;
    }
}

void Esp::DrawCenteredLine(HDC hdc, int screenWidth, int screenHeight, POINT wst, COLORREF lineColor, int typeEsp)
{
    HPEN hPen = CreatePen(PS_SOLID, 2, lineColor);
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);

    switch (typeEsp)
    {
    case 1:
        // 1 cima
        MoveToEx(hdc, screenWidth / 2, 0, NULL);
        break;
    case 2:
        // 2 cima baixo
        MoveToEx(hdc, screenWidth / 2, screenHeight, NULL);
        break;
    }

    LineTo(hdc, wst.x, wst.y);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

vector<int> Esp::ScreenSize(HWND janela) {
    RECT clientRect;
    
    GetClientRect(janela, &clientRect);
    int screenWidth = clientRect.right - clientRect.left;
    int screenHeight = clientRect.bottom - clientRect.top;
    vector<int> value = { screenWidth, screenHeight };

    return value;
}