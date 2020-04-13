# this function gets the ip address of the arduino corresponding to a given location
def getIP(location):
    # query IP database using location 
    import sqlite3
    
    conn = sqlite3.connect('IPAddresses.db')
    cursor = conn.cursor()
    table_name = 'IPAddresses'
    id_column = 'my_1st_column' #primary key
    column_2 = 'my_2nd_column' #ip address
    
    # https://sebastianraschka.com/Articles/2014_sqlite_in_python_tutorial.html#querying-the-database---selecting-rows
    c.execute("SELECT * FROM {tn} WHERE {idf}={my_id}".\
            format(tn=table_name, cn=column_2, idf=id_column, my_id=location))
    #id_exists = c.fetchone()
    #possible error checking
    #if id_exists:
    #    print('5): {}'.format(id_exists))
    #else:
    #    print('5): {} does not exist'.format(some_id))
    
    arduinoIP = c.fetchone()
    
    conn.close()
    
    return arduinoIP
