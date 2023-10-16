//
// Created by lyreplus on 16/10/23.
//

#include "StateMachine.h"
typedef unsigned char byte;

StateMachine::StateMachine(byte maxStates, byte initialState) :
    MAX_STATES(maxStates),
    m_current_state(initialState),
    m_new_state(0),
    m_event_generated(0),
    m_pEventData(NULL),
