#include <cstdio>
#include <iostream>
#include <ranges>
#include <thread>
//#include "StateMachine.h"
using namespace std;
#include <fstream>
#include "fake_receiver.h"
#include <cmath>

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

void parse(std::string str){
    std::string id;
    int i = 0;

    while(str[i] != '#'){
        id.push_back(str[i]);
        i++;
    }
    cout<< "Stringa parsata: " << id << endl;

    int hexToDec = 0;
    int z = 0;
    for(int y = id.length()-1; y>=0; y--){
        char a = id.at(y);
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

    cout << hexToDec << endl;

//    static const char hex_digits[] = "0123456789ABCDEF";
//
//    std::string output;
//    output.reserve(id.length() * 2);
//    for (unsigned char c : id)
//    {
//        output.push_back(hex_digits[c >> 4]);
//        output.push_back(hex_digits[c & 15]);
//    }

}


//void log(){
//    //TODO
//}
//
//void statistics(){
//
//}

//idle state function
//void idle(){
//
//}
//
//void run(){
//
//}
//
//typedef enum {
//    STATE_INIT,
//    STATE_IDLE,
//    STATE_RUN,
//    NUM_STATES
//}State_t;
//
//typedef struct{
//    State_t state;
//    void (*state_function)();
//} StateMachine_t;
//
//void fn_INIT();
//void fn_IDLE();
//void fn_RUN();
//
//State_t current_state = STATE_INIT;
//
//StateMachine_t fsm[] = {
//        {STATE_INIT, fn_INIT},
//        {STATE_IDLE, fn_IDLE},
//        {STATE_RUN, fn_RUN}
//};
//
//typedef enum {
//    E_NONE,
//    E_START,
//    E_STOP
//} Event;
//
//Event event = E_NONE;
//
//void fn_INIT(){
//    if (event == E_NONE){
//
//        current_state = STATE_IDLE;
//    }
//}
//
//void fn_IDLE(){
//    if (event == E_START){
//        RedOn();
//        current_state = STATE_RUN;
//    }
//}
//
//void fn_RUN(){
//    if(event ==E_STOP){
//        BlueOn();
//        current_state = STATE_IDLE;
//    }
//}
//
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




    int a = open_can("file.txt");

    cout << "Codice apertura file: " << a << endl;

    char message[10];

    for (int i = 0; i < 10; ++i) {
        can_receive(message);
        cout << message << endl;
        std::string str(message);
        cout << str << endl;
        parse(str);
    }

    close_can();
    cout << "End of Project 2" << endl;

    return 0;
}