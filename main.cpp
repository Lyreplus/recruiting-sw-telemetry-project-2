//#include <cstdio>
#include <iostream>
#include <ranges>
#include <thread>
//#include "StateMachine.h"
using namespace std;
#include <fstream>
#include "fake_receiver.h"
#include <cmath>
#include <string>
#include <vector>
#include "unixTime.h"
#include <iomanip>

extern "C"{
    #include "fake_receiver.h"
}

//static bool s_Finished = false;
//
//void DoWork(){
//    while(!s_Finished){
//        std::cout << "Great Scott its working!" << std::endl;
//
//    }
//};

/*catch START event*/
void checkS1(){

}

/*catch STOP event*/
void checkS2(){

}

int hexToDec(std::string str){
    int hexToDec = 0;
    int z = 0;
    for(int y = str.length()-1; y>=0; y--){
        char a = str.at(y);
        if(a >= 48 && a<=57){
            int ia = a - '0';
            hexToDec += ia*(int)pow(16, z);
        }else{
            if(a>= 65 && a<=70){
                int ia = a - 55;
                hexToDec += ia*(int)pow(16, z);
            }
        }
        z++;
    }
    return hexToDec;
}

/*actual functions that do the work*/
void parse(std::string str) {
    std::string id;
    std::string payload;
    int i = 0;

    while (str[i] != '#') {
        id.push_back(str[i]);
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

            parsed_payload.emplace_back(string_into_vector);


        }

        for(const string& s : parsed_payload){
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

void idleThread(){
    int a = open_can("candump.log");
    cout << "Codice apertura file: " << a << endl;
    char message[10];
    while(can_receive(message) != -1){
        cout << message << endl;
        std::string str(message);
        cout << str << endl;
        parse(str);
    }
    close_can();
}

void runThread(){
    int a = open_can("candump.log");
    cout << "Codice apertura file: " << a << endl;
    char message[10];

    //create filename with datetime
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    auto str_format = oss.str();

    string filename = "file" + str_format;
    ofstream MyFile(filename);
    while (can_receive(message) != -1) {
        cout << message << endl;
        std::string str(message);
        cout << str << endl;
        parse(str);
        log(str, MyFile);
    }
    close_can();
    MyFile.close();
}

/*enum with the various states*/
typedef enum {
    STATE_INIT,
    STATE_IDLE,
    STATE_RUN,
    NUM_STATES
}State_t;

/*state machine*/
typedef struct{
    State_t state;
    void (*state_function)();
} StateMachine_t;

/*prototypes of the function corresponding to the states*/
void fn_INIT();
void fn_IDLE();
void fn_RUN();

/*initialization to the init state*/
State_t current_state = STATE_INIT;

/*initialization of the fsm*/
StateMachine_t fsm[] = {
        {STATE_INIT, fn_INIT},
        {STATE_IDLE, fn_IDLE},
        {STATE_RUN, fn_RUN}
};

/*enum with the various events*/
typedef enum {
    E_NONE,
    E_START,
    E_STOP
} Event;

/*initialization of the current event*/
Event event = E_NONE;

/*declarations of the functions that correspond to the states*/
void fn_INIT(){
    if (event == E_NONE){
        //start thread
        std::thread idle(idleThread);
        current_state = STATE_IDLE;
    }
}

void fn_IDLE(){
    if (event == E_START){
        //TODO
        current_state = STATE_RUN;
    }
}

void fn_RUN(){
    if(event ==E_STOP){
        //TODO
        current_state = STATE_IDLE;
    }
}

//void eventDetector(){
//
//}

int main(void){
    cout << "Welcome to Project 2" << endl;

    //playing with threads
    //starting worker thread
//    std::thread worker(DoWork);
//
//    //enter will set s_Finished to true causing thread to stop
//    std::cin.get();
//    s_Finished = true;
//
//    //wait the thread to finish its execution
//    worker.join();


    idleThread();
    //runThread();
    cout << "End of Project 2" << endl;

    return 0;
}