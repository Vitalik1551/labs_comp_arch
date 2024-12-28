#include <iostream>
#include <limits>
#define NOMINMAX
#include <windows.h>
#include "rectangle.h"

bool getIntInput(const std::string& prompt, int& value) {
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter an integer value: ";
    }
    return true;
}

bool getDoubleInput(const std::string& prompt, double& value) {
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a numeric value: ";
    }   
    return true;
}

int main() {
    double x1, y1, x2, y2;
    getDoubleInput("Enter the coordinates of the first corner of the rectangle (x1): ", x1);
    getDoubleInput("Enter the coordinates of the first corner of the rectangle (y1): ", y1);
    getDoubleInput("Enter the coordinates of the opposite corner of the rectangle (x2): ", x2);
    getDoubleInput("Enter the coordinates of the opposite corner of the rectangle (y2): ", y2);

    double area = CalculateRectangleArea(x1, y1, x2, y2);
    std::cout << "The area of the rectangle is: " << area << std::endl;

    HMODULE hModule = LoadLibrary(L"DynamicCircleDrawer.dll");
    if (!hModule) {
        std::cerr << "Failed to load DynamicCircleDrawer.dll" << std::endl;
        return 1;
    }

    typedef void(*DrawCircleFunc)(HDC, int, int, int, COLORREF);    
    DrawCircleFunc DrawCircle = (DrawCircleFunc)GetProcAddress(hModule, "DrawCircle");
    if (!DrawCircle) {
        std::cerr << "Failed to get the address of DrawCircle function" << std::endl;
        FreeLibrary(hModule);
        return 1;
    }

    int centerX, centerY, radius;
    getIntInput("Enter the center x-coordinate of the circle (x): ", centerX);
    getIntInput("Enter the center y-coordinate of the circle (y): ", centerY);
    getIntInput("Enter the radius of the circle: ", radius);

    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);

    DrawCircle(hdc, centerX, centerY, radius, RGB(255, 0, 0));

    ReleaseDC(hwnd, hdc);
    FreeLibrary(hModule);

    std::cout << "Press Enter to exit." << std::endl;
    std::cin.ignore();
    std::cin.get();

    return 0;
}
