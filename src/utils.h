#ifndef LAB_INTEGRALS_UTILS_H
#define LAB_INTEGRALS_UTILS_H

#include <string>
#include <map>


int readConfig(std::map<std::string, double> &conf, int &num_of_threads, char* filename);

#endif //LAB_INTEGRALS_UTILS_H