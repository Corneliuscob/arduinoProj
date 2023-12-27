// Basic Bluetooth sketch HC-06_01
// Connect the Hc-06 module and communicate using the serial monitor
//
// The HC-06 defaults to AT mode when first powered on.
// The default baud rate is 9600
// The Hc-06 requires all AT commands to be in uppercase. NL+CR should not be added to the command string
//
 
 
#include <SoftwareSerial.h>
#include "serialHelp.h"
#include "parseActions.h"
SoftwareSerial BTserial(2, 3); // RX | TX
// Connect the HC-06 TX to the Arduino RX on pin 2. 
// Connect the HC-06 RX to the Arduino TX on pin 3 through a voltage divider.
// 

 
 
void setup() 
{
    Serial.begin(9600);
    Serial.println("Enter AT commands:");
    displaySerialCommands();

    // HC-06 default serial speed is 9600
    BTserial.begin(9600);  

    BTserial.println("Hello Bastard!");
    delay(1000);
}
 
void loop()
{
    String receivedString = "blank";
    // Keep reading from HC-06 and send to Arduino Serial Monitor
    if (BTserial.available()>0)
    {  
        int numChars = BTserial.available();
        // Call the function to convert the available characters to a string
        receivedString = readSerialString(numChars);
        // Process the received string
        processReceivedString(receivedString);
        displayAction(receivedString);
    }
         
        // BTserial.write("processed");
    // Keep reading from Arduino Serial Monitor and send to HC-06
    if (Serial.available())
    {
        BTserial.write(Serial.read());
    }
  delay(60);
}


String readSerialString(int numChars) {
  String result = ""; // Initialize an empty String

  // Read each character from the serial buffer and append it to the result string
  for (int i = 0; i < numChars; ++i) {
    char incomingByte = BTserial.read();
    result += incomingByte;
  }

  return result;
}

void processReceivedString(String str) {
  // Process the received string as needed

  Serial.println(str);
}