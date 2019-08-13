/* Final Project
File Name: TestDriver.cpp
Programmer:  Emiliia Dyrenkova
Date Last Modified: 5/23/19
Problem Statement: Creates a secure system to store ASG elections results and identifies the winner
Overall Plan: 
1. Create Blockchain class:
  1. private inner class Block:
    - stores previous hash, current hash, generic data, as well as the link (pointer) to the next block.
    - default constructor creates genesis block: previous hash is the hash of "genesis block", data is empty generic object. Current hash is calculated in normal way.
    - full constructor creates the block from provided previous hash, data and the link to the next block.
    - destructor destroys data pointer
    - calcHash(previousHash) recalculates the hash of the block with the new previous hash (used when validating the chain).
    - getData() returns value of data 
    - << operator overload prints information about the block
  2. contains references (pointers) to head and tail as well as size (does not include genesis block)
  3. Default constructor:
    - use default constructor from Block to create a genesis block and point head and tail to it
    - call getLastHash and output it into the binary file representing the database (or other users to compare to)
  4. getSize returns the size (size)
  5. getLastHash() traverses through the linked list to recalculate all the hashes:
    - sets current block and current hash
    - while the end of list not reached: 
      - call the calcHash on current.next with current hash
      - move current to the next block
    return the last hash (current's hash)
  6. verify() compares last hash of blockchain with database's hash:
    - getLastHash() into variable
    - read the hash from binary file
    - return the result of comparison
  7. addBlock(data) adds new block to the end of the chain:
    - throws a string exception if verify() failed.
    - set tail's next to the new block that takes tail's 
    - repoint tail to the tail's next (new block is the new tail)
    - getLastHash() and put it into the binary file (database)
    - increment the size
  8. toArray() returns a pointer to the array that contains the data from all blocks excluding genesis:
    - traverse the blockchain in a similar way to getLastHash() and put each data point to the array
    - return the array
  9. << operator overload:
    - prints all blocks excluding genesis
2. Test Drive:
  1. printResults() takes in the array of results and array of names and prints the winner and the percentage:
    - sort the array
    - count the repeated items
    - if the counter reached a new max - set max to the counter. save the value that corresponds to max.
    - when reached the new item, reset the counter.
    - print the message
  2. main (demo):
    - create a blockchain object
    - create appropriate variables
    - in the loop:
      - prompt the user if they want to enter the vote or tally the votes
      - if enter the vote, add a block with the choice to the blockchain
      - if tally the votes, call toArray on blockchain and print the message of received array.
      - allow user to exit whenever they'd like
*/
#include <iostream>
#include <cstring>
#include <iomanip>
#include <algorithm>
#include "Blockchain.cpp"
using namespace std;
void printResults(string *arr, int size, string names[],int sizeNames);
int main(int argv, char* argc[])
{
  Blockchain<string> b;
	int userChoice = 0;
  int size;
  string *arr;
  string names[]={"Licol Zeinfeld", "Ahmad Abbasi", "Jose Mendez Entrekin"};
	do {
		cout << "************************************************************************" << endl;
		cout << "**                                                                    **" << endl;
		cout << "**                    Welcome to ASG elections                        **" << endl;  
		cout << "**                                                                    **" << endl;
		cout << "************************************************************************" << endl;
		cout << "** Please make a choice from the following options:                   **" << endl;
		cout << "** 1)  Record the vote                                                **" << endl;
		cout << "** 2)  Tally up the votes                                             **" << endl;
		cout << "** 3)  Exit                                                           **" << endl;
		cout << "************************************************************************" << endl;
		cout << ">> ";
    string vote;
    string *temp;
		cin >> userChoice;
		switch (userChoice)
		{
      case 1:
        cout<<"Presidential elections\n";
        cout<<"Enter the number corresponding to the candidate of your choice:\n";
        cout<<"1. Licol Zeinfeld\n"<<"2. Ahmad Abbasi\n"<<"3. Jose Mendez Entrekin\n";
        cout<<">> ";
        cin>>vote;
        try{
          b.addBlock(new string(vote));
        }
        catch(string s){
          cout<<s<<endl;
          break;
        }
        break;
        case 2:
         // cout<<"The winner is:\n";
          //go through full chain and calculate the winner (function from blockchain class)
          arr=b.toArray();
          printResults(arr, b.getSize(), names, 3);
          //find most frequent in array
          
          //TESTING: PRINT ARRAY
          /*
          cout<<"all votes: \n";
          for(int i=0;i<b.getSize();i++){
            cout<<arr[i]<<endl;
          }*/
          delete[]arr;
          arr=0;
          break;
        case 3:
          cout<<"Thank you for your vote!\n";
          break;
        default:
          cout<<"Invalid choice.\n";
          break;
		}
	} while (userChoice != 3 && userChoice != 2);
	cout << endl;
	//system("PAUSE");
	//return EXIT_SUCCESS;
}

void printResults(string *arr, int size, string names[],int sizeNames){
  sort(arr, arr+size);
  int max=0;
  int count=1;
  string current=arr[0], maxResult=arr[0];
  for(int i=1;i<size;i++){
    if(current==arr[i]){
      count++;
      //current=arr[i];
      if(count>max){
        max=count;
        maxResult=current;
      }
    }
    else{
      current=arr[i];
      count=1;
    }
  }
  cout<<"The winner is: "<<names[atoi(maxResult.c_str())-1]<<". Result is "<<setprecision(3)<<(max*100)/(size)<<"%.\n";
}
