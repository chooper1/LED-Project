# Software
This repository contains the software for the LED project. 

Pseudocode:
1) readScannerID_pseudocode.py - reads the ID of the scanner currently being used
2) sendPartLocationAndRFID_pseudocode.py - sends the location and RFID of the part that was just scanned to the microcontroller
3) sendPartLocation_pseudocode.py - sends the location of the part that was just scanned to the microcontroller
4) getArduinoID_pseudocode.py - gets the arduino ID corresponding to a given location in the warehouse

code: 
1) main.py - the main script running continuously 
2) Python_client.py - current program to test WiFi connectivity to Arduino. To use, input a string in the form [storLoc,pickerID,on/off] (e.g. "356,12,on").
