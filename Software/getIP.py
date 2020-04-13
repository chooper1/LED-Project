# this function gets the ip address of the arduino corresponding to a given location

def getIP(location):
    # query IP database using location (replace this with SQL query)
    arduinoIP = IPDatabase[location]
    return arduinoIP
