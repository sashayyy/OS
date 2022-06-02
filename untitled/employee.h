#ifndef FIRST_LAB_EMPLOYEE_H
#define FIRST_LAB_EMPLOYEE_H
#include <cstring>

struct employee
{
public:
    int num;
    char name[10];
    double hours;

    employee() {
        num = 0;
        strcpy(name, "Noname");
        hours = 0.;
    }
    employee(int num_, const char name_[], double hours_) {
        num = num_;
        strcpy(name, name_);
        hours = hours_;
    }
};

#endif //FIRST_LAB_EMPLOYEE_H