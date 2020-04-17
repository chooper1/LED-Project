# Function that determines if the LED at a specific location needs to be turn on or off

def onOrOff(location, RFID, pickORput):

	# check to see if location already exists in the table that has all the LED locations that are currently on 
	check_table_location = crsr.execute("SELECT EXISTS(SELECT * from table_on WHERE table_on.Location = %s)", (location))

	# for picking, light can be turned on when the location appears in the WMS and turned off when it appears again
	if pickORput == pick:
	# save location into the database if it is not there and turn on the LED at that location
		if check_table_location == 0:
			# insert the location into the table 
			crsr.execute("INSERT INTO table_on VALUES (%s, %s)", (location, RFID))
			# return the location and 1 to indicate that the location needs to be turned on
			return location, 1
	# if the location is already in the database, then the light needs to be turned off 
		else: 
			# delete the location from the table
			crsr.execute("DELETE FROM table_on WHERE table_on.Location = %s", (location))
			# return the location and 0 to indicate that the location needs to be turned off
			return location, 0

	# for putaway, light can turned on when the location appears in the WMS and turned off when the next location appears 
	# location must not be turned off if the location is a result of scanning the shelf 
	else:
		# if the location is not already in the table
		if check_table_location == 0:
			# insert the new location that needs to be turn on into the table 
			crsr.execute("INSERT INTO table_on VALUES (%s, %s)", (location, RFID))

			# find the last location that needs to be turned off 
			location 2 = crsr.execute("SELECT * from table_on WHERE table_on.RFID = %s AND table_on.Location <> %s)", (RFID, location))

			# return the new location and 1 to indicate that the location needs to be turned on, and the old location with 0 to be turned off
			return location, 1, location 2, 0 
	
		else:
			# if the location is already in the table, this means that the scan was a shelf scan so nothing should happen
			return NULL 
			

			





