# function to detect when a new transaction occurs that outputs the transaction's information 

def findNewTransaction(prevTransactionNumber):
	import sqlite3
	import numpy 
	
	conn = sqlite3.connect('wms.db')
	cursor = conn.cursor()
	table_name = 'transaction'
	id_column = 'test' #not primary key, since two transactions could happen in same time step
	column_2 = 'pickOrPut' 
	column_3 = 'location'
	column_4 = 'RFID' 
    	# https://sebastianraschka.com/Articles/2014_sqlite_in_python_tutorial.html#querying-the-database---selecting-rows
    	c.execute("SELECT {coi1},{coi2},{coi3} FROM {tn} WHERE {idf}={my_id}".\
      	      format(tn=table_name, coi1=column_2, coi2=column3, coi3=column4; idf=id_column, my_id=prevTransactionNumber))
	
	rows = c.fetchall()
	if (rows):
		newTransactions = numpy.array(rows) 
	else: 
		newTransactions = []
	conn.close()
    
	return newTransactions # should be [] if no new transactions
