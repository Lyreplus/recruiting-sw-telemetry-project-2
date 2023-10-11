#include <iostream>
#include <ctime>
#include <unistd.h>
#include <string>
#include <sstream>

void printUnixTimestamp(int seconds);
std::string getUnixTimestamp();
std::string convertTimeToString(std::time_t time);


int main(){
  std::cout << "Ciao" << std::endl;

  std::time_t result = std::time(nullptr);
  std::cout << std::asctime(std::localtime(&result)) << result << " seconds since the Epoch\n";

  printUnixTimestamp(2);
  std::cout << "The current time is " << getUnixTimestamp() << std::endl;
    
  return 0;
}

void printUnixTimestamp(int seconds){
    std::time_t result = std::time(nullptr);
    unsigned int microsecond = 1000000;

    for (int i(0); i<seconds; i++) {
        result = std::time(nullptr);
        std::cout << std::asctime(std::localtime(&result)) << result << " seconds since the Epoch\n" << std::endl;
        usleep(1*microsecond);
    }
}

std::string getUnixTimestamp(){
    std::time_t actual_time = std::time(nullptr);
    return convertTimeToString(actual_time);
}

std::string convertTimeToString(std::time_t time){
    std::stringstream stringStream;
    stringStream << time;
    return stringStream.str();
}