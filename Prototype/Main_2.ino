
/*
Program to read sensors, log the data with time stamp on an SD Card, display the data on a 7"touchscreen and send the datastring to IoT gateway
Sensors added-
1. Conductivity Sensors - 3
2. pH Sensor - 1
3. Level Sensors -10

Written by Niteesh S Shanbog
v1 - 28/7/2021
v2 - 29/7/2021 - Sorted variable mixing up in the Display
v3 - 29/7/2021 - Updated version of Main_1.ino by remvoing all Serial.println() except the datastring to be transferred to IoT Gateway
*/


//NEXTION Display using Serial 
#include "Nextion.h"

// Declare your Nextion objects - Example (page id = 0, component id = 1, component name = "b0") 

  //Conductivity Display
  NexNumber c0 = NexNumber(1, 13, "n0");
  NexNumber c1 = NexNumber(1, 14, "n2");
  NexNumber c2 = NexNumber(1, 15, "n7");

  //pH Display
  NexNumber pH1 = NexNumber(1, 17, "n1");

  //Level Display
  NexNumber l1 = NexNumber(3, 3, "n3");
  NexNumber l2 = NexNumber(3, 5, "n0");
  NexNumber l3 = NexNumber(3, 7, "n1");

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
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

int hT01 = 0;
int hT06 = 0;
int hT03 = 0;
int hT07 = 0;
int hT05 = 0;
int hT12 = 0;
int lT01 = 0;
int lT07 = 0;
int lT03 = 0;
int lT12 = 0;

float CT001 = 0;
float CT002 = 0;
float CT003 = 0;

float pH001 = 0;

int PI001 = 0;
int PI003 = 0;
int PT001 = 0;
int PT004 = 0;
int PT005 = 0;
int ZL001 = 0;
int ZL002 = 0;
             
int high_cond = 0;
int COD = 0;
int BOD = 0;

int M001 = 0;
int M002 = 0;
int M003 = 0;
int M005 = 0;

float buffer_arr[10];
float conductivity = 0;
float pH = 0;


void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  //Serial.print("Initializing SD card...");

  if (!SD.begin()) 
  {
    //Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  //Setup for RTC
  Serial.begin(9600);
  if (! rtc.begin()) 
  {
    //Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) 
  {
    //Serial.println("RTC lost power, lets set the time!");
  
    //Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
    // rtc.adjust(DateTime(2021, 3, 17, 11, 33, 0));
  }
  
  nexInit();
}

void loop()
{
  hT01 = level_check(1,LAH001);
  hT06 = level_check(1,LAH002);
  hT03 = level_check(1,LAH003);
  hT07 = level_check(1,LAH004);
  hT05 = level_check(1,LAH005);
  hT12 = level_check(1,LAH006);
  lT01 = level_check(0,LAL001);
  lT07 = level_check(0,LAL002);
  lT03 = level_check(0,LAL004);
  lT12 = level_check(0,LAL005);

  CT001 = measure_conductivity(1,A3);
  CT002 = measure_conductivity(2,A4);
  CT003 = measure_conductivity(3,A5);

  pH001 = measure_pH();
  
  write_SD();

  delay (2000);

  display_values();
}


int level_check(int sensorType, int FloatSensor)
{
  int level = 0;
  int buttonState = digitalRead(FloatSensor);
  if (sensorType == 1)
  {
    if (buttonState == LOW) 
    { 
          level = 0;
          
    } 
    else 
    { 
          int level = 1;
          //Serial.println( "WATER LEVEL - HIGH" ); 
    }
  }
  else
  {
    if (buttonState == LOW) 
      { 
          int level = 1;
      } 
    else 
      { 
          int level = 0;
      }
  }
  return level;
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
        //Range: 0-2mS/cm
        conductivity = (float)avgval/6*1999/1024;
        break;
      }
      case 2:
      {
        //Range: 0-10mS/cm
        conductivity = (float)avgval/6*9999/1024;
        Serial.println(conductivity);
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
  
    dataString += String(CT001);
    dataString += ",";
    dataString += String(CT002);
    dataString += ",";
    dataString += String(CT003);
    dataString += ",";
  
    dataString += String(PI001);
    dataString += ",";
    dataString += String(PI002);
    dataString += ",";
    dataString += String(PT003);
    dataString += ",";
    dataString += String(PT004);
    dataString += ",";
    dataString += String(PT005);
    dataString += ",";
    dataString += String(ZL001);
    dataString += ",";
    dataString += String(ZL002);
    dataString += ",";
  
    dataString += String(pH001);
    dataString += ",";  
  
    dataString += String(high_cond);
    dataString += ",";
    dataString += String(cod);
    dataString += ",";
    dataString += String(bod);
    dataString += ",";
 
  

    dataString += String(hT01);
    dataString += ",";
    dataString += String(hT06);
    dataString += ",";
    dataString += String(hT03);
    dataString += ",";
    dataString += String(hT07);
    dataString += ",";
    dataString += String(hT05);
    dataString += ",";
    dataString += String(hT12);
    dataString += ",";
    dataString += String(lT01);
    dataString += ",";
    dataString += String(lT07);
    dataString += ",";
    dataString += String(lT03);
    dataString += ",";
    dataString += String(lT12);
    dataString += ",";
  
    dataString += String(M001);
    dataString += ",";
    dataString += String(M002);
    dataString += ",";
    dataString += String(M003);
    dataString += ",";
    dataString += String(M005);
    dataString += ",";
  


    Serial.println(dataString);

    char fileName[14];
    sprintf(fileName, "%4d%02d%02d.CSV", year, month, day);   //Name the file with today's date
  
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    myFile = SD.open(fileName, FILE_WRITE);

    // if the file opened okay, write to it:
    if (myFile) 
    {
      //Serial.print("Writing to test.txt...");
      myFile.println(dataString);
      // close the file:
      myFile.close();
      //Serial.println("done.");
    } 
  else 
  {
      // if the file didn't open, print an error:
      //Serial.println("error opening test.txt");
  }
}

void display_values()
{
  pH1.setValue(pH001);
  c0.setValue(CT001);
  c1.setValue(CT002);
  c2.setValue(CT003);
  l1.setValue(hT01);
  l2.setValue(hT06);
  l3.setValue(hT03);
}

