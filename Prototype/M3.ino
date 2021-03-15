#include <SPI.h>
#include <SD.h>

File myFile;

// change this to match your SD shield or module;
const int chipSelect = 53;

int FloatSensor = 22;   //DI1

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

  
}

void loop()
{
  int level = 0;
  int buttonState = digitalRead(FloatSensor); 
  if (buttonState == LOW) 
  { 
    int level = 1;
    Serial.println( "WATER LEVEL - HIGH"); 
  } 
  else 
  { 
    int level = 0;
    Serial.println( "WATER LEVEL - LOW" ); 
  } 

  float level_1 = level; 
  
  
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

  float pH_in = (float)avgval/10*14.0/1024;
  float ph_act = pH_in;
  Serial.print("pH Val:");
  Serial.print(ph_act);
  Serial.print("; ");


  delay(10);

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

  float cond_in = (float)avgval/10*14.0/1024;
  float cond_act = cond_in;
  Serial.print("Cond Val:");
  Serial.print(cond_act);
  Serial.print("; ");


  

  String dataString = "";
 

  dataString += String(ph_act);
  dataString += ",";
  dataString += String(cond_act);
  dataString += ",";
  dataString += String(level);
  dataString += ",";
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

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


  

  delay (2000);
  // nothing happens after setup
}

