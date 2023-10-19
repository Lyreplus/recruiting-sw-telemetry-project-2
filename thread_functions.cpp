//
// Created by lyreplus on 19/10/23.
//

#include <iostream>
#include "common.h"
#include "fake_receiver.h"
#include <mutex>
#include <map>
#include <chrono>
#include <iomanip>
#include <fstream>
#include "data_processing.h"

using namespace std;

void idleThread(){
    cout << "Entering idleThread" << endl;
    char message[20];
    while(event != E_START && event != E_ENDFILE){
        unique_lock<mutex> idleLock(LogMutex);
        if(can_receive(message) != -1){
            idleLock.unlock();
            std::string str(message);
            parse(str, id, event);
        }else{
            idleLock.unlock();
            unique_lock<mutex> eventLock(LogMutex);
            event = E_ENDFILE;
            eventLock.unlock();
        }
    }


}

void runThread(){

    cout << "Entering runThread; " << endl;
    map<string, tuple<uint, double, long>> rows;

    char message[20];

    //create filename with datetime
    time_t t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    string str_format = oss.str();

    string filename = "file" + str_format + ".txt";
    ofstream MyFile(filename, ios_base::out);
    while (event != E_STOP && event != E_ENDFILE) {
        unique_lock<mutex> can_protection(LogMutex);
        if(can_receive(message) != -1){
            can_protection.unlock();
            long start = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

            std::string str(message);
            parse(str, id, event);
            if(!(rows.find(id) == rows.end())){
                long end = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
                uint amount = get<0>(rows[id]);
                long alpha = get<2>(rows[id]);
                long beta = end - alpha;
                rows[id] = tuple<uint, double, long>(amount+1, (get<1>(rows[id])*amount + beta)/(amount+1), end);
            }else{
                long end = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
                long delta = end - start;
                rows[id] = tuple<uint, double, long>(1, delta, end);
            }
            log(str, MyFile);
        } else {
            can_protection.unlock();
            unique_lock<mutex> runLock(LogMutex);
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
    MyFile.close();
}
