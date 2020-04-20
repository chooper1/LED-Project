# Software
This repository contains the software for the LED project. 

Code: 
1) main.py - the main script running continuously 
2) findNewTransaction.py - pulls new transactions from the WMS to be processed
3) getIP.py - gets the ip address of the arduino corresponding to an input location
4) onOrOff.py - checks whether the LED at a given location needs to be turned on or off
5) SetupConn.py - function for accessing the WMS

Databases:
1) IPAddresses.db - this database is used for converting location number to IP address for selecting the microcontroller that you want to communicate with
2) wms.db - this database is used as a stand-in for testing purposes until we have access to the wms.

test code:
1) Python_client.py - current program to test WiFi connectivity to Arduino. To use, input a string in the form [storLoc,pickerID,on/off] (e.g. "356,12,on").
