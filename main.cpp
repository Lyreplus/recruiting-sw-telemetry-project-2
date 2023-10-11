#include <stdio.h>
#include <iostream>
#include <thread>
using namespace std;

extern "C"{
    #include "fake_receiver.h"
}

static bool s_Finished = false;

void DoWork(){
    while(!s_Finished){
        std::cout << "Great Scott its working!" << std::endl;

    }
};

int main(void){
    cout << "Welcome to Project 2" << endl;

    //playing with threads
    //starting worker thread
    std::thread worker(DoWork);

    //enter will set s_Finished to true causing thread to stop
    std::cin.get();
    s_Finished = true;

    //wait the thread to finish its execution
    worker.join();


    cout << "End of Project 2" << endl;

    return 0;
}