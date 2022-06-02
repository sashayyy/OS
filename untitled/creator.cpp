#include<iostream>
#include<fstream>
#include<string>
#include <cstdlib>
#include "employee.h"

int main_(int argc, char** argv) {
    int n = atoi(argv[1]);
    employee* emps = new employee[n];
    for (int i = 0; i < n; i++) {
        std::cout << "Input number, name and hours of working of employee " << i + 1 << ": ";
        int num;
        std::cin >> num;
        std::string name;
        std::cin >> name;
        double h;
        std::cin >> h;
        emps[i] = employee(num, name.c_str(), h);
    }

    std::ofstream out(argv[2], std::ios::binary);
    out.write((const char*)emps, sizeof(employee) * n);
    out.close();
}
