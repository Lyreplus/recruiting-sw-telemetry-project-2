//
// Created by lyreplus on 15/10/23.
//

#ifndef EAGLETRT_STATEMACHINE_H
#define EAGLETRT_STATEMACHINE_H

#include <stdio.h>

typedef unsigned char byte;

class EventData{
public:
    virtual ~EventData() {}
};


class StateMachine
{
public:

    StateMachine(byte maxStates, byte initialState = 0);

    virtual ~StateMachine() {}

    byte GetCurrentState() { return m_currentstate;}
private:
    const byte MAX_STATES;
    byte m_current_state;
    byte m_new_state;
    int m_event_generated;
    const EventData* m_pEventData;
};



#endif //EAGLETRT_STATEMACHINE_H
