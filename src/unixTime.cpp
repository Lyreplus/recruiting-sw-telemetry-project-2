#include <iostream>
#include "../include/unixTime.h"
#include <ctime>
#include <unistd.h>
#include <string>
#include <sstream>

//debug function to print the unix timestamp
void printUnixTimestamp(int seconds){
    std::time_t result = std::time(nullptr);
    unsigned int microsecond = 1000000;

    for (int i(0); i<seconds; i++) {
        result = std::time(nullptr);
        std::cout << std::asctime(std::localtime(&result)) << result << " seconds since the Epoch\n" << std::endl;
        usleep(1*microsecond);
    }
}

//function to get the unix timestamp
std::string getUnixTimestamp(){
    std::time_t actual_time = std::time(nullptr);
    return convertTimeToString(actual_time);
}

//function to convert the unix timestamp to string
std::string convertTimeToString(std::time_t time){
    std::stringstream stringStream;
    stringStream << time;
    return stringStream.str();
}