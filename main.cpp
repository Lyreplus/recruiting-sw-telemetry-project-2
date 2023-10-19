#include <cstdio>
#include <iostream>
#include <thread>
#include <fstream>
#include "fake_receiver.h"
#include <string>
#include <mutex>
#include "types.h"
#include "check_statuses.h"
#include "state_functions.h"
#include "common.h"
#include "thread_functions.h"

using namespace std;

mutex LogMutex;
std::string id;
int endfile = 0;
int numFile = 0;

extern "C"{
    #include "fake_receiver.h"
}

/*state machine*/
typedef struct{
    State_t state;
    void (*state_function)();
} StateMachine_t;

/*initialization to the init state*/
State_t current_state = STATE_INIT;

/*initialization of the Finite State Machine*/
StateMachine_t fsm[] = {
        {STATE_INIT, fn_INIT},
        {STATE_IDLE, fn_IDLE},
        {STATE_RUN, fn_RUN},
        {STATE_ENDFILE, fn_ENDFILE}
};

/*initialization of the current event*/
Event event = E_NONE;

int main(){
    cout << "Welcome to Project 2" << endl;
    int a = open_can("candump.log");
    cout << "Can opening code: " << a << endl;
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