
#include "wdtHandler.h"
#include "logFileHandling.h"

time_t current_time = time(0);
int hours = 0;
int minutes = 0;
int seconds = 0;
bool wdtResetFlag = 0;

void timer()
{
    while (1)
    {
        writeFile("WDT init\n");
        cout << "WDT init" << endl;
        int total_time = hours * 60 * 60 + minutes * 60 + seconds;
        current_time = time(0);
        char *b = ctime(&current_time);

        tm *gmtm = gmtime(&current_time);

        b = asctime(gmtm);

        int i = 0;
        while (time(0) - current_time < total_time)
        {
            // do nothing.
            //  cout << "timer" << i << endl;
            //  i++;
            //  sleep(1);
        }
        wdtResetFlag = 1;
        cout << "WDT RESET!!\n";
        writeFile("WDT RESET\n");
        Sleep(2000);
    }
}