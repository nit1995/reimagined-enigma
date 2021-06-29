
/*
Program to read all 10 Magnetic Float Sensors to be installed in Lodhva and store the data in an SD Card with timestamp
Written by Niteesh S Shanbog
Date - 29/06/2021
*/



// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

#include <SPI.h>
#include <SD.h>

File myFile;

// change this to match your SD shield or module;
const int chipSelect = 53;


int LAH001 = 28;  //DI7 Float Sensor High Level of T01
int LAH002 = 22;  //DI1 Float Sensor High Level of T06
int LAH003 = 23;  //DI2 Float Sensor High Level of T03
int LAH004 = 24;  //DI3 Float Sensor High Level of T07
int LAH005 = 27;  //DI6 Float Sensor High Level of T05
int LAH006 = 29;  //DI8 Float Sensor High Level of T12

int LAL001 = 30;  //DI9 Float Sensor Low Level of T01
int LAL002 = 31;  //DI10 Float Sensor Low Level of T07
int LAL004 = 33;  //DI12 Float Sensor Low Level of T03
int LAL005 = 26;  //DI5 Float Sensor Low Level of T12



void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

//Setup for RTC
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
  
  // Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
     rtc.adjust(DateTime(2021, 3, 17, 11, 33, 0));
  }
}



void loop()
{
  DateTime now = rtc.now(); 

  //Values for timestamp to be appended at the beginning of the datastring
  int year = now.year();
  int month = now.month();
  int day = now.day();
  int hour = now.hour();
  int minute = now.minute();
  int second = now.second();  


  int hT01 = level_check(1,LAH001);
  int hT06 = level_check(1,LAH002);
  int hT03 = level_check(1,LAH003);
  int hT07 = level_check(1,LAH004);
  int hT05 = level_check(1,LAH005);
  int hT12 = level_check(1,LAH006);
  int lT01 = level_check(0,LAL001);
  int lT07 = level_check(0,LAL002);
  int lT03 = level_check(0,LAL004);
  int lT12 = level_check(0,LAL005);

  String dataString = "";
  dataString += String(year);
  dataString += "-";
  dataString += String(month);
  dataString += "-";
  dataString += String(day);
  dataString += " ";
  dataString += String(hour);
  dataString += ":";
  dataString += String(minute);
  dataString += ":";
  dataString += String(second);
  dataString += ",";

  dataString += String(hT01);
  dataString += ",";

  char fileName[14];
  sprintf(fileName, "%4d%02d%02d.CSV", year, month, day);   //Name the file with today's date
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(fileName, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) 
  {
      Serial.print("Writing to test.txt...");
      myFile.println(dataString);
      // close the file:
      myFile.close();
      Serial.println("done.");
  } 
  else 
  {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  delay (2000);

}


int level_check(int sensorType, int FloatSensor)
{
  int level = 0;
  int buttonState = digitalRead(FloatSensor);
  if (sensorType == 1)
  {
    if (buttonState == LOW) 
      { 
          int level = 0;
          Serial.println( "WATER LEVEL - LOW"); 
    } 
      else 
      { 
          int level = 1;
          Serial.println( "WATER LEVEL - HIGH" ); 
      }
  }
  else
  {
    if (buttonState == LOW) 
      { 
          int level = 1;
          Serial.println( "WATER LEVEL - LOW"); 
    } 
      else 
      { 
          int level = 0;
          Serial.println( "WATER LEVEL - HIGH" ); 
      }
  }
  return level;
}
    
