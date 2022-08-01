#include "src/RBTree.h"
#include "src/hashtable.h"



int main() 
{
	RBTree test;
	
	test.readfiles();
	string userInput;

	cout << "What anime? " ;
	getline(cin, userInput);
	cout << endl;

	test.displayinfo(userInput);

	return 0;
}