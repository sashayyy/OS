#include <iostream>
#include <mutex>
#include <vector>
#include <condition_variable>
#include "Thread_info.h"
using namespace std;

mutex startMutex;
mutex stateMutex;
mutex markerMutex;

condition_variable startCV;
condition_variable stateChanged;

bool isReady;
vector<bool> threadsPaused;

int getRandomNumber(int arg) {
    srand(time(NULL));
    return ((rand() * 10000 / 5 + 123) % 65 * 8 % 123 * 902 / 2 + 12345) % arg;
}

void printArray(std::vector<int> arr){
    std::lock_guard<std::mutex> lk(markerMutex);
    int size = arr.size();
    for(int i = 0; i < size; ++i){
        std::cout << arr[i];
    }
    std::cout << '\n';
}

bool allPaused(std::vector<bool> arr){
    int size = arr.size();
    for(int i = 0; i < size; ++i){
        if(!arr[i]) {
            return false;
        }
    }
    return true;
}

void marker(threadArgs* args){
    unique_lock<std::mutex> startlk(startMutex);
    startCV.wait(startlk, []()->bool{return isReady;});
    startlk.unlock();
    cout << "Thread " << args->num << " has been started.\n";

    int count = 0;
    while (true) {
        unique_lock<std::mutex> lk(markerMutex);
        int i = getRandomNumber(args->n);

        if (args->arr[i] != 0) {
            cout << "Thread №" << args-> num << ", marked " << count << "elements, " << " unable to mark №" << i << '\n';

            threadsPaused[args->num - 1] = true;
            stateChanged.notify_all();
            lk.unlock();
            unique_lock<std::mutex> actionLock(args->actionMutex);

            args->doAction.wait(actionLock, [=]()->bool{return args->actions[1] || args->actions[0];});

            if(args->actions[1]){
                for(int i = 0; i < args->n; ++i) {
                    if (args->arr[i] == args->num) {
                        args->arr[i] = 0;
                    }
                }

                cout << "Thread" << args->num << "has been terminated.";
                break;
            }

            args->actions[0] = false;
        } else{
            this_thread::sleep_for(std::chrono::milliseconds(5));
            ++count;
            args->arr[i] = args->num;
            lk.unlock();
            this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
}

int main() {
    int n;
    n = getRandomNumber(15) + 5;
    vector<int> arr;
    for(int i = 0; i < n; ++i){
        arr.push_back(0);
    }
    cout << "Array of " << n << " elements has been created.\n";

    int threadCount = getRandomNumber(5) + 5;
    vector<std::thread> threads;
    vector<threadArgs*> argsVec;

    vector<bool> terminated(threadCount);
    for(int i = 0; i < threadCount; ++i){
        terminated[i] = false;
        argsVec.push_back(new threadArgs(arr, n, i+1));
        threads.push_back(std::thread(marker, argsVec.back()));
    }
    cout << threadCount << "threads ate ready to start.\n";
    isReady = true;
    startCV.notify_all();

    int terminatedCount = 0;
    while(terminatedCount != threadCount){
        unique_lock<std::mutex> lk(stateMutex);
        stateChanged.wait(lk, [=](){return allPaused(threadsPaused);});

        printArray(arr);

        cout << "All threads are paused. Which one is to terminate?\n";
        int k;
        cin >> k;
        if(k <= 0 || k > threadCount || terminated[k - 1]){
            cout << "Invalid input...\n";
            continue;
        }

        argsVec[k-1]->actions[1] = true;
        argsVec[k-1]->doAction.notify_one();
        terminated[k-1] = true;
        threads[k-1].join();

        ++terminatedCount;
        printArray(arr);

        for(int i = 0; i < threadCount; ++i){
            if(!terminated[i]) {
                threadsPaused[i] = false;
                argsVec[i]->actions[0] = true;
                argsVec[i]->doAction.notify_one();
            }
        }
    }

    cout << "All threads are terminated.\n";
    return 0;
}
