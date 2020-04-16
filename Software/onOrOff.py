# Function that determines if the LED at a specific location needs to be turn on or off

def onOrOff(location):

	# check to see if location already exists in the table that has all the LED locations that are currently on 
	check_table = crsr.execute("SELECT EXISTS(SELECT * from table_on WHERE table_on.Location = %s)", (location))

	# save location into the database if it is not there 
	if check_table = 0:
		# insert the location into the table 
		crsr.execute("INSERT INTO table_on (Location) VALUES (%s)", (location))
		# return the location and 1 to indicate that the location needs to be turned on
		return location, 1
	else:
		# delete the location from the table
		crsr.execute("DELETE FROM table_on WHERE table_on.Location = %s", (location))
		# return the location and 0 to indicate that the location needs to be turned off
		return location, 0
