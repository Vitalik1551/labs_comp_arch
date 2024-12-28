#include "pch.h"
#include "circle.h"
#include <windows.h>

void DrawCircle(HDC hdc, int x, int y, int radius, COLORREF color) {
    HBRUSH hBrush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}
