

#include "checkFileModification.h"
#include <iostream> 
#include <windows.h>
#include "logFileHandling.h"

using namespace std;

int x = 10;
struct tm *foo;
struct stat attrib;
bool firstFileSize = 0, fileModifiedFlag = 0;
int fileSize, tempFileSize;

bool checkFileModification()
{
    struct stat fileInfo;
    while (1)
    {
        // check file is exist or not
        if (stat("config.json", &fileInfo) != 0)
        {
            std::cerr << "Error: " << strerror(errno) << '\n';
            writeFile("JSON file error\n");
            return (EXIT_FAILURE);
        }

        tempFileSize = fileInfo.st_size;

        if (firstFileSize == 0)
        {
            fileSize = tempFileSize;
            cout << "fileSize is " << fileSize << endl;
            firstFileSize = 1;
            fileModifiedFlag = 1;
        }
        else
        {
            // cout << "fileSize: " << tempFileSize << endl;
            if (fileSize != tempFileSize)
            {
                writeFile("JSON file is modified\n");
                cout << "json file modified\n";
                firstFileSize = 0;
                fileModifiedFlag = 1;
            }

            else
            {
                Sleep(1000);
                // cout << "file not modified\n";
            }
        }
    }
}
