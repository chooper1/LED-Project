# Software
This repository contains the software for the LED project.

Code:
1) main.py - the main script running continuously
2) findNewTransaction.py - pulls new transactions from the WMS to be processed
3) getIP.py - gets the ip address of the arduino corresponding to an input location
4) onOrOff.py - checks whether the LED at a given location needs to be turned on or off

Databases:
wms.db - this database is used as a stand-in for testing purposes. It has 3 tables:
1) IPAddresses - used to convert location to the IP address of the microcontroller connected to a location
2) OnOff - used to determine whether an LED needs to be turned on or off
3) transact - contains new incoming transactions that need to be processed
