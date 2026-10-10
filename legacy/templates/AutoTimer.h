#ifndef AUTOTIMER_H
#define AUTOTIMER_H

#include <chrono>
#include <fstream>
#include <iomanip>
using namespace std;
using namespace std::chrono;

class AutoTimer {
private:
    time_point<high_resolution_clock> start;
public:
    AutoTimer();
    ~AutoTimer();
};


#endif
