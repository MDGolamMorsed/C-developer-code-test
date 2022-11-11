/*
Author: MD Golam Morsed
Mail: golammorsed6@gmail.com
background: i have completed Task 1 from "C++ Developer code test.doc"
Task list: i have made separate file for different task and just integrate in this file and create thread.
1. JSON file load and create task base on json file parameter
2. check json file is modified or not.
3. if json file is modified or WDT is reset then task will be terminated and again update json file will be loaded
*/

#include <iostream> //for input/output on to screen
#include <thread>   //to handle thread/process

/************ custom header file **************/
#include "checkFileModification.cpp"  //to check file modification
#include "taskCreateFromJsonFile.cpp" //load json file and create task
#include "logFileHandling.cpp"        //to set log info into a file
#include "wdtHandler.cpp"             //to handle WDT

using namespace std;

int main()
{
    writeFile("LOG File for Debug V1.0.0\n\n"); // write into a log file
    writeFile("json file is loaded successfully\n");

    thread th1(jsonFileLoad);
    thread th2(checkFileModification);
    Sleep(5000);
    thread th3(timer);

    // Wait for thread t1 to finish
    th1.join();

    // Wait for thread t2 to finish
    th2.join();

    // Wait for thread t2 to finish
    th3.join();
    return 0;
}
