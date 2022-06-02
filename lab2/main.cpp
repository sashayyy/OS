#include <iostream>
#include "myArray.h"
#include <windows.h>
#include <process.h>

void inputArray(myArray a, int size) {
    for(int i = 0; i < size; i = -~i) {
        std::cout << "Input " << -~i << " element of array: ";
        std::cin >> a.storage[i];
    }
}

void outputArray(myArray a, int size) {
    std::cout << "All elements in array: ";
    for(int i = 0; i < size; i = -~i) {
        std::cout << a.storage[i] << ' ';
    }
}

UINT WINAPI min_max(void* temp){
    myArray* array = static_cast<myArray*>(temp);
    int maxI = 0;
    int minI = 0;
    for(int i = 1; i < array->size; i = -~i) {
        if(array->storage[maxI] < array->storage[i]){
            maxI = i;
        }
        Sleep(7);
        if(array->storage[minI] > array->storage[i]){
            minI = i;
        }
        Sleep(7);
    }
    std::cout << "Max element is: " << array->storage[maxI] << ", index: " << maxI << '\n';
    array->maxElement = maxI;
    std::cout << "Min element is: " << array->storage[minI] << ", index: " << minI << '\n';
    array->minElement = minI;
    return 0;
}

UINT WINAPI average(void* temp){
    myArray* array = static_cast<myArray*>(temp);
    int sum = 0;
    for(int i = 0; i < array->size; i = -~i){
        sum += array->storage[i];
        Sleep(12);
    }
    array->average = sum / array->size;
    std::cout << "Average number: " << array->average << '\n';
    return 0;
}

int main() {
    std::cout << "Input size of array: ";
    int n;
    std::cin >> n;

    myArray* array = new myArray();
    array->storage = new int[n];
    array->size = n;
    inputArray(*array, n);

    HANDLE handleMinMax =
            (HANDLE) _beginthreadex(NULL, 0, min_max, array, 0, NULL);
    if (handleMinMax == NULL) {
        return GetLastError();
    }
    WaitForSingleObject(handleMinMax, INFINITE);

    HANDLE handleAverage =
            (HANDLE) _beginthreadex(NULL, 0, average, array, 0, NULL);
    if(handleAverage == NULL){
        return GetLastError();
    }
    WaitForSingleObject(handleAverage, INFINITE);

    array->storage[array->maxElement] = array->average;
    array->storage[array->minElement] = array->average;

    outputArray(*array, n);

    return 0;
}
