//NEXTION Display using Serial 
#include "Nextion.h"

// Declare your Nextion objects - Example (page id = 0, component id = 1, component name = "b0") 


NexNumber n3 = NexNumber(3, 3, "n3");
NexNumber n0 = NexNumber(1, 13, "n0");
NexNumber n1 = NexNumber(1, 17, "n1");

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"


RTC_DS3231 rtc;


int do1 = 44;

#include <SPI.h>
#include <SD.h>

File myFile;

// change this to match your SD shield or module;
const int chipSelect = 53;

int FloatSensor = 28;   //DI7

int pH_in;
int phval = 0; 


int cond_in;
int condval = 0;

unsigned long int avgval; 
int buffer_arr[10];

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
  


nexInit();

 pinMode(do1, OUTPUT);
  
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
   
  
  int level = 0;
  int buttonState = digitalRead(FloatSensor); 
  if (buttonState == LOW) 
  { 
    int level = 1;
    Serial.println( "WATER LEVEL - HIGH"); 
    digitalWrite(do1, LOW);
  } 
  else 
  { 
    int level = 0;
    Serial.println( "WATER LEVEL - LOW" ); 
    digitalWrite(do1, HIGH);
  } 

  float level_1 = level; 
  
  
  for(int i=0;i<10;i++) 
  { 
    buffer_arr[i]=analogRead(A3);
    delay(30);
  }

  avgval=0;
  for(int i=0;i<10;i++)
  {
    avgval+=buffer_arr[i];
  }

  float pH_in = (float)avgval/10*14.0/1024;
  float ph_act = pH_in;
  Serial.print("pH Val:");
  Serial.print(ph_act);
  Serial.print("; ");


  delay(10);

  for(int i=0;i<10;i++) 
  { 
    buffer_arr[i]=analogRead(A2);
    delay(30);
  }

  avgval=0;
  for(int i=0;i<10;i++)
  {
    avgval+=buffer_arr[i];
  }

  float cond_in = (float)avgval/10;
  float cond_act = map(cond_in, 0, 1023, 0, 1999);
  Serial.print("Cond Val:");
  Serial.print(cond_act);
  Serial.print("; ");


  

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
 

  dataString += String(ph_act);
  dataString += ",";
  dataString += String(cond_act);
  dataString += ",";
  dataString += String(level_1);
  dataString += ",";


  char fileName[14];
  sprintf(fileName, "%4d%02d%02d.CSV", year, month, day);   //Name the file with today's date
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(fileName, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println(dataString);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  //Pot readings to be displayed on the NEXTION display
  n1.setValue(ph_act);
  n0.setValue(cond_act);
  n3.setValue(level);

  

  delay (2000);
  // nothing happens after setup
}
