#pragma once
#include <vector>
#include <string>
using namespace std;

class Anime
{
public:
    string title = "default";
    string upperTitle = "";
    int id;
    string description;
    string genre;
    vector <Anime*> related;
    string score;

    Anime(string title = "default", string upperTitle = "", int id = 0, string description = " ", string genre = " ", string score = " ");
    Anime(const Anime& rhs);
};

