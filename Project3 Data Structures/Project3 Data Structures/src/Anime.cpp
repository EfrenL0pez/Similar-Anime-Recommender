#include "src/Anime.h"
Anime::Anime(string title, string upperTitle, int id, string description, string genre, string score) 
{
	this->title = title;
	this->upperTitle = upperTitle;
	this->id = id;
	this->description = description;
	this->genre = genre;
	this->score = score;
}

Anime::Anime(const Anime& rhs) 
{
	this->title = rhs.title;
	this->upperTitle = rhs.upperTitle;
	this->id = rhs.id;
	this->description = rhs.description;
	this->genre = rhs.genre;
	this->score = rhs.score;
	this->related = rhs.related;
}

