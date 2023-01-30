/*
  Our program designed to run both sensors at the same time
*/

#include <Wire.h>
#include "Adafruit_SI1145.h"
#include <SFE_BMP180.h>
// SD Card
#include <SPI.h>
#include <SD.h>

Adafruit_SI1145 uv = Adafruit_SI1145();
SFE_BMP180 pressure;

#define ALTITUDE 100.0 // Approx altitude of Queen's ============== LOOK INTO WHERE THIS IS USED AND IF NEEDED, AS ALTITUDE WILL CHANGE IN FLIGHT
File dataFile;
File logFile;

bool serialOutput = false;

void setup() {
  if (serialOutput){ // For interfacing with Serial monitor
    Serial.begin(9600);
    Serial.println("REBOOT");    
    // Open serial communications and wait for port to open:
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
  
    // INITIALISATION SECTION
    //=======================
    // Initialise the SD Card (See https://www.arduino.cc/reference/en/libraries/sd/)
    // SD Card
    Serial.print("Initializing SD card...");

    if (!SD.begin(10)) {
      Serial.println("SD card initialization failed!");
      while (1);
    }
    Serial.println("SD card initialization done.");

    // Initialise the Adafruit Vis/IR/UV sensor
    Serial.println("Adafruit SI1145 test");
  
    if (! uv.begin()) {
      Serial.println("Didn't find Si1145");
      while (1);
    }

    Serial.println("Adafruit OK!");

    // Initialize the pressure sensor (it is important to get calibration values stored on the device).
    if (pressure.begin())
      Serial.println("BMP180 init success");
    else
    {
      // Oops, something went wrong, this is usually a connection problem,
      // see the comments at the top of this sketch for the proper connections.

      Serial.println("BMP180 init fail\n\n");
      while(1); // Pause forever.
    }
  }  
  else{ // for when running without serial monitor  
    // INITIALISATION SECTION
    //=======================
    // Initialise the SD Card (See https://www.arduino.cc/reference/en/libraries/sd/)
    // (Don't run the rest of the code if this fails)
    Serial.begin(9600);
    if (!SD.begin(10)) { // uses the default SS hardware pin of 10
      while (1);
    }
    // Save to a log file that the SD card was succesfully initialised
    logFile = SD.open("logs.txt", FILE_WRITE);
    logFile.println("SD card successfully initialised at [TIME]"); // =========== ADD TIME IDENTIFIER
    //Serial.println("Log file opened");
    
    // Initialise the Adafruit Vis/IR/UV sensor
    if (uv.begin()) {
      // Save to a log file that the light sensor was succesfully initialised
      logFile.println("Light sensor successfully initialised at [TIME]"); // =========== ADD TIME IDENTIFIER
      //Serial.println("Light sensor started");
    }
    else{
      // Save to a log file that the light sensor failed to initialise
      logFile.println("Light sensor failed to initialise at [TIME]"); // =========== ADD TIME IDENTIFIER
      //Serial.println("Light sensor failed");
      logFile.close();
      while (1); // pause forever
    }

    // Initialize the pressure sensor (it is important to get calibration values stored on the device).      
    if (pressure.begin()){
      // Save to a log file that the pressure sensor was succesfully initialised
      logFile.println("Pressure sensor successfully initialised at [TIME]"); // =========== ADD TIME IDENTIFIER
      //Serial.println("Pressure sensor started");
    } else
    {
      // Save to a log file that the pressure sensor failed to initialise
      logFile.println("Pressure sensor failed to initialise at [TIME]"); // =========== ADD TIME IDENTIFIER
      //Serial.println("Pressure sensor failed");
      logFile.close(); 
      while(1); // Pause forever.
    }  

    // close the log file
    logFile.close();
    //Serial.println("Log file closed");
    // OPEN THE DATA FILE
    // note that only one file can be open at a time
    dataFile = SD.open("flightData.txt", FILE_WRITE);
  }
}

void loop() {

  // Adafruit sensor:
  
  Serial.println("ADAFRUIT VIS/IR/UV:");
  Serial.println("===================");
  Serial.print("Vis: "); Serial.println(uv.readVisible());
  Serial.print("IR: "); Serial.println(uv.readIR());
  
  // Uncomment if you have an IR LED attached to LED pin!
  //Serial.print("Prox: "); Serial.println(uv.readProx());

  float UVindex = uv.readUV();
  // the index is multiplied by 100 so to get the
  // integer index, divide by 100!
  UVindex /= 100.0;  
  Serial.print("UV: ");  Serial.println(UVindex);

  delay(1000);

  // Pressure sensor:

  Serial.println("Pressure sensor");
  char status;
  double T,P,p0,a;

  // Loop here getting pressure readings every 10 seconds.

  // If you want sea-level-compensated pressure, as used in weather reports,
  // you will need to know the altitude at which your measurements are taken.
  // We're using a constant called ALTITUDE in this sketch:
  
  Serial.println();
  Serial.print("provided altitude: ");
  Serial.print(ALTITUDE,0);
  Serial.print(" meters, ");
  Serial.print(ALTITUDE*3.28084,0);
  Serial.println(" feet");
  
  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println(" deg F");
      
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = pressure.altitude(P,p0);
          Serial.print("computed altitude: ");
          Serial.print(a,0);
          Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  delay(5000);  // Pause for 5 seconds.
}
