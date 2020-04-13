# main function that will run continuously 

# import functions defined in other files 
from getArduinoID import getArduinoID 
from sendPartLocation import sendPartLocation

def main():
	
    while(1):
        pickOrPut, location, RFID = findNewTransaction()

        sendPartLocation(location)

        getArduinoID(location)
	

if __name__== "__main__":
    main()
