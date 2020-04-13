# Software
This repository contains the software for the LED project. 

code: 
1) main.py - the main script running continuously 
2) findNewTransaction.py - pulls new transactions from the WMS to be processed
3) getIP.py - gets the ip address of the arduino corresponding to an input location
4) onOrOff.py - checks whether the LED at a given location needs to be turned on or off

test code:
1) Python_client.py - current program to test WiFi connectivity to Arduino. To use, input a string in the form [storLoc,pickerID,on/off] (e.g. "356,12,on").
