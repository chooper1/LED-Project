# function to detect when a new transaction occurs that outputs the transaction's information

def findNewTransaction(prevTransactionNumber):
    import sqlite3
    import numpy

    conn = sqlite3.connect('wms.db')
    c = conn.cursor()
    #define table name and columns
    table_name = 'transact'
    id_column = 'test' #not primary key, since two transactions could happen in same time step
    column_2 = 'pickOrPut'
    column_3 = 'location'
    column_4 = 'RFID'

    #query the transact table
    c.execute("SELECT {coi2},{coi3},{coi4} FROM {tn} WHERE {coi1}={my_id}".format(coi2=column_2, coi3=column_3, coi4=column_4, tn=table_name, coi1=id_column, my_id=prevTransactionNumber))
    #get all rows that need to be currently processed
    rows = c.fetchall()
    if (rows):
        #format of each element of newTransactions is [pickOrPut, location, RFID]
        newTransactions = numpy.array(rows)
    else:
        #return [] if no new transactions
        newTransactions = []
    conn.close()
    return newTransactions # should be [] if no new transactions
