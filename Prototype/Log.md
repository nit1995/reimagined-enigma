**30/6/2021**
Revision to M5 - write_SD() function added to write datastring into SD Card


****************************************************************************************************
**29/6/2021**

M5.ino - Program to take data from all 10 Magnetic Float Sensors to the EMCS and store them in SD Card with timestamp.

Wrote function to check level from the level sensor. 

**Testing to be done**

Read this source to understand how to write functions for Arduino code: https://canvas.instructure.com/courses/1054116/pages/arduino-coding-writing-functions-4-examples
*********************************************************************************************

**19/3/2021**

Added sleep to Nextion Display in Nextion GUI Editor

```
//The following code is only run once when power on, and is generally used for global variable definition and power on initialization data
int sys0=0,sys1=0,sys2=0    
thsp=5    //Sleep on No Touch
thup=1    //Auto wake up on touch
page 0                       //Power on start page 0
```
****************************************************************************************
**16/3/2021**
1. SDA and SCL lines interchanged in the PCB. PCB physically corrected by cutting the traces and using jumper wires to make the proper connections. Timestamp issue solved in M1. 
2. Timestamp added to M4. 

**************************************************************************************
**15/3/2021**
1. ~Problem with interfacing DS3231. Looks to be an I2C or Power issue. Needs debugging.~ *Problem in PCB - Solved*

2. M2 succesfully made without RTC. ~**No timestamp**~.

3. M3 - pH Sensor, Conductivity Meter and Level Sensor integrated to M2. Problem with RTC not resolved. ~**No time stamp**~. **Calibration of Conductivity Meter required.**

4. M4 - Added NEXTION Display to M3. Calibrated Conductivity Sensor. ~**No timestamp**~



***********************************************************************************


