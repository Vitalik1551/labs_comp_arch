#pragma once

#ifdef DYNAMICCIRCLEDRAWER_EXPORTS
#define CIRCLE_API __declspec(dllexport)
#else
#define CIRCLE_API __declspec(dllimport)
#endif

extern "C" {
    CIRCLE_API void DrawCircle(HDC hdc, int x, int y, int radius, COLORREF color);
}