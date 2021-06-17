# Summer_of_Bitcoin_Challenge
Task for Bitcoin Summer of Code 2021.

## The problem
Bitcoin miners construct blocks by selecting a set of transactions from their mempool. Each transaction in the mempool:

i)includes a fee which is collected by the miner if that transaction is included in a block

ii)has a weight, which indicates the size of the transaction

iii)may have one or more parent transactions which are also in the mempool

The miner selects an ordered list of transactions which have a combined weight below the maximum block weight. Transactions with parent transactions in the mempool may be included in the list, but only if all of their parents appear before them in the list. Naturally, the miner would like to include the transactions that maximize the total fee.

Your task is to write a program which reads a file mempool.csv, with the format:

i)<txid>,<fee>,<weight>,<parent_txids>

ii)txid is the transaction identifier

iii)fee is the transaction fee

iv)weight is the transaction weight

parent_txids is a list of the txids of the transaction’s unconfirmed parent transactions (confirmed parent transactions are not included in this list). It is of
the form: <txid1>;<txid2>;...

The output from the program should be txids, separated by newlines, which make a valid block, maximizing the fee to the miner. Transactions MUST appear in order
(no transaction should appear before one of its parents).

We've included a non-working block_sample.txt file to demonstrate the expected format.

## Approach and Solution:
The approach is based on a Greedy Implementation of an algorithm that-

i)Sorts the transactions in the mempool set in the order of fees, to maximize returns for the miner. 

ii)Next, it checks if all the parents of the transaction are already present in the block or not(before said transaction). This is done using a set to ensure only unique values are present and checked.

iii)If step ii) is true, we include the transaction and update the fees and weight accordingly. The current transaction is also erased from the current set to ensure every transaction is checked only once, else we move to the next higher transaction.

Output:
Total Number of Transactions Read: 5214
Total Number of transactions included in the final block: 3174
Total fees generated from the current block: 5696031
Total weight of the current block: 3999936

![Screenshot](https://github.com/ishaan10dutta/Summer_of_Bitcoin/blob/main/final_output.JPG)
