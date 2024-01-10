#pragma once
#include <Windows.h>
#include <vector>
#include <iostream>

using namespace std;

class Esp
{
public: 
	struct ViewMatrix {
		float m11, m12, m13, m14;
		float m21, m22, m23, m24;
		float m31, m32, m33, m34;
		float m41, m42, m43, m44;
	};

	POINT WorldToScreen(ViewMatrix mtx, float posX, float posY, float posZ, int width, int height);
	void DrawCenteredLine(HDC hdc, int screenWidth, int screenHeight, POINT wst, COLORREF lineColor, int typeEsp);
	vector<int> ScreenSize(HWND janela);
};

