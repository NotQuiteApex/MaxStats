#include <Arduino.h>

#include "serialhelp.h"

// Input string, we use it for storing all our data.
extern String inputString;

// A temp string we use to store data in, only for when we recieve data.
extern String tempInput;

// Specific hardware details, we only need them once.
extern String cpuName;
extern String gpuName;
extern String ramCount;

// Other details that we recieve multiple times over a connection.
extern String cpuFreq;
extern String cpuTemp;
extern String cpuLoad;
extern String ramUsed;
extern String gpuTemp;
extern String gpuCoreClock;
extern String gpuCoreLoad;
extern String gpuVramClock;
extern String gpuVramLoad;


void serial_receive()
{
  inputString = "";
  if (Serial.available() > 0)
  {
    while (Serial.available() > 0)
    {
      // Read in data to a string
      int readbyte = Serial.read();
      if (readbyte != -1)
        inputString += (char)readbyte;
    }
  }
}

bool serial_matches(char * matcher)
{
  serial_receive();

  return inputString.equals(matcher);
}

bool receive_once_data()
{
  // string format of computer parts stats
  // $"{cpuName}|{gpuName}|{ramTotal}GB|"

  // recieve data
  serial_receive();

  // if data isn't empty, process it
  if (!inputString.equals(""))
  {
    // process the names!
    tempInput = "";

    // beginning index and end index, and the variable to store data into
    unsigned int idx1 = 0;
    unsigned int idx2 = 0;
    byte count = 0;

    // Loop through the string, finding the pipe seperators
    for (unsigned int i = 0; i < inputString.length(); i++)
    {
      // if character matches a pipe, we process the data before
      if (inputString[i] == '|')
      {
        // Old start index, to end index
        idx1 = idx2;
        if (inputString[idx1] == '|')
          idx1++;
        idx2 = i;

        // slice string, grab the data
        tempInput = inputString.substring(idx1, idx2);

        // hand off data to proper variable
        if (count == 0)
          cpuName = tempInput;
        else if (count == 1)
          gpuName = tempInput;
        else if (count == 2)
          ramCount = tempInput;

        // update variable to determine what variable to hand off to next
        count++;
      }
    }

    return true; // we sucessfully processed data
  }

  return false; // we didn't have any data to process, we bounce
}

bool receive_cont_data()
{
  // Format string of continuous stats
  // $"{cpuFreq}|{cpuTemp}|{cpuLoad}|{ramUsed}|{gpuTemp}|" +
  // $"{gpuCoreClock}|{gpuCoreLoad}|{gpuVramClock}|{gpuVramLoad}|"

  // this is the same as above, just with more variables to hand data to

  serial_receive();
  if (!inputString.equals(""))
  {
    tempInput = "";

    unsigned int idx1 = 0;
    unsigned int idx2 = 0;
    byte count = 0;

    for (unsigned int i = 0; i < inputString.length(); i++)
    {
      if (inputString[i] == '|')
      {
        idx1 = idx2;
        if (inputString[idx1] == '|')
          idx1++;
        idx2 = i;

        tempInput = inputString.substring(idx1, idx2);

        if (count == 0)
          cpuFreq = tempInput;
        else if (count == 1)
          cpuTemp = tempInput;
        else if (count == 2)
          cpuLoad = tempInput;
        else if (count == 3)
          ramUsed = tempInput;
        else if (count == 4)
          gpuTemp = tempInput;
        else if (count == 5)
          gpuCoreClock = tempInput;
        else if (count == 6)
          gpuCoreLoad = tempInput;
        else if (count == 7)
          gpuVramClock = tempInput;
        else if (count == 8)
          gpuVramLoad = tempInput;

        count++;
      }
    }

    return true;
  }

  return false;
}