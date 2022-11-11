#ifndef __WDT_HANDLER__
#define __WDT_HANDLER__

#include <iostream>
#include <ctime>
#include <unistd.h>
#include <Windows.h>

using namespace std;

extern time_t current_time;
extern int seconds;
extern bool wdtResetFlag;
void timer();

#endif
