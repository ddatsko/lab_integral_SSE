#include <iostream>
#include <cmath>
#include <map>
#include <stdexcept>
#include "utils.h"
#include "IntegralCalculator.h"
#include "TimeCounter.h"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Wrong number of arguments" << std::endl;
        return -2;
    }


    std::map<std::string, double> conf;
    int numOfThreads = 0;
    if (readConfig(conf, numOfThreads, argv[1]) != 0)
        return -1;

    IntegralCalculator calculator(conf["x_start"], conf["x_end"], conf["y_start"], conf["y_end"]);
    TimeCounter timeCounter;

    timeCounter.startCount();
    std::cout << "Starting counting...." << std::endl;

    int intervals = 1000;
    double newRes = calculator.calculate(intervals, numOfThreads);
    double res;
    int iteration = 0;
    do {
        intervals *= 2;
        res = newRes;
        newRes = calculator.calculate(intervals, numOfThreads);

        iteration++;
    } while ((std::abs(res - newRes) > conf["abs_error"] ||
              std::abs(newRes - res) / newRes > conf["rel_error"]) &&
             iteration <= 15);

    double timeElapsed = timeCounter.timeElapsed();
    std::cout << std::fixed << timeElapsed << " " << newRes << " " << std::abs(res - newRes) << " " << std::abs(newRes - res) / newRes;
}