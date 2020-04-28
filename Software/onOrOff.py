
# Function that determines if the LED at a specific location needs to be turn on or off

def onOrOff(location, RFID, pickORput):
    import sqlite3
    conn = sqlite3.connect('wms.db')
    c = conn.cursor()
    table_name = 'OnOff'
    id_column = 'location' #not primary key, since two transactions could happen in same time step
    column_2 = 'RFID'
    pick = 0
    put = 1

    # check to see if location already exists in the table that has all the LED locations that are currently on
    check_table_location = c.execute("SELECT * FROM {tn} WHERE {coi1}={my_id} LIMIT 1".format(tn=table_name, coi1=id_column, my_id=location))
    check = c.fetchone()

    # for picking, light can be turned on when the location appears in the WMS and turned off when it appears again
    if pickORput == pick:
    # save location into the database if it is not there and turn on the LED at that location
        if check == None:
            # insert the location into the table
            c.execute("INSERT INTO {tn} ({col1},{col2}) VALUES ({loc},{rf})".format(tn=table_name, col1=id_column, col2=column_2, loc=location, rf=RFID))
            # return the location and 1 to indicate that the location needs to be turned on
            # To save the changes in the files
            conn.commit()
            # close the connection with the databse for onORoff function
            conn.close()
            return location, 1
    # if the location is already in the database, then the light needs to be turned off
        else:
            # delete the location from the table
            c.execute("DELETE FROM {tn} WHERE {coi1}={my_id}".format(tn=table_name, coi1=id_column, my_id=location))
            # return the location and 0 to indicate that the location needs to be turned off
            # To save the changes in the files
            conn.commit()
            # close the connection with the databse for onORoff function
            conn.close()
            return location, 0

    # for putaway, light can turned on when the location appears in the WMS and turned off when the next location appears
    # location must not be turned off if the location is a result of scanning the shelf
    elif pickORput == put:
        # if the location is not already in the table
        if check == None:
            # insert the new location that needs to be turn on into the table
            c.execute("INSERT INTO {tn} ({col1},{col2}) VALUES ({loc},{rf})".format(tn=table_name, col1=id_column, col2=column_2, loc=location, rf=RFID))
            # find the last location that needs to be turned off
            c.execute("SELECT * FROM {tn} WHERE {coi1}={rf} AND {id_col}<>{loc}".format(tn=table_name, coi1=column_2, rf=RFID, id_col=id_column,loc=location))
            location2 = c.fetchone()
            # To save the changes in the files
            conn.commit()
            # close the connection with the databse for onORoff function
            conn.close()
            if (location2 == None):
                # return the new location and 1 to indicate that the location needs to be turned on, and the old location with 0 to be turned off
                return location, 1, location2, 0
            else:
                # return the new location and 1 to indicate that the location needs to be turned on, and the old location with 0 to be turned off
                return location, 1, location2[0], 0 #location2 returns a tuple, so we need to return only the first value
        else:
            conn.close()
            # if the location is already in the table, this means that the scan was a shelf scan so nothing should happen
            return -1,-1
    #if the scan was not for picking or putaway
    else:
            conn.close()
            return -1,-1
