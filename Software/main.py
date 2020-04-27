# main function that will run continuously

# import functions defined in other files
from getIP import getIP
from findNewTransaction import findNewTransaction
from onOrOff import onOrOff
#from sendPartLocation import sendPartLocation

#import sqlite3

# coding: utf-8
# In[ ]:
from socket import *
import numpy as np
from time import *

def main():
    prevTransactionNumber = 0
    sleep(2)
    while(1):
        #findNewTransaction() gets all the recent transactions that need to be processed.
        #this is the only function that would need to be changed if we got access to the WMS
        newTransactions = findNewTransaction(prevTransactionNumber)

        if(len(newTransactions) > 0):
            for i in range(0,len(newTransactions)):
                # looping through new transactions
                pickOrPut, location, RFID = newTransactions[i]
                #pick 0, put 1

                # check if the location needs to be turned on or off
                ON_Tuple = onOrOff(location, RFID, pickOrPut)
                if (len(ON_Tuple) == 2 and ON_Tuple[0] != -1): #picking
                    #ON_Tuple format: [location, ON/OFF]

                    #input format: [storage location,pickerID,on/off]
                    string_address = str(ON_Tuple[0]) + "," + np.array2string(RFID) + "," + str(ON_Tuple[1])
                    data = string_address.encode('utf-8') # encode input as a byte for datagram protocol
                    #get IP address of the microcontroller connected to the location
                    address = getIP(location)
                    #send the command to the arduino
                    client_socket = socket(AF_INET, SOCK_DGRAM) #set up the socket
                    client_socket.settimeout(1) #only wait one second for a response
                    send_address = (str(address[1]), 5000)
                    client_socket.sendto(data,send_address) #send command to arduino
                    try:
                        rec_data, addr = client_socket.recvfrom(2048) #reading response from arduino
                        #print(rec_data.decode()) #Print the response from the arduino
                    except:
                        #error checking if transmission doesn't work?
                        pass
                    client_socket.close()

                if (len(ON_Tuple) == 4): #putaway
                    location = [ON_Tuple[0], ON_Tuple[2]]
                    ON = [ON_Tuple[1], ON_Tuple[3]]
                    #ON_Tuple format: [location1, ON/OFF1, location2, ON/OFF2]
                    for i in range(0,2): #for each location in the tuple
                        if (str(location[i]) != 'None'):
                            #input format: [storage location,pickerID,on/off]
                            string_address = str(location[i]) + "," + np.array2string(RFID) + "," + str(ON[i])
                            data = string_address.encode('utf-8') # encode input as a byte for datagram protocol
                            #get IP address of the microcontroller connected to the location
                            address = getIP(location[i])
                            #send the command to the arduino
                            client_socket = socket(AF_INET, SOCK_DGRAM) #set up the socket
                            client_socket.settimeout(1) #only wait one second for a response
                            send_address = (str(address[1]), 5000)
                            client_socket.sendto(data,send_address) #send command to arduino
                            try:
                                rec_data, addr = client_socket.recvfrom(2048) #reading response from arduino
                                #print(rec_data.decode()) #Print the response from the arduino
                            except:
                                #error checking if transmission doesn't work?
                                pass
                            client_socket.close()
                            sleep(1);
        prevTransactionNumber = prevTransactionNumber + 1
        print(prevTransactionNumber)
        sleep(5) # for testing


if __name__== "__main__":
    main()
