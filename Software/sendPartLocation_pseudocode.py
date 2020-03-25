# this function sends the location to the microcontroller (without RFID)

def sendPartLocation(location, ON_OFF, arduinoID):
    #use built in library to transmit location to the arduino corresponding to arduinoID
    #use built in library to transmit ON_OFF to the arduino corresponding to arduinoID
    #the arduino should block while waiting for these two commands
