#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
#include <windows.h>
#include <iomanip>
#include "employee.h"

bool createBinFile(std::string fileName, int count) {
    STARTUPINFO si;
    PROCESS_INFORMATION piApp;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    char num[10];
    itoa(count, num, 10);
    std::string arg = "Creator.exe " + std::string(num) + " " + fileName;
    char args[255];
    strcpy(args, arg.c_str());
    bool isStarted = CreateProcess(NULL, args, NULL,
        NULL, FALSE, CREATE_NEW_CONSOLE,
        NULL, NULL, &si, &piApp);
    WaitForSingleObject(piApp.hProcess, INFINITE);
    CloseHandle(piApp.hThread);
    CloseHandle(piApp.hProcess);
    return isStarted;
}

void printFromBinFile(std::string fileName) {
    std::ifstream in(fileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    in.seekg(0, std::ios::end);
    int n = in.tellg() / sizeof(employee);
    in.seekg(0, std::ios::beg);
    employee* allEmployees = new employee[n];
    in.read((char*)allEmployees, n * sizeof(employee));
    in.close();
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(5) << allEmployees[i].num
            << std::setw(12) << allEmployees[i].name
            << std::setw(10) << allEmployees[i].hours << '\n';
    }
}

bool writeInReportFile(std::string inputFile, std::string reportFile, int salary) {
    STARTUPINFO si;
    PROCESS_INFORMATION piApp;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    char sal[10];
    itoa(salary, sal, 10);
    std::string arg = "Reporter.exe " + inputFile + " " + reportFile + " " + std::string(sal);
    char args[255];
    strcpy(args, arg.c_str());
    bool isStarted = CreateProcess(NULL, args, NULL,
        NULL, FALSE, CREATE_NEW_CONSOLE,
        NULL, NULL, &si, &piApp);
    WaitForSingleObject(piApp.hProcess, INFINITE);
    CloseHandle(piApp.hThread);
    CloseHandle(piApp.hProcess);
    return isStarted;
}

int main(int argc, char* argv[]) {
    std::cout << "Input initial filename: ";
    std::string initialFileName;
    std::cin >> initialFileName;

    std::ofstream out;

    std::cout << "Input number of records in file: ";
    int recordsNumber;
    std::cin >> recordsNumber;

    bool isCreate = createBinFile(initialFileName, recordsNumber);
    if (isCreate) {
        system("cls");
        std::cout << "File " << initialFileName << " created.\n";
        printFromBinFile(initialFileName);
        std::string reporterFileName;
        std::cout << "Input reporter file name: ";
        std::cin >> reporterFileName;
        int salary;
        std::cout << "Input salary for 1h working: ";
        std::cin >> salary;
        if (writeInReportFile(initialFileName, reporterFileName, salary)) {
            std::cout << "Report file created.\n";
        }
    }

    system("pause");
    return 0;
}
