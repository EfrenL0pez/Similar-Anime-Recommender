#include "hashTable.h"

class hashTable
{
    struct anime
    {
        string title = "default";
        int id;
        string description;
        string genre;
        anime* next = nullptr;
        vector <anime*> related;
        string score;
    };

    //remember to delete table and dictionary later
    anime* table = new anime[20000];
    list<pair<string, string>>* dictionary = new list<pair<string, string>>[20000];

    float capacity = 20000;
    float items = 0;
    float dicItems = 0;
    int collisions = 0;

public:
    void insert(string name, string id, string description, string genre, string score)
    {
        anime* a = new anime;
        a->title = name;
        a->id = stoi(id);
        a->description = description;
        a->genre = genre;

        //doesn't insert phantom input
        if(score == "" || name == "")
            return;
        a->score = score;
        int index = idHash(id);

        if(table[index].title == "default")
            table[index] = *a;
        else
        {
            anime* tracker = &table[index];
            while(tracker->next != nullptr)
            {
                tracker = tracker->next;
            }
            tracker->next = a;
            collisions++;
        }

        items++;
    }

    float getLF()
    {
        return items/capacity;
    }

    float getLFD()
    {
        return dicItems/capacity;
    }
    int getSize()
    {
        return items;
    }

    int getCollisions()
    {
        return collisions;
    }

    void insertRel(string lhsID, string rhsID)
    {
        anime* a = find(lhsID);
        anime* b = find(rhsID);
        if(a != nullptr && b != nullptr)
        {
            a->related.push_back(find(rhsID));
        }
    }

    anime* find(string id)
    {
        int index = stoi(id.substr(0,4));
        string last = id.substr(id.size()-1, 1);
        anime* a = &table[index];
        if(a->title == "default")
            return nullptr;
        else
        {
            while(a != nullptr)
            {
                if(a->id == stoi(id))
                    return a;
                a = a->next;
            }
        }
        return nullptr;
    }

    void addToDictionary(string title, string& id)
    {
        if(title != "")
        {
            pair p = make_pair(title, id);
            //makes the string a constant char pointer by using the function c_str to return the pointer to the string and casting it to a const char
            char* c = const_cast<char*>(title.c_str());
            dictionary[stoi(hash(c))].push_back(p);
            dicItems++;
        }
    }

    /* 
       This hash function was written by Daniel J. Bernstein. 
       http://www.cse.yorku.ca/~oz/hash.html
    */

    string hash(const char* str)
    {
        unsigned long hash = 5381;
        int c;
        while(c = *str++)
        {
            hash = ((hash * 32) + hash) + c;
        }
        hash = hash % 20000;
        return to_string(hash);
    }

    int idHash(string id)
    {
        int index = stoi(id.substr(0,4));
        string last = id.substr(id.size()-1, 1);
        if(last == "0")
        {
            index += 10000;
        }
        return index;
    }

    anime* search(string title)
    {
        char* n = const_cast<char*>(title.c_str());
        int index = stoi(hash(n));
        int id;
        //first find the ID in the dictionary
        for (auto iter = dictionary[index].begin(); iter != dictionary[index].end(); iter++)
        {
            //then search for it in the main hash table
            if(title == iter->first)
            {
                int index = idHash(iter->second);
                anime* a = &table[index];
                while (a != nullptr)
                {
                    if(a->title == title)
                        return a;
                    else
                    {
                        a = a->next;
                    }
                }
            }
        }
        return nullptr;
    }

    void displayInfo(string title)
    {
        auto start = std::chrono::high_resolution_clock::now();
        anime* a = search(title);
        if(a != nullptr)
        {
            cout << "Search Term: " << a->title << " Score: " << a->score << endl;
            cout << "Related: " << endl;
            if(a->related.size() > 0)
            {
                sort(a->related.begin(), a->related.end(),
                     [](anime* lhs, anime* rhs)
                     {
                         return (stoi(lhs->score) > stoi(rhs->score));
                     });

                for(anime* r: a->related)
                {
                    if(r != nullptr)
                    {
                        cout << "Name: " << r->title << " Genre: " << r->genre << " Score: " << r->score << endl;
                    }
                }
            }
            else
            {
                cout << "no recommendations found" << endl;
            }
        }
        else
        {
            cout << "Invalid input" << endl;
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        cout << "Retrieval time: " << (float)(duration.count())/1000 << " seconds" << endl;

    }

    void sortTable()
    {
        vector<pair<string, int>> anime;
        for(int i = 0; i < 20000; i++)
        {
            if(table[i].title != "default" && table[i].score != "" && table[i].score != "N/A")
                anime.push_back(make_pair(table[i].title, table[i].related.size()));
        }
        sort(anime.begin(), anime.end(),
             [](pair<string, int> lhs, pair<string, int> rhs)
             {
                 return (lhs.second > rhs.second);
             });

        for(int x = 0; x < 50; x++)
        {
            cout << "title: " << anime[x].first << " recommendations: " << anime[x].second << endl;
        }
    }

};