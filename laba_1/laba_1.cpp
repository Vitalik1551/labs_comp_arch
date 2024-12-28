#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Для выбранной пользователем папки реализовать вывод на экран список  файлов в 
// данной папке.Подсчитать суммарный объём этих файлов.Полученную
// информацию записать в файл по выбору пользователя.


// Функция для вывода логических дисков
void DisplayLogicalDrives() {
    char driveStrings[256];
    DWORD result = GetLogicalDriveStringsA(sizeof(driveStrings), driveStrings);

    if (result == 0) {
        std::cerr << "Error: Could not retrieve logical drives." << std::endl;
        return;
    }

    std::cout << "Logical drives:" << std::endl;
    for (char* drive = driveStrings; *drive; drive += strlen(drive) + 1) {
        std::cout << drive << std::endl;
    }
}

// Функция для вывода свободного места на диске
void DisplayDiskFreeSpace(const std::string& drive) {
    ULARGE_INTEGER freeBytesAvailable, totalBytes, freeBytes;

    if (GetDiskFreeSpaceExA(drive.c_str(), &freeBytesAvailable, &totalBytes, &freeBytes)) {
        std::cout << "Drive: " << drive << std::endl;
        std::cout << "Total space: " << totalBytes.QuadPart / (1024 * 1024) << " MB" << std::endl;
        std::cout << "Free space: " << freeBytes.QuadPart / (1024 * 1024) << " MB" << std::endl;
    }
    else {
        std::cerr << "Error: Could not retrieve disk space for " << drive << std::endl;
    }
}

// Функция для отображения пути временной папки
void DisplayTempPath() {
    char tempPath[MAX_PATH];
    if (GetTempPathA(sizeof(tempPath), tempPath)) {
        std::cout << "Temporary path: " << tempPath << std::endl;
    }
    else {
        std::cerr << "Error: Could not retrieve temporary path." << std::endl;
    }
}

// Функция для отображения атрибутов файла
void DisplayFileAttributes(const std::string& filePath) {
    DWORD attributes = GetFileAttributesA(filePath.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        std::cerr << "Error: Could not get attributes for file " << filePath << std::endl;
        return;
    }

    std::cout << "Attributes for " << filePath << ":" << std::endl;
    if (attributes & FILE_ATTRIBUTE_READONLY) std::cout << "Read-only" << std::endl;
    if (attributes & FILE_ATTRIBUTE_HIDDEN) std::cout << "Hidden" << std::endl;
    if (attributes & FILE_ATTRIBUTE_SYSTEM) std::cout << "System" << std::endl;
    if (attributes & FILE_ATTRIBUTE_DIRECTORY) std::cout << "Directory" << std::endl;
}

// Функция для сравнения времён файлов
void CompareFileTimes(const std::string& file1, const std::string& file2) {
    HANDLE hFile1 = CreateFileA(file1.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hFile2 = CreateFileA(file2.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile1 == INVALID_HANDLE_VALUE || hFile2 == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Could not open one or both files for comparison." << std::endl;
        if (hFile1 != INVALID_HANDLE_VALUE) CloseHandle(hFile1);
        if (hFile2 != INVALID_HANDLE_VALUE) CloseHandle(hFile2);
        return;
    }

    FILETIME ft1, ft2;
    GetFileTime(hFile1, NULL, NULL, &ft1);
    GetFileTime(hFile2, NULL, NULL, &ft2);

    CloseHandle(hFile1);
    CloseHandle(hFile2);

    int comparison = CompareFileTime(&ft1, &ft2);
    if (comparison < 0) {
        std::cout << file1 << " is older than " << file2 << std::endl;
    }
    else if (comparison > 0) {
        std::cout << file1 << " is newer than " << file2 << std::endl;
    }
    else {
        std::cout << file1 << " and " << file2 << " have the same timestamp." << std::endl;
    }
}

// Функция для подсчёта файлов и их размера
void ProcessDirectory(const std::string& directoryPath, const std::string& outputFilePath) {
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind;

    std::string searchPath = directoryPath + "\\*";
    hFind = FindFirstFileA(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Could not opesdfsn directory " << directoryPath << std::endl;
        return;
    }

    std::ofstream outputFile(outputFilePath, std::ios::out);
    if (!outputFile) {
        std::cerr << "Error: Could not open file " << outputFilePath << " for writing." << std::endl;
        FindClose(hFind);
        return;
    }

    outputFile << "Files in directory: " << directoryPath << std::endl;
    std::cout << "Files in directory: " << directoryPath << std::endl;

    ULONGLONG totalSize = 0;

    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            std::string fileName = findFileData.cFileName;

            // Получаем размер файла
            ULONGLONG fileSize = (static_cast<ULONGLONG>(findFileData.nFileSizeHigh) << 32) | findFileData.nFileSizeLow;
            totalSize += fileSize;

            // Записываем информацию о файле в консоль и файл
            std::cout << fileName << " - " << fileSize << " bytes" << std::endl;
            outputFile << fileName << " - " << fileSize << " bytes" << std::endl;
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    // Вывод общей информации
    std::cout << "Total size of files: " << totalSize << " bytes" << std::endl;
    outputFile << "Total size of files: " << totalSize << " bytes" << std::endl;

    // Закрываем файл и дескриптор поиска
    outputFile.close();
    FindClose(hFind);
}

// Основная функция
int main() {
    SetConsoleOutputCP(CP_UTF8);
    // Демонстрация работы с логическими дисками
    DisplayLogicalDrives();

    // Демонстрация свободного места на диске C:
    DisplayDiskFreeSpace("C:\\");

    // Демонстрация временной папки
    DisplayTempPath();

    // Обработка директории
    std::string directoryPath;
    std::string outputFilePath;

    std::cout << "Enter the path to the directory: ";
    std::getline(std::cin, directoryPath);

    std::cout << "Enter the path to the output file: ";
    std::getline(std::cin, outputFilePath);

    ProcessDirectory(directoryPath, outputFilePath);

    return 0;
}