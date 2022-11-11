#ifndef __CHECK_FILE_MODIFICATION_H__
#define __CHECK_FILE_MODIFICATION_H__

#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>

extern int x;
extern bool fileModifiedFlag;

bool checkFileModification();

#endif // __CHECK_FILE_MODIFICATION_H__
