#include <chrono>
#include <iostream>

using namespace std;

time_t getMillTimeStamp()
{
    chrono::time_point<chrono::system_clock, chrono::milliseconds> tp = chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    std::time_t timestamp = tmp.count();
    return timestamp;
}