#include <iostream>
#include "../include/common.h"
#include "../fake_receiver.h"
#include <mutex>
#include <map>
#include <chrono>
#include <iomanip>
#include <fstream>
#include "../include/data_processing.h"

using namespace std;

//thread function for the idle state, executed in the main thread
void idleThread(){
    cout << "Entering idleThread" << endl;
    char message[20];
    while(event != E_START && event != E_ENDFILE){
        //mutex for the mutual access to the can_receive() function
        unique_lock<mutex> idleLock(LockMutex);
        if(can_receive(message) != -1){
            idleLock.unlock();

            //parse the message
            std::string str(message);
            parse(str, id, event);
        }else{
            idleLock.unlock();
            //mutex for the mutual access to the event variable
            unique_lock<mutex> eventLock(LockMutex);
            event = E_ENDFILE;
            eventLock.unlock();
        }
    }
    cout << "Exiting idleThread" << endl;
}

//thread function for the run state, executed in an external thread
void runThread(){

    cout << "Entering runThread; " << endl;

    //create the rows hashmap to store the statistics, each row is a different ID
    map<string, tuple<uint, double, long>> rows;
    char message[20];

    //create filename with datetime
    time_t time = std::time(nullptr);
    auto tm = *std::localtime(&time);

    //create a formatted string with the current date
    std::ostringstream outputFormattedTimeStream;
    outputFormattedTimeStream << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    string str_format = outputFormattedTimeStream.str();

    //give the filename with file_date_time
    string filename = "file" + str_format + ".txt";
    ofstream logTextFile(filename, ios_base::out);

    //exit from the while when there is a STOP signal or an ENDFILE signal
    while (event != E_STOP && event != E_ENDFILE) {
        //give mutual access to the can_receive function
        unique_lock<mutex> can_protection(LockMutex);
        if(can_receive(message) != -1){
            can_protection.unlock();

            //chrono to calculate the mean time of the message
            long startChrono = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

            std::string str(message);
            parse(str, id, event);

            //check if the id is already in the rows hashmap
            if(!(rows.find(id) == rows.end())){

                //stop the chrono and calculate the mean time
                long endChrono = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
                uint amount = get<0>(rows[id]);
                long oldChrono = get<2>(rows[id]);
                long deltaTime = endChrono - oldChrono;
                rows[id] = tuple<uint, double, long>(amount+1, (get<1>(rows[id])*amount + deltaTime)/(amount+1), endChrono);
            }else{

                //otherwise add the id to the hashmap, with the deltaTime
                long endChrono = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
                long deltaTime = endChrono - startChrono;
                rows[id] = tuple<uint, double, long>(1, deltaTime, endChrono);
            }
            log(str, logTextFile);
        } else {
            //give mutual access to the event variable
            can_protection.unlock();
            unique_lock<mutex> runLock(LockMutex);
            event = E_ENDFILE;
            runLock.unlock();
        }
    }

    //create a csv file with the statistics
    ofstream csvFile(to_string(numFile) + " statistics.csv");
    csvFile << "ID,number_of_messages,mean_time " << endl;
    for(auto& row : rows){
        csvFile << row.first << ", " << get<0>(row.second) << ", " << get<1>(row.second) << ", " << endl;
    }

    csvFile.close();
    numFile++;
    logTextFile.close();
    cout << "Exiting runThread; " << endl;
}
