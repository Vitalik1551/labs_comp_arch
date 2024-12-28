#include "pch.h"
#include "rectangle.h"

double absolute(double value) {
    return (value < 0) ? -value : value;
}

double CalculateRectangleArea(double x1, double y1, double x2, double y2) {
    return absolute(x2 - x1) * absolute(y2 - y1);
}
