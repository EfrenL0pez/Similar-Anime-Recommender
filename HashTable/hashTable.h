#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <chrono>
#include <cctype>
#include <set>
using namespace std;
class hashTable
{   //maybe square id and mod by a prime
    struct anime
    {
        string title = "default";
        string realTitle;
        string id;
        string description;
        string genre;
        vector <anime*> related;
        string score;
    };

    //remember to delete table and dictionary later
    list<anime>* table = new list<anime>[20000];
    list<pair<string, string>>* dictionary = new list<pair<string, string>>[20000];

    float capacity = 20000;
    float items = 0;
    float dicItems = 0;
    int collisions = 0;
    set<anime*> recommended;

    public:
    hashTable();
    void readFiles();
    void insert(string name, string id, string description, string genre, string score);

    float getLF();
    float getLFD();
    int getSize();
    int getCollisions();
    void insertRel(string lhsID, string rhsID);
    anime* find(string id);
    void addToDictionary(string title, string& id);

    /* 
       This hash function was written by Daniel J. Bernstein. 
       http://www.cse.yorku.ca/~oz/hashtml
    */
    string hash(const char* str);
    int idHash(string id);
    anime* search(string title);
    string toLower(string& title);
    void displayInfo(string title);
    ~hashTable();

};