#include <cstdio>
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
#include <mutex>
#include <chrono>
#include <map>
#include "data_processing.h"
#include "types.h"
#include "data_converter.h"

mutex LogMutex;
std::string id;
int endfile = 0;

extern "C"{
    #include "fake_receiver.h"
}



/*catch STOP event*/
void checkS1();

/*catch START event*/
void checkS2();


/*actual functions that do the work*/
void parse(std::string str);

void log(string str, std::ofstream& MyFile);

void statistics();

void idleThread();

void runThread();

/*enum with the various states*/
typedef enum {
    STATE_INIT,
    STATE_IDLE,
    STATE_RUN,
    STATE_ENDFILE,
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
void fn_ENDFILE();

/*initialization to the init state*/
State_t current_state = STATE_INIT;

/*initialization of the fsm*/
StateMachine_t fsm[] = {
        {STATE_INIT, fn_INIT},
        {STATE_IDLE, fn_IDLE},
        {STATE_RUN, fn_RUN},
        {STATE_ENDFILE, fn_ENDFILE}
};



/*initialization of the current event*/
Event event = E_NONE;

/*declarations of the functions that correspond to the states*/
void fn_INIT(){
    if (event == E_NONE){
        current_state = STATE_IDLE;
    }
}

void fn_IDLE(){
    std::thread thread(idleThread);
    checkS1();
    thread.join();
    if(event != E_ENDFILE){
        current_state = STATE_RUN;
    }else{
        current_state = STATE_ENDFILE;
    }
}

void fn_RUN(){
    std::thread thread(runThread);
    checkS2();
    thread.join();
    if(event != E_ENDFILE){
        current_state = STATE_IDLE;
    }else{
        current_state = STATE_ENDFILE;
    }

}

void fn_ENDFILE(){
    cout << "End Of File!" << endl;
    endfile = 1;
}

int main(void){
    cout << "Welcome to Project 2" << endl;

//    std::thread idle(idleThread);
//    idle.join();
//    sleep(2);
//    std::thread run(runThread);
//    run.join();
    int a = open_can("candump.log");
    cout << "Codice apertura file: " << a << endl;
    if(a != -1){
        while(endfile == 0){
            if(current_state < NUM_STATES){
                (*fsm[current_state].state_function) ();
            } else{
                current_state = STATE_INIT;
            }
        }
        close_can();
    }else{
        cout << "Error" << endl;
    }


    cout << "End of Project 2" << endl;

    return 0;
}

void checkS1(){
    while(event == E_STOP || event == E_NONE || event != E_ENDFILE);
}

void checkS2(){
    while(event == E_START || event != E_ENDFILE);
}
void parse(std::string str) {
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

void idleThread(){
    char message[20];
    while(event != E_START && event != E_ENDFILE){
        unique_lock<mutex> idleLock(LogMutex);
        if(can_receive(message) != -1){
            idleLock.unlock();
            cout << message << endl;
            std::string str(message);
            parse(str, id, event);
        }else{
            unique_lock<mutex> eventLock(LogMutex);
            event = E_ENDFILE;
            idleLock.unlock();
        }
    }


}

void runThread(){
    unique_lock<mutex> runLock(LogMutex);

    map<string, tuple<uint, double>> rows;

//    auto start = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//    auto end = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
//    auto delta = end - start;
//    std::cout << "Tempo trascorso: " << delta << " millisecondi" << std::endl;

        char message[20];

        //create filename with datetime
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
        auto str_format = oss.str();

        string filename = "file" + str_format;
        ofstream MyFile(filename);
        while (event != E_STOP && event != E_ENDFILE) {
            if(can_receive(message) != -1){
                cout << message << endl;
                auto start = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

                std::string str(message);
                cout << str << endl;
                parse(str, id, event);
                if(!(rows.find(id) == rows.end())){
                    auto end = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
                    auto delta = end - start;
                    cout << "Tempo trascorso: " << delta << " millisecondi" << endl;
                }
                log(str, MyFile);
            } else {
                event = E_ENDFILE;
            }

        }
        MyFile.close();

    runLock.unlock();
}
