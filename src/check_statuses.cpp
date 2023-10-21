#include <iostream>
#include "../include/check_statuses.h"
#include "../include/types.h"

using namespace std;

//check if the event is STOP or NONE
void checkS1(Event &event){
    while(event == E_STOP || event == E_NONE);
}

//check if the event is START
void checkS2(Event &event){
    while(event == E_START);
}
