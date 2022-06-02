#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include <cstdlib>
#include "employee.h"

int compareOfName(const void* p1, const void* p2) {
    employee* e1 = (employee*)p1;
    employee* e2 = (employee*)p2;
    return strcmp(e1->name, e2->name);
}

int Main(int argc, char** argv) {
    std::string inputFileName = argv[1];
    std::string reportFileName = argv[2];
    int salary = atoi(argv[3]);

    std::ifstream in(inputFileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    in.seekg(0, std::ios::end);
    int n = in.tellg() / sizeof(employee);
    in.seekg(0, std::ios::beg);
    employee* allEmployees = new employee[n];
    in.read((char*)allEmployees, n * sizeof(employee));

    qsort(allEmployees, n, sizeof(employee), compareOfName);

    std::ofstream out(reportFileName.c_str());
    out << "Input file name: " << inputFileName << '\n'
        << std::setw(5) << "Number" << std::setw(12) << "Name"
        << std::setw(5) << "Hours" << std::setw(12) << "Salary" << '\n';
    for (int i = 0; i < n; i++) {
        out << std::setw(5) << allEmployees[i].num << std::setw(12) << allEmployees[i].name
            << std::setw(10) << allEmployees[i].hours << std::setw(12) << allEmployees[i].hours * salary << '\n';
    }
    out.close();
}