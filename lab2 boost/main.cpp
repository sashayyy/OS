#include <iostream>
#include <conio.h>
#include <boost/thread.hpp>
#include <boost/random.hpp>
#include <boost/chrono.hpp>
using namespace std;

const int SLEEPTIME_7 = 7;
const int SLEEPTIME_12 = 12;

struct Args {
    int* arr;
    int size;
    int maxInd;
    int minInd;
    int avg;
};

void min_max(Args* args) {
    int n = args->size;
    int* arr = args->arr;
    int maxInd, minInd;
    maxInd = minInd = 0;
    for (int i = 1; i < n; i++) {
        if (arr[maxInd] < arr[i]) {
            maxInd = i;
        }
        if (arr[minInd] > arr[i]) {
            minInd = i;
        }
        boost::this_thread::sleep_for(boost::chrono::milliseconds(SLEEPTIME7));
    }
    args->maxInd = maxInd;
    args->minInd = minInd;
    cout << "Max element: " << arr[maxInd] << endl;
    cout << "Min element: " << arr[minInd] << endl;
}

void average(Args* args) {
    int n = args->size;
    int* arr = args->arr;
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
        boost::this_thread::sleep_for(boost::chrono::milliseconds(SLEEP_TIME12));
    }
    args->avg = sum / n;
    printf("Average value: %d \n", args->avg);
}

void printArr(int* arr, int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
}

int main() {
    typedef boost::mt19937 RNGType;
    RNGType rng(time(0));
    boost::uniform_int<> arraySize(5, 15);
    boost::variate_generator<RNGType, boost::uniform_int<> dice(rng, arraySize);
    int n = dice();
    boost::uniform_int<> arrayElem(0, 35000);
    dice.distribution() = arrayElem;
    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        arr[i] = dice();
    }
    printf("Generated array: \n");
    printArr(arr, n);
    printf("\n\n");

    Args* args = new Args();
    args->size = n;
    args->arr = arr;

    //create threads
    boost::thread min_maxThread(min_max, args);
    boost::thread averageThread(average, args);
    min_maxThread.join();
    averageThread.join();

    arr[args->minInd] = arr[args->maxInd] = args->avg;
    cout << "\nChanged array: " << endl;
    printArr(arr, n);
    _getch();
    delete args;
    delete[] arr;
    return 0;
}