//
// Created by lyreplus on 19/10/23.
//

#include <iostream>
#include "check_statuses.h"
#include "types.h"

using namespace std;

void checkS1(Event &event){
    cout << "s1" << endl;
    while(event == E_STOP || event == E_NONE);
}

void checkS2(Event &event){
    cout << "s2" << endl;
    while(event == E_START);
}
