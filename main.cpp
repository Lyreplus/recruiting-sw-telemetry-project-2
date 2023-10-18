#include <cstdio>
#include <iostream>
#include <ranges>
#include <thread>
//#include "StateMachine.h"
using namespace std;
#include <fstream>
#include "fake_receiver.h"
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
int numFile = 0;

extern "C"{
    #include "fake_receiver.h"
}


/*catch STOP event*/
void checkS1();

/*catch START event*/
void checkS2();

void dump();

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
    cout << "s1" << endl;
    while(event == E_STOP || event == E_NONE);
}

void checkS2(){
    cout << "s2" << endl;
    while(event == E_START);
}

void idleThread(){
    cout << "Siamo in idle" << endl;
    char message[20];
    while(event != E_START && event != E_ENDFILE){
        unique_lock<mutex> idleLock(LogMutex);
        if(can_receive(message) != -1){
            idleLock.unlock();
            //cout << message << endl;
            std::string str(message);
            parse(str, id, event);
        }else{
            idleLock.unlock();
            unique_lock<mutex> eventLock(LogMutex);
            event = E_ENDFILE;
            eventLock.unlock();
        }
    }


}

void runThread(){

    cout << "Siamo in run; " << endl;
    map<string, tuple<uint, double, long>> rows;

        char message[20];

        //create filename with datetime
        time_t t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
        string str_format = oss.str();

        string filename = "file" + str_format + ".txt";
        ofstream MyFile(filename, std::ios::out | std::ios::binary);
        while (event != E_STOP && event != E_ENDFILE) {
            unique_lock<mutex> can_protection(LogMutex);
            if(can_receive(message) != -1){
                can_protection.unlock();
                long start = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

                std::string str(message);
                parse(str, id, event);
                if(!(rows.find(id) == rows.end())){
                    long end = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
                    long delta = end - start;
                    //cout << "Tempo trascorso: " << delta << " millisecondi" << endl;
                    uint amount = get<0>(rows[id]);
                    long alpha = get<2>(rows[id]);
                    long beta = end - alpha;
                    rows[id] = tuple<uint, double, long>(amount+1, (get<1>(rows[id])*amount + beta)/(amount+1), end);
                }else{
                    long end = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
                    long delta = end - start;
                    rows[id] = tuple<uint, double, long>(1, delta, end);
                }
                log(str, MyFile);
            } else {
                can_protection.unlock();
                unique_lock<mutex> runLock(LogMutex);
                event = E_ENDFILE;
                runLock.unlock();
            }
        }

        //create a csv file with the statistics
        ofstream csvFile(to_string(numFile) + " statistics.csv");
        csvFile << "ID,number_of_messages,mean_time " << endl;
        for(auto& row : rows){
            csvFile << row.first << ", " << get<0>(row.second) << ", " << get<1>(row.second) << ", " << endl;
        }
        csvFile.close();
        numFile++;
        MyFile.close();
}
