//This file contains the types used the most in the project
#ifndef RECRUITING_SW_TELEMETRY_PROJECT_2_TYPES_H
#define RECRUITING_SW_TELEMETRY_PROJECT_2_TYPES_H

/*enum with the various events*/
typedef enum {
    E_NONE,
    E_START,
    E_STOP,
    E_ENDFILE
} Event;

/*enum with the various states*/
typedef enum {
    STATE_INIT,
    STATE_IDLE,
    STATE_RUN,
    STATE_ENDFILE,
    NUM_STATES
}State_t;

#endif //RECRUITING_SW_TELEMETRY_PROJECT_2_TYPES_H
