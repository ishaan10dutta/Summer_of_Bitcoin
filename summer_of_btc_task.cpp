//Ishaan Dutta - 17th June 2021
//Repo Link: https://github.com/ishaan10dutta/Summer_of_Bitcoin
#include<bits/stdc++.h>
using namespace std;

//Defining the Global Variables
string filename="mempool.csv"; //Contains details of fees,weight and parent transactions in the mempool set
int max_block_weight=4000000;//The total weight of transactions in a block must not exceed 4,000,000 weight.

//Defining a Transaction and related information as a class
class Transaction{
    public:
        string transaction_id;
        int fee;
        int weight;
        vector<string> parents;
    };

pair<string, Transaction*> addTransaction(vector<string>& transaction_details){
    auto transaction = new Transaction(); //transaction is a new object of the Transaction class
    transaction->transaction_id = transaction_details[0]; //transaction id defined
    transaction->fee = stoi(transaction_details[1]); //fee defined; string input converted to int
    transaction->weight = stoi(transaction_details[2]); //weight defined; string input converted to int
    vector<string> parent_ids; //for parent ids
    
    for (int i = 3; i < transaction_details.size(); i++){
        parent_ids.push_back(transaction_details[i]);
    }

    transaction->parents = parent_ids;
    return {transaction_details[0], transaction};
    }

//Method for reading the csv file(mempool.csv), containing details of all the transactions in the mempool set
void read_csv(string filename, unordered_map<string, Transaction*>& transactions){
    ifstream fin(filename);
    vector<string> transaction_details;
    string temp,line,word;
    getline(fin,line);
    while(getline(fin,line)){
        transaction_details.clear();
        stringstream s(line);
        while(getline(s,word,',')){
            transaction_details.push_back(word);
        }
        pair<string,Transaction*> p = addTransaction(transaction_details);
        transactions[p.first] = p.second;
    }
    fin.close();
    cout<<"Total Number of Transactions Read: "<<transactions.size()<<endl;
    }

//Method to check if the selected transaction should be included as valid, ie to maximize fees, have parents appearing before them etc
bool isValidTransaction(Transaction *transaction, set<string> &included_transaction_ids){
    //If all the parents of the transaction are present in included_transaction_ids
    //and appear before the transaction in the csv, then it is valid
    for (auto parent:transaction->parents)
    {
        if (included_transaction_ids.find(parent) == included_transaction_ids.end())
            return false;
    }
    return true;
    }

//Method for generating the output
void getOutput(vector<string>& final_included_transactions, string filename){
    ofstream myfile(filename);
    for(auto s:final_included_transactions){
        myfile << s << endl;
    }
    myfile.close();
    }

//Main Function
int main(){
    unordered_map<string, Transaction*> transaction;
    read_csv(filename, transaction);
    set<pair<float, Transaction*>, greater<pair<float, Transaction*>>>transactionSet; //This maintains the order of max fee/weight first
    set<string> included_transactions_set; //This includes all transaction which are included in the block
    vector<string> final_included_transactions; //This maintains the final list of transactions included, in the order of output
    
    for(auto i:transaction){
        transactionSet.insert({(float)i.second->fee / (float)i.second->weight, i.second});
    }

    int current_block_weight=0; //For calculating the current block weight 
    int total_fees=0; //For calculating the total fees generated

    while(!transactionSet.empty() && (current_block_weight < max_block_weight)){
        bool found = false;

        for(auto itr = transactionSet.begin(); itr != transactionSet.end(); itr++){
            Transaction* current_Transaction = (*itr).second;
            int current_fee = current_Transaction->fee;
            int current_weight = current_Transaction->weight;

            if(isValidTransaction(current_Transaction, included_transactions_set) && (current_block_weight + current_weight)<=max_block_weight){
                current_block_weight += current_weight;
                included_transactions_set.insert(current_Transaction->transaction_id);
                final_included_transactions.push_back(current_Transaction->transaction_id);
                total_fees += current_fee;
                transactionSet.erase(itr);
                found = true;
                break;
            }
        }
        if(!found){
            break;
        }
    }
    cout << "Total Number of transactions included in the final block: " << included_transactions_set.size() << endl;
    cout << "Total fees generated from the current block: " << total_fees << endl;
    cout << "Total weight of the current block: " << current_block_weight << endl;
    getOutput(final_included_transactions, "block.txt");

    return 0;
}