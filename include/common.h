// Common global variables for the project
#ifndef RECRUITING_SW_TELEMETRY_PROJECT_2_COMMON_H
#define RECRUITING_SW_TELEMETRY_PROJECT_2_COMMON_H

#include "types.h"
#include <mutex>
#include <string>

extern Event event;
extern State_t current_state;
extern std::mutex LockMutex;
extern std::string id;
extern int numFile;
extern int endfile;

#endif //RECRUITING_SW_TELEMETRY_PROJECT_2_COMMON_H
