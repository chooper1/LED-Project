# main function that will run continuously 

# import functions defined in other files 
from getArduinoID import getArduinoID 
from sendPartLocation import sendPartLocation

# coding: utf-8
# In[ ]:
from socket import *
import time

def main():
	prevTransaction = [] 
    	while(1):
		#findNewTransaction() needs to be updated to return multiple transactions, and needs to be able to take in either
		#the previously processed transaction or no transactions
		#pickOrPut, location, RFID = findNewTransaction()
		newTransactions = findNewTransaction(prevTransaction)
		
		if(len(newTransactions) > 0):
			for i in range(0,len(newTransactions)):
				pickOrPut, location, RFID = newTransactions[i] 

				address = getIP(location)
				#address = ('192.168.5.27', 5000) # Define who you are talking to
				client_socket = socket(AF_INET, SOCK_DGRAM) #set up the socket
				client_socket.settimeout(1) #only wait one second for a response

				ON = onOrOff(location, RFID, pickOrPut)

				#sendPartLocationAndRFID(location, RFID, IP)

				#input format: [storage location,pickerID,on/off]
				string_address = location + ',' + RFID + + ',' + ON 
				data = string_address.encode('utf-8') # encode input as a byte for datagram protocol

				client_socket.sendto(data,address) #send command to arduino
				try:
					rec_data, addr = client_socket.recvfrom(2048) #reading response from arduino 
					#print(rec_data.decode()) #Print the response from the arduino
				except:
					#error checking if transmission doesn't work?
					pass
				client_socket.close()
			
			prevTransaction = newTransactions[len(newTransactions)-1]
		# maybe put some type of short time delay here (to wait between queries if no new transactions were found)
if __name__== "__main__":
	main()
