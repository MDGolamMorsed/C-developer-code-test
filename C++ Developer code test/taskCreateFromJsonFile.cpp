#include <iostream>
#include <fstream>
#include "dist/json/json.h"
#include "dist/jsoncpp.cpp"
#include <string>
#include <pthread.h>
#include <windows.h>

#include <tlhelp32.h>
#include <vector>

#include "taskCreateFromJsonFile.h"
#include "checkFileModification.h"
#include "logFileHandling.h"
#include "wdtHandler.h"

using namespace std;
using std::wstring;

uint8_t taskKillCount = 0, totalTask;
bool blockTaskFlag;

void *taskCreate(void *id);
void killProcessByName(const char *filename);
bool checkFileStatus(const char *fileName);

void jsonFileLoad()
{
    while (1)
    {
        // if ((fileModifiedFlag || wdtResetFlag) && (totalTask == taskKillCount))
        if (1)
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
                string taskName = characters[i]["task"].asString();
                cout << "task: " << taskName << endl;
                // blockTaskFlag = characters[i]["blockTask"].asUInt();
                // cout << "blockTask: " << blockTaskFlag << endl;

                bool result = checkFileStatus(taskName.c_str());
                cout << result;
                if (!result)
                {
                    system(taskName.c_str());
                    writeFile("Task is killed successfully\n");
                }

                cout << endl;

                // int ret = pthread_create(&my_thread[i], NULL, &taskCreate, (void *)i);
                // if (ret != 0)
                // {
                //     printf("Error: pthread_create() failed\n");
                //     exit(EXIT_FAILURE);
                // }

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

void killProcessByName(const char *filename)
{
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);
    while (hRes)
    {
        if (strcmp(pEntry.szExeFile, filename) == 0)
        {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
            if (hProcess != NULL)
            {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
        hRes = Process32Next(hSnapShot, &pEntry);
    }
    CloseHandle(hSnapShot);
}

bool checkFileStatus(const char *fileName)
{
    cout << fileName;
    std::vector<DWORD> pids;
    wstring wstr(fileName, fileName + strlen(fileName));
    std::wstring targetProcessName = wstr;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // all processes

    PROCESSENTRY32W entry; // current process
    entry.dwSize = sizeof entry;

    if (!Process32FirstW(snap, &entry)) // start with the first in snapshot
    {
        cout << "not found\n";
        return 0;
    }

    do
    {
        if (std::wstring(entry.szExeFile) == targetProcessName)
        {
            pids.emplace_back(entry.th32ProcessID); // name matches; add to list
        }
    } while (Process32NextW(snap, &entry)); // keep going until end of snapshot

    for (int i(0); i < pids.size(); ++i)
    {
        cout << "PID is: ";
        std::cout << pids[i] << std::endl;
        return 1;
    }
}