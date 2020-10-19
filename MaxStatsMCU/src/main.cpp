/***************************************\
 * MaxStats - NotQuiteApex             *
 * Showing PC stats from a tiny screen *
\***************************************/

#include <Arduino.h>

// Nothing yet!
#include <Adafruit_ST7735.h>
//#include <Adafruit_ST7789.h>
//#include <Adafruit_ST77xx.h>

// Stages of serial communication
enum SerialStage {
  Handshake,      // The initial stage, where the computer and MCU greet
  ComputerParts,  // The computer sends some constant data over the wire
  ContinuousStats // The computer sends data that gets updated over time
} commstage;

// The parts of comm stages (stage recieve and response)
int commstagepart;

void setup() {
  // Set up serial communication for the board
  #if defined (TEENSYDUINO)
  Serial.begin(115200);
  #else
  Serial.begin(115200, SERIAL_8E2);
  #endif

  // Initialize the communications stage
  commstage = SerialStage::Handshake;
  commstagepart = 0;
}

void loop() {
  if (commstage == SerialStage::Handshake)
  {
    
  }
  else if (commstage == SerialStage::ComputerParts)
  {

  }
  else if (commstage == SerialStage::ContinuousStats)
  {

  }

  if (Serial.available() > 0) {
    String data = "";
    do {
      // Read in data to a string
      int readbyte = Serial.read();
      if (readbyte != -1)
        data += (char)readbyte;
    } while (Serial.available() > 0);
    Serial.print("Pong! Look what I got: \"");
    Serial.print(data);
    Serial.println("\"");
  }
}
