
# coding: utf-8

# In[ ]:


from socket import *
import time

address = ( '192.168.1.88', 5000) # Define who you are talking to
client_socket = socket(AF_INET, SOCK_DGRAM) #set up the socket
client_socket.settimeout(1) #only wait one second for a response

while(1): # Main Loop
    string_address = input() # ask for user input
    data = string_address.encode('utf-8') # encode input as a byte for datagram protocol
    client_socket.sendto(data,address) #send command to arduino
    try:
        rec_data, addr = client_socket.recvfrom(2048) #reading response from arduino 
        print(rec_data.decode()) #Print the response from the arduino
    except:
        pass 

