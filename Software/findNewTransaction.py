# function to detect when a new transaction occurs that outputs the transaction's information 

def findNewTransaction():

	# continuously loop through the most recent 50 transactions in the table 
	# store the top 5 transactions in a locally stored SQL table
		# start with an empty table for the first ever transaction (so that we don't miss the first 5)
		# then table will fill up as transactions come in 
	# compare the newest transaction with the transactions in the SQL table 
	# if the transaction is already in the table
		# return false 
	# if the transaction is not already in the table
		# save it in the table 
		# delete the oldest transaction in the table 
		# output the needed information from the transaction (pick or put, location, RFID)
		# return transaction_type, location, RFID, true 