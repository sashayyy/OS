#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
using namespace std;

struct threadArgs{
    std::vector<int> arr;
    int n;
    int num;
    bool actions[2];
    condition_variable doAction;
    mutex actionMutex;
    threadArgs(std::vector<int> _arr, int _n, int _num) : arr(_arr), num(_num),  n(_n){
        actions[0] = false;
        actions[1] = false;
    }
};

