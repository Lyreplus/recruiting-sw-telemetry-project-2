#include <iostream>
#include "../include/state_functions.h"
#include "../include/common.h"
#include "../include/thread_functions.h"
#include <thread>
#include "../include/check_statuses.h"

using namespace std;

/*declarations of the functions that correspond to the states*/
void fn_INIT(){
    if (event == E_NONE){
        current_state = STATE_IDLE;
    }
}

// execute the idleThread function and loop if the event is STOP
// If the event is different from ENDFILE and RUN, change the state to RUN
void fn_IDLE(){
    idleThread();
    checkS1(event);
    if(event != E_ENDFILE){
        current_state = STATE_RUN;
    }else{
        current_state = STATE_ENDFILE;
    }
}

// execute the runThread function in another thread and loop if the event is START
// If the event is STOP and is different from ENDFILE, change the state to IDLE
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

// print the message "End Of File!" and change the value of the endfile variable to 1
void fn_ENDFILE(){
    cout << "End Of File!" << endl;
    endfile = 1;
}