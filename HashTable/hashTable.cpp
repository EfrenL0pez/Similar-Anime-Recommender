#pragma once
#include "hashTable.h"

hashTable::hashTable()
{
    anime a;
    a.title = "default";
    list<anime> l;
    l.push_back(a);
    for(int i = 0; i < 20000; i++)
    {
        table[i] = l;
    }
}

void hashTable::readFiles()
{
    auto start = std::chrono::high_resolution_clock::now();
    ifstream stream;
    //reads anime data
    stream.open("../data/anime.csv");
    if(stream.is_open())
    {
        //reads first line and ignores it
        string buffer;
        getline(stream, buffer);
        while(!stream.eof())
        {
            string id;
            getline(stream, id, '\t');
            string url;
            getline(stream, url, '\t');
            string title;
            getline(stream, title, '\t');
            string synopsis;
            getline(stream, synopsis, '\t');
            string mainPic;
            getline(stream, mainPic, '\t');
            string type;
            getline(stream, type, '\t');
            string source;
            getline(stream, source, '\t');
            string episodes;
            getline(stream, episodes, '\t');
            string status;
            getline(stream, status, '\t');
            string startDate;
            getline(stream, startDate, '\t');
            string endDate;
            getline(stream, endDate, '\t');
            string season;
            getline(stream, season, '\t');
            string studios;
            getline(stream, studios, '\t');
            string genres;
            getline(stream, genres, '\t');
            string score;
            getline(stream, score, '\t');
            string scoreCount;
            getline(stream, scoreCount, '\t');
            string scoreRank;
            getline(stream, scoreRank, '\t');
            string popRank;
            getline(stream, popRank, '\t');
            string memCount;
            getline(stream, memCount, '\t');
            string favCount;
            getline(stream, favCount, '\t');
            //computes score if it isn't in the dataset
            if(score == "")
            {
                float average = 0;
                float total = 0;
                for(int i = 10; i > 0; i--)
                {
                    string buffer;
                    getline(stream, buffer, '\t');
                    if(buffer != "")
                    {
                        total += stof(buffer);
                        float x = stof(buffer);
                        average += (x*(float)i);
                    }
                }

                score = (average == 0) ? "N/A" : to_string(average/total);
            }
            while (id.length() < 6)
            {
                id = "0" + id;
            }
            insert(title, id, synopsis, genres, score);
            addToDictionary(title, id);
            getline(stream, buffer); //goes through useless data
           
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
   
    stream.close();
   
    stream.open("../data/anime_anime.csv");
    if(stream.is_open())
    {
        string buffer;
        getline(stream, buffer);

        while(!stream.eof())
        {
            string idA;
            string idB;
            string rec;
            string recURL;
            string recNum;
            string rel;
            string relType;
            getline(stream, idA, '\t');
            getline(stream, idB, '\t');
            getline(stream, rec, '\t');
            getline(stream, recURL, '\t');
            getline(stream, rel, '\t');
            getline(stream, relType, '\t');
            while (idA.length() < 6)
            {
                idA  = "0" + idA;
            }
            while (idB.length() < 6)
            {
                idB  = "0" + idB;
            }

            if(rec == "1" && idA != "000000")
                insertRel(idA, idB);
            getline(stream, buffer);
        }
    }
    cout << "Time to insert " << (getSize()*2) << " elements into hash tables: " << (float)(duration.count())/1000 << " seconds" << endl;
    cout << "Number of collisions: " << getCollisions() << endl;
}
void hashTable::insert(string name, string id, string description, string genre, string score)
{
        anime a;
        a.title = name;
        a.id = id;
        a.description = description;
        a.genre = genre;

        //doesn't insert phantom input
        if(score == "" || name == "")
            return;
        a.score = score;
        int index = idHash(id);
        //cout << name << " " << index << endl;
        
        if(table[index].front().title == "default")
            table[index].front() = a;
        else
        {
            table[index].push_back(a);
            collisions++; 
        }
        items++;
}

float hashTable::getLF()
{
    return items/capacity;

}

float hashTable::getLFD()
{
    return dicItems/capacity;

}

int hashTable::getSize()
{
    return items;

}

int hashTable::getCollisions()
{
    return collisions;
}

void hashTable::insertRel(string lhsID, string rhsID)
    {
        anime* a = find(lhsID);
        anime* b = find(rhsID);
        if(a != nullptr && b != nullptr)
        {
            a->related.push_back(find(rhsID));
        }
    }

hashTable::anime* hashTable::find(string id)
    {
        anime* a = &table[idHash(id)].front();
            for(auto iter = table[idHash(id)].begin(); iter != table[idHash(id)].end(); iter++)
            {
                if (iter->id == id)
                    return &*iter;
            }
        return nullptr;
    }

    void hashTable::addToDictionary(string title, string& id)
    {
        if(title != "")
        {
            pair p = make_pair(title, id);
            //makes the string a constant char pointer by using the function c_str to return the pointer to the string and casting it to a const char
            char* c = const_cast<char*>(title.c_str());
            dictionary[stoi(hash(c))].push_back(p);
            if(dictionary[stoi(hash(c))].size() > 2)
            {
                collisions++;
            }
            dicItems++;
        }
    }

    /* 
       This hash function was written by Daniel J. Bernstein. 
       http://www.cse.yorku.ca/~oz/hash.html
    */

    string hashTable::hash(const char* str)
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

    int hashTable::idHash(string id)
    {
        int index = stoi(id.substr(1,4));
        string first = id.substr(0,1);
        if(first == "0" && (index % 2) == 0)
           index += 10000;
       return index;
    }

    hashTable::anime* hashTable::search(string title)
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
                for (auto iter = table[index].begin(); iter != table[index].end(); iter++)
                {
                    if(iter->title == title)
                        return &*iter;
                }
            }
        }
        return nullptr;
    }

    void hashTable::displayInfo(string title)
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
                    return (stof(lhs->score) > stof(rhs->score));
                });
                int i = 0;
                int c = 0;
                while(c < 5 && i < a->related.size())
                {
                    anime* r = a->related[i];
                    i++;
                    //skips titles that have already been recommended
                    if(recommended.count(r) == 0 && r != nullptr)
                    {
                        cout << "Name: " << r->title << " Genre: " << r->genre << " Score: " << r->score << endl;
                        cout << r->description << "\n\n";
                        recommended.insert(r);
                        c++;
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
    
    hashTable::~hashTable()
    {
        delete[] table;
        delete[] dictionary;
    }