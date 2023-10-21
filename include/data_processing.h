// Function declarations for data processing functions
#ifndef RECRUITING_SW_TELEMETRY_PROJECT_2_DATA_PROCESSING_H
#define RECRUITING_SW_TELEMETRY_PROJECT_2_DATA_PROCESSING_H

#include <iostream>
#include <string>
#include "types.h"

using namespace std;

void parse(string str, string &id, Event &event);

void log(string str, std::ofstream& MyFile);


#endif //RECRUITING_SW_TELEMETRY_PROJECT_2_DATA_PROCESSING_H
