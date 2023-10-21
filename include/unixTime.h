//Header file for functions that deal with the unixTime
#ifndef EAGLETRT_UNIXTIME_H
#define EAGLETRT_UNIXTIME_H

#include <iostream>
#include <ctime>
#include <unistd.h>
#include <string>
#include <sstream>

void printUnixTimestamp(int seconds);
std::string getUnixTimestamp();
std::string convertTimeToString(std::time_t time);


#endif //EAGLETRT_UNIXTIME_H
