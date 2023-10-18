//
// Created by lyreplus on 18/10/23.
//
#include <string>
#include <cmath>
#include "data_converter.h"


int hexToDec(std::string str){
    int hexToDec = 0;
    int z = 0;
    for(int y = str.length()-1; y>=0; y--) {
        char a = str[y];
        if (a >= 48 && a <= 57) {
            int ia = a - '0';
            hexToDec += ia * (int) pow(16, z);
        } else {
            if (a >= 65 && a <= 70) {
                int ia = a - 55;
                hexToDec += ia * (int) pow(16, z);
            }
        }
        z++;
    }
    return hexToDec;
}