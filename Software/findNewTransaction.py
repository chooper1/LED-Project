# function to detect when a new transaction occurs that outputs the transaction's information 

def findNewTransaction(prevTransaction):
	if(len(prevTransaction) == 0): # no previous transactions
		#access all transactions that are waiting to be putaway / waiting to be picked
		newTrans = query transactionTable # need to replace this with sql query for all newer objects
        else:
		#access all transactions newer than prevTransaction that are waiting to be putaway / waiting to be picked
		newTrans = query transactionTable # need to replace this with sql query for all newer objects
        
	newTransactions = []
	for i in range(0,len(newTrans)):
		newTransactions.append(newTrans[i]) # assumes newTrans[i] is formatted like (pickOrPut, location, RFID)
	return newTransactions # should be [] if no new transactions
