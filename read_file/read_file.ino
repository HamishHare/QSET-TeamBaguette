/*
  SD card read/write
  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10 (for MKRZero SD: SDCARD_SS_PIN)
  This example code is in the public domain.
*/

#include <SPI.h>
#include <SD.h>

File logFile;
File dataFile;

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

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  logFile = SD.open("logs.txt", FILE_WRITE);

  // if the file opened properly, write to it:
  // if (myFile) {
  //   Serial.print("Writing to logs.txt...");
  //   myFile.println("testing 1, 2, 3.");
  //   // close the file:
  //   myFile.close();
  //   Serial.println("done.");
  // } else {
  //   // if the file didn't open, print an error:
  //   Serial.println("error opening test.txt");
  // }

  // Open the logs file for reading:
  logFile = SD.open("logs.txt");
  if (logFile) {
    Serial.println("=================");
    Serial.println("READING logs.txt:");
    Serial.println("=================");

    // read from the file until there's nothing else in it:
    while (logFile.available()) {
      Serial.write(logFile.read());
    }
    // close the file:
    logFile.close();

    Serial.println("======================");
    Serial.println("Done reading logs.txt:");
    Serial.println("======================");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening logs.txt");
  }

  // Open the data file for reading:
  dataFile = SD.open("flightData.txt");
  if (dataFile) {
    Serial.println("=======================");
    Serial.println("READING flightData.txt:");
    Serial.println("=======================");

    // read from the file until there's nothing else in it:
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    // close the file:
    dataFile.close();

    Serial.println("============================");
    Serial.println("Done reading flightData.txt:");
    Serial.println("============================");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening flightData.txt");
  }
}

void loop() {
  // nothing happens after setup
}