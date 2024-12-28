#include <windows.h>
#include <iostream>
#include <thread>

// 4.Курсор мыши работает как «распылитель» : цвет пикселей, на которые попадает «распыление», меняется на цвет, 
// выбранный пользователем.При длительном нахождении курсора на одном месте пиксели вокруг него закрашиваются интенсивнее.

// Цвет для "распылителя"
COLORREF sprayColor = RGB(0, 255, 0); // Зеленый

// Функция для отображения информации о системе (только цифры)
void showSystemInfo() {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    std::cout << width << " " << height << std::endl;

    DEVMODE devMode = { 0 };
    devMode.dmSize = sizeof(devMode);
    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode)) {
        std::cout << devMode.dmPelsWidth << " "
            << devMode.dmPelsHeight << " "
            << devMode.dmDisplayFrequency << std::endl;
    }
}

// Установка системного цвета
void setSystemColors() {
    COLORREF newColors[1] = { RGB(255, 0, 0) }; // Красный
    int elements[1] = { COLOR_BACKGROUND };
    SetSysColors(1, elements, newColors);
}

// Функция для "распылителя" 
void sprayCursor(HDC hdc) {
    POINT cursorPos;
    static POINT lastPos = { -1, -1 };
    static int intensity = 1;

    while (true) {
        GetCursorPos(&cursorPos);
        ScreenToClient(GetDesktopWindow(), &cursorPos);

        if (cursorPos.x == lastPos.x && cursorPos.y == lastPos.y) {
            intensity++;
        }
        else {
            intensity = 1;
        }

        lastPos = cursorPos;

        for (int dx = -intensity; dx <= intensity; dx++) {
            for (int dy = -intensity; dy <= intensity; dy++) {
                SetPixel(hdc, cursorPos.x + dx, cursorPos.y + dy, sprayColor);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

// Запуск "распылителя"
void startSpray() {
    HDC hdc = GetDC(NULL);
    std::thread sprayThread(sprayCursor, hdc);
    sprayThread.detach();
}

int main() {
    // Устанавливаем UTF-8 для вывода в консоль
    SetConsoleOutputCP(CP_UTF8);

    // Вывод информации о системе (только цифры)
    showSystemInfo();
    setSystemColors();

    // Старт "распылителя"
    startSpray();

    // Главный цикл сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;       
}