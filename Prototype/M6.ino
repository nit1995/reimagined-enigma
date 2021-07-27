
/* Program to read Conductivity Sensor
 *  
 *  Written by Niteesh S Shanbog
 *  v1 - 26/7/2021 - Read Conductivity Sensor, Create datastring with timestamp
 *  v2 - 27/7/2021 - Corrected measure_conductivity() function by adding analog pin input 
 */





// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

#include <SPI.h>
#include <SD.h>

File myFile;


float CT001 = 0;
float CT002 = 0;
float CT003 = 0;
float buffer_arr[10];
float conductivity = 0;

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
  
  CT001 = measure_conductivity(1,A1);
  CT002 = measure_conductivity(2,A2);
  CT003 = measure_conductivity(3,A3);

  write_SD();

  delay (2000);

}

int measure_conductivity(int condType, int an_pin)
{
  int val = 0;
  for (int i=0; i<10; i++)
  {
    buffer_arr[i] = analogRead(an_pin);
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
  switch (condType)
    {
      case 1:
      {
        //Range: 0-1mS/cm
        conductivity = (float)avgval/6*999/1024;
        break;
      }
      case 2:
      {
        //Range: 0-10mS/cm
        conductivity = (float)avgval/6*1999/1024;
        break;
      }
      case 3:
      {
        //Range: 0-50mS/cm
        conductivity = (float)avgval/6*49999/1024;
        break;
      }
      default:
        conductivity = 0;
    }
    return conductivity;
      
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

  dataString += String(CT001);
  dataString += String(CT002);
  dataString += String(CT003);
  
  Serial.println(dataString);
}

    
