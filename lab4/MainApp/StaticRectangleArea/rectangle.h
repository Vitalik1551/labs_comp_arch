#pragma once

#ifdef STATICRECTANGLEAREA_EXPORTS
#define RECTANGLE_API __declspec(dllexport)
#else
#define RECTANGLE_API __declspec(dllimport)
#endif

extern "C" {
    RECTANGLE_API double CalculateRectangleArea(double x1, double y1, double x2, double y2);
}
