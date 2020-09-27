/***************************************\
 * MaxStats - NotQuiteApex             *
 * Showing PC stats from a tiny screen *
\***************************************/

// Nothing yet!

void setup() {
  Serial.begin(9600, SERIAL_8E2);
}

void loop() {
  if (Serial.available() > 0) {
    String data = "";
    do {
      // Read in data to a string
      data += Serial.read()
    } while (Serial.available() > 0);
    Serial.print("Pong! Look what I got: \"");
    Serial.print(data);
    Serial.println("\"");
  }
}
