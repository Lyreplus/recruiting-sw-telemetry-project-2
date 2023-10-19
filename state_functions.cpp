//
// Created by lyreplus on 19/10/23.
//
#include <iostream>
#include "state_functions.h"
#include "common.h"
#include "thread_functions.h"
#include <thread>
#include "check_statuses.h"

using namespace std;

/*declarations of the functions that correspond to the states*/
void fn_INIT(){
    if (event == E_NONE){
        current_state = STATE_IDLE;
    }
}

void fn_IDLE(){
    idleThread();
    checkS1(event);
    if(event != E_ENDFILE){
        current_state = STATE_RUN;
    }else{
        current_state = STATE_ENDFILE;
    }
}

void fn_RUN(){
    std::thread thread(runThread);
    checkS2(event);
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