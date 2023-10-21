#include "../include/data_processing.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../include/unixTime.h"
#include "../include/data_converter.h"
#include <cstdint>
#include <vector>


using namespace std;

//parse
void parse(string str, string &id, Event &event) {
    std::string payload;
    int i = 0;
    id = "";

    while (str[i] != '#') {
        id.push_back(str.at(i));
        i++;
    }

    //convert id from hex to dec
    uint16_t id_dec = hexToDec(id);

    //skip #
    i++;

    while (i < str.length()) {
        payload.push_back(str[i]);
        i++;
    }

    //check if the payload is valid, so divisible by 2 and length between 2 and 16
    if (payload.length() % 2 != 0 || payload.length() > 16 || payload.length() < 2) {
        cout << "Payload not valid!" << endl;
    } else {
        //create a vector of strings to store the parsed payload
        vector<string> parsed_payload;
        int payload_dec;
        string string_into_vector;

        //loop through the payload with a step of 2
        for (int j = 0; j < payload.length(); j = j + 2) {
            string str2 = payload.substr(j, 2);
            //convert the payload from hex to dec
            payload_dec = hexToDec(str2);
            str2 = std::to_string(payload_dec);
            string position;
            if (j == 0) {
                position = "st";
            } else {
                if (j == 2) {
                    position = "nd";
                } else{
                    if (j == 4){
                        position = "rd";
                    } else {
                        position = "th";
                    }
                }
            }

            //assign the string position to the string_into_vector, with the payload in decimal
            if(j==0) {
                string_into_vector = "1" + position + " byte -> " + str2 + " in decimal";

            }else{
                string_into_vector = std::to_string(j/2 + 1) + position + " byte -> " + str2 + " in decimal";
            }

            //check if the id is 160 and the payload is 26367. In this case the stop signal is received
            if(id_dec == 160 && hexToDec(payload) == 26367){
                event = E_STOP;
            }

            //check if the id is 160, so 0A0 in hexadecimal, and the payload is 26113 or 65281. In this case the start signal is received
            if(id_dec == 160 && (hexToDec(payload) == 26113 || hexToDec(payload) == 65281)){
                event = E_START;
            }

            //push the formatted string into the vector
            parsed_payload.emplace_back(string_into_vector);
            cout << string_into_vector << endl;

        }

    }
}

void log(string str, std::ofstream& MyFile){
    // Write to the file with a timestamp and the string
    MyFile << getUnixTimestamp() << " " << str << endl;
}
