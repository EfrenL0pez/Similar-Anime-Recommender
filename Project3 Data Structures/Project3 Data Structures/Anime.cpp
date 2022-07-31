#include "Anime.h"
Anime::Anime(string title, int id, string description, string genre, string score) 
{
	this->title = title;
	this->id = id;
	this->description = description;
	this->genre = genre;
	this->score = score;
}

Anime::Anime(const Anime& rhs) 
{
	this->title = rhs.title;
	this->id = rhs.id;
	this->description = rhs.description;
	this->genre = rhs.genre;
	this->score = rhs.score;
	this->related = rhs.related;
}

