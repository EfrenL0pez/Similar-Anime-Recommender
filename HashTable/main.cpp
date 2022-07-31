
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <chrono>
#include "hashTable.h"
#include "RBTree.h"
using namespace std;

int main()
{   
    hashTable h;
    RBTree r;
    h.readFiles();
    r.readfiles();
    bool htm = true;
    while(true)
    {
        if(htm == true)
        {
        cout << "Type in anime (type -1 to exit and -2 to change mode)" << endl;
        cout << "-hash table mode" << endl;
        string input;
        getline(cin, input);
        
        if(input == "-1")
        {
            cout << "Exiting..." << endl;
            break;
        }
        else if (input == "-2")
        {
            htm = false;
        }
        else
        {
            h.displayInfo(input);
        }
        }
        
        else
        {
        cout << "Type in anime (type -1 to exit and -2 to change mode)" << endl;
        cout << "-red black tree mode" << endl;

        string input;
        getline(cin, input);
        
        if(input == "-1")
        {
            cout << "Exiting..." << endl;
            break;
        }
        else if (input == "-2")
        {
            htm = true;
        }
        else
        {
            r.displayinfo(input);
        }
        }
    }
    
    return 0;
}