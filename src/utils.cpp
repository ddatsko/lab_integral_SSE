#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <iostream>
#include <memory>


int readConfig(std::map<std::string, double> &conf, int &num_of_threads, char* filename) {
    std::ifstream configFile(filename);
    std::string line, key, value;
    int iteration = 0;

    while (getline(configFile, line)) {
        if (line.empty() || line[0] == '#' || line[0] == ' ')
            continue;
        std::istringstream lineStream(line);
        if (std::getline(lineStream, key, '=')) {
            if (std::getline(lineStream, value)) {
                if (iteration == 0)
                    num_of_threads = std::stoi(value);
                else
                    conf[key]  = std::stof(value);
            }
        }
        iteration++;
    }
    if (conf["x_end"] < conf["x_start"] || conf["y_end"] < conf["y_start"]) {
        std::cout << "Wrong integration limits" << std::endl;
        return -1;
    }

    if (num_of_threads <= 0 || conf["abs_error"] <= 0 || conf["rel_error"] <= 0) {
        std::cout << "Number of threads or errors in config file are <= 0" << std::endl;
        return -2;
    }
    return 0;
}

double *new_aligned_two_doubles() {
    void* p = operator new(256);
    unsigned long space = sizeof(double) * 4;
    if (std::align(16, sizeof(double) * 2, p, space)) {
        return reinterpret_cast<double *>(p);
    }
    return nullptr;
}