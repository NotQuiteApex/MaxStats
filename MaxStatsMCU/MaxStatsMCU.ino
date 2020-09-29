/***************************************\
 * MaxStats - NotQuiteApex             *
 * Showing PC stats from a tiny screen *
\***************************************/

// Nothing yet!

void setup() {
  Serial.begin(9600);
}

void loop() {
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
