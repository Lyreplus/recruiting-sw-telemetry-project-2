//
// Created by lyreplus on 18/10/23.
//

#include "data_processing.h"
#include <iostream>
#include <string>
#include "unixTime.h"
#include "data_converter.h"
#include <cstdint>
#include <vector>


using namespace std;

void parse(std::string str, string &id, Event &event) {
    std::string payload;
    int i = 0;
    id = "";

    while (str[i] != '#') {
        id.push_back(str.at(i));
        i++;
    }

    uint16_t id_dec = hexToDec(id);

    cout << "ID: " << id_dec << endl;

    i++;//skip #

    while (i < str.length()) {
        payload.push_back(str[i]);
        i++;
    }

    if (payload.length() % 2 != 0 || payload.length() > 16 || payload.length() < 2) {
        cout << "Payload not valid!" << endl;
    } else {
        vector<string> parsed_payload;
        int payload_dec = 0;
        string string_into_vector;
        for (int j = 0; j < payload.length(); j = j + 2) {
            string str2 = payload.substr(j, 2);
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

            if(j==0) {
                string_into_vector = "1" + position + " byte -> " + str2 + " in decimal";

            }else{
                string_into_vector = std::to_string(j/2 + 1) + position + " byte -> " + str2 + " in decimal";
            }

            if(id_dec == 160 && hexToDec(payload) == 26367){
                event = E_STOP;
            }

            if(id_dec == 160 && (hexToDec(payload) == 26113 || hexToDec(payload) == 65281)){
                event = E_START;
            }

            parsed_payload.emplace_back(string_into_vector);


        }

        for(string s : parsed_payload){
            cout << s << endl;
        }

    }
}

void log(string str, std::ofstream& MyFile){
    // Write to the file
    MyFile << getUnixTimestamp() << " " << str << endl;
}

void statistics(){
    //TODO
}