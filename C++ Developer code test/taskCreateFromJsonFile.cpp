#include <iostream>
#include <fstream>
#include "dist/json/json.h"
#include "dist/jsoncpp.cpp"
#include <string>
#include <pthread.h>
#include <windows.h>

#include "taskCreateFromJsonFile.h"
#include "checkFileModification.h"
#include "logFileHandling.h"
#include "wdtHandler.h"

using namespace std;

uint8_t taskKillCount = 0, totalTask;
bool blockTaskFlag;

void *taskCreate(void *id);

void jsonFileLoad()
{
    while (1)
    {
        if ((fileModifiedFlag || wdtResetFlag) && (totalTask == taskKillCount))
        {
            cout << "############################# json file load ########################\n";
            fileModifiedFlag = 0;
            taskKillCount = 0;
            wdtResetFlag = 0;
            ifstream ifs("config.json");
            Json::Reader reader;
            Json::Value obj;
            reader.parse(ifs, obj); // reader can also read strings
            seconds = obj["WDT"].asUInt();
            cout << "WDT is: " << seconds << endl;
            blockTaskFlag = obj["blockTask"].asUInt();
            cout << "blockTask: " << blockTaskFlag << endl;
            const Json::Value &characters = obj["taskList"]; // array of characters
            cout << "total created task is " << characters.size() << endl;
            totalTask = characters.size();

            pthread_t my_thread[totalTask];

            for (int i = 0; i < totalTask; i++)
            {
                cout << "Task " << i << " parameter" << endl;
                cout << "task: " << characters[i]["task"].asString() << endl;
                // blockTaskFlag = characters[i]["blockTask"].asUInt();
                // cout << "blockTask: " << blockTaskFlag << endl;

                cout << endl;

                int ret = pthread_create(&my_thread[i], NULL, &taskCreate, (void *)i);
                if (ret != 0)
                {
                    printf("Error: pthread_create() failed\n");
                    exit(EXIT_FAILURE);
                }

                writeFile("Task create successfully\n");
            }

            // pthread_join(my_thread[1], NULL);
            // pthread_join(my_thread[2], NULL);
            // pthread_join(my_thread[3], NULL);
            // pthread_join(my_thread[4], NULL);
            // pthread_exit(NULL);
        }
        else
        {
            // cout << "################################## no file load ########################\n";
            Sleep(2000);
        }
    }
}

void *taskCreate(void *id)
{
    while (1)
    {
        cout << "task ID is: " << (id) << endl;
        Sleep(500); // in ms
        if (!blockTaskFlag)
        {
            current_time = time(0);
        }

        if (fileModifiedFlag || wdtResetFlag)
        {
            // cout << strcat("Task kill successfully", str(taskKillCount));
            writeFile("Task is killed successfully\n");
            cout << "task is killed:" << endl;

            taskKillCount++;
            cout << taskKillCount;
            break;
        }
    }
}
