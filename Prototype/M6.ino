
/*
Code to implement Conductivity Sensors in EMCS and write in SD Card
Written by Niteesh S Shanbog
Date: 30/6/2021
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


int CT001 = 0;	//Conductivity at the outlet of the BRO (concentrated)
int CT002 = 0;	//Conductivity at the inlet of the BRO
int CT003 = 0;	//Conductivity at the outlet of the BRO (permeate)

int buffer_arr[10];
float conductivity = 0;

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
	
	CT001 = measure_conductivity(1);
	CT002 = measure_conductivity(2);
	CT003 = measure_conductivity(3);

	

	write_SD();

	delay (2000);

}


int measure_conductivity(int condType)
{
	int val = 0;
	for (int i=0; i<10; i++)
	{
		buffer_arr[i] = analogRead(A2);
		delay(30);
	}
	unsigned long int avgval = 0;
	for (int i=0; i<10; i++)
	{
		avgval += buffer_arr[i];
	}
	switch (condType)
		{
			case 1:
			{
				//Range: 0-1mS/cm
				conductivity = (float)avgval/10*999/1024;
				break;
			}
			case 2:
			{
				//Range: 0-10mS/cm
				conductivity = (float)avgval/10*9999/1024;
				break;
			}
			case 3:
			{
				//Range: 0-50mS/cm
				conductivity = (float)avgval/10*49999/1024;
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
	  dataString += ",";
  	dataString += String(CT002);
	  dataString += ",";
	  dataString += String(CT003);
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
}
	 	
