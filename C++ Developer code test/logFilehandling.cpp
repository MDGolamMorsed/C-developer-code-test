
/*
#include <fstream>
ofstream 	Creates and writes to files
ifstream 	Reads from files
fstream 	A combination of ofstream and ifstream: creates, reads, and writes to files

*/

#include "logFileHandling.h"

bool fileClear = 1;

void writeFile(char *payload)
{
  // Create a text file
  if (fileClear)
  {
    ofstream MyWriteFile("system.log");
    // Close the file
    MyWriteFile << payload;
    MyWriteFile.close();
    fileClear = 0;
  }
  else
  {
    ofstream MyWriteFile("system.log", ios_base::app);

    // Write to the file
    MyWriteFile << payload;

    // Close the file
    MyWriteFile.close();
  }
}
