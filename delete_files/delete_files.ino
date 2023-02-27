/*
  SD card delete files
*/

#include <SPI.h>
#include <SD.h>

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // DELETE FILES
  // Check to see if the logs file exists:
  if (SD.exists("logs.txt")) {
    Serial.println("logs.txt exists.");
    
    // delete the file:
    Serial.println("Removing logs.txt...");
    SD.remove("logs.txt");

  } else {
    Serial.println("logs.txt doesn't exist.");
  }

  // Check to see if the flightData file exists:
  if (SD.exists("flightData.txt")) {
    Serial.println("flightData.txt exists.");
    
    // delete the file:
    Serial.println("Removing flightData.txt...");
    SD.remove("logs.txt");

  } else {
    Serial.println("flightData.txt doesn't exist.");
  }
}

void loop() {
  // nothing happens after setup
}