
/*
Program to read a pH Meter
Written by Niteesh S Shanbog
v1 - 26/7/2021 - Tested


Things to be done-
1. Recheck calibration - The display in the sensor is showing 7.41 and the serial monitor is showing 7.63. Proper calibration has to be done prior to installation. 


*/





// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

#include <SPI.h>
#include <SD.h>

File myFile;


float pH001 = 0;

float buffer_arr[10];
float pH = 0;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  //Setup for RTC
  Serial.begin(9600);
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, lets set the time!");
  
    // Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
    //rtc.adjust(DateTime(2021, 3, 17, 11, 33, 0));
  }
}



void loop()
{
  
  pH001 = measure_pH();

  write_SD();

  delay (2000);

}


float measure_pH()
{
  int val = 0;
  for (int i=0; i<10; i++)
  {
    buffer_arr[i] = analogRead(A2);
    delay(10);
  }
  for (int i=0; i<9; i++) //To sort the readings in ascending order
  {
    for (int j=i+1; j<10; j++)
    {
      if (buffer_arr[i]>buffer_arr[j])
      {
        float temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  
  float avgval = 0;
  for (int i=2; i<8; i++)   //Ignore two readings at the extreme ends
  {
    avgval += buffer_arr[i];
  }
  float voltage = (float)avgval*14/1024/6;
  float pH = voltage;
  return pH;
}


void write_SD()
{
  DateTime now = rtc.now(); 

  //Values for timestamp to be appended at the beginning of the datastring
  int year = now.year();
  int month = now.month();
  int day = now.day();
  int hour = now.hour();
  int minute = now.minute();
  int second = now.second();    


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

  dataString += String(pH001);
  
  Serial.println(dataString);
}

    
