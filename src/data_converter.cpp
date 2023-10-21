#include <string>
#include <cmath>
#include "../include/data_converter.h"

//Converts hex represented as string to decimal
int hexToDec(std::string str){
    int hexToDec = 0;
    int z = 0;
    //loop through the string from the end
    for(int y = str.length()-1; y>=0; y--) {
        char a = str[y];
        //if the character is a number convert it to int
        if (a >= 48 && a <= 57) {
            int ia = a - '0';
            hexToDec += ia * (int) pow(16, z);
        } else {
            //if the character is a letter convert it to int
            if (a >= 65 && a <= 70) {
                int ia = a - 55;
                hexToDec += ia * (int) pow(16, z);
            }
        }
        z++;
    }
    return hexToDec;
}