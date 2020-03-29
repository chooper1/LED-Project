# function to detemine whether to turn the LED on or off at a specific location 

def onOrOff(location, RFID, pickOrPut):

	# for the putaway process (if put)
		# location1 = input location
			# location 1 should always be turned on except if location1 = location2
		# location2 = find the last transaction involving the same RFID and extract the location associated with that transaction
			# location 2 should always be turned off except if location1 = location2
		# if location1 = location2 (this would mean that the product was scanned, and then the shelf was scanned, so a new putaway didn't begin)
			# don't return anything 
		# return (location1, location2) 
			# the arduino will control the fact that location1 should always be turned on, and location 2 should always be turned off

	# for the picking process (if pick)
		#
