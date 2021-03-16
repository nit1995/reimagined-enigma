15/3/2021
1. Problem with interfacing DS3231. Looks to be an I2C or Power issue. Needs debugging. 

2. M2 succesfully made without RTC. **No timestamp**.

3. M3 - pH Sensor, Conductivity Meter and Level Sensor integrated to M2. Problem with RTC not resolved. **No time stamp**. **Calibration of Conductivity Meter required.**

4. M4 - Added NEXTION Display to M3. Calibrated Conductivity Sensor. **No timestamp**



***********************************************************************************
16/3/2021
1. SDA and SCL lines interchanged in the PCB. PCB physically corrected by cutting the traces and using jumper wires to make the proper connections. Timestamp issue solved in M1. 
