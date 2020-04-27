# this function gets the ip address of the arduino corresponding to a given location
def getIP(location):
    # query IP database using location
    import sqlite3

    conn = sqlite3.connect('wms.db')
    c = conn.cursor()
    #define table name and columns
    table_name = 'IPAddresses'
    id_column = 'Location' #primary key
    column_2 = 'IP' #ip address

    #define table name and columns
    c.execute("SELECT * FROM {tn} WHERE {idf}={my_id}".\
            format(tn=table_name, cn=column_2, idf=id_column, my_id=location))

    #possible error checking
    #if id_exists:
    #    print('5): {}'.format(id_exists))
    #else:
    #    print('5): {} does not exist'.format(some_id))

    arduinoIP = c.fetchone()

    conn.close()

    return arduinoIP
