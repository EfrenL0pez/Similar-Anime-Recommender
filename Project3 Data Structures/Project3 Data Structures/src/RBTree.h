#pragma once
#include <iostream>
#include <algorithm>
#include <chrono>
#include <queue>
#include <fstream>
#include <unordered_set>
#include "Anime.h"

class RBTree
{
private:
	struct Node
	{
		int height = 0;
		int minHeight = 0;
		bool black = false;
		Anime anime;
		Node* left = nullptr;
		Node* right = nullptr;
		Node* parent = nullptr;
		Node* sibling = nullptr;

		Node(Anime* anime, Node* parent);
	};
	int size = 0;
	bool successfulInsert = false;
	bool balanced = false;
	bool sortID = false;
	Node* root = nullptr;
	unordered_set<string> recommended;

	bool CheckValidRecursive(Node* curr);
	Node* RotLeft(Node* curr);
	Node* RotRight(Node* curr);
	pair<Node*, Node*> InsertRecursive(Anime* anime, Node* curr);
	pair<Node*, Node*> InsertRecursiveTitle(Anime* anime, Node* curr);
	void PrintInorderRecursive(Node* curr);
	void DeleteTree(Node* root);
	Anime* FindRecursive(int id, Node* curr);
	Anime* FindRecursive(string title, Node* curr);
	int TreeHeightRecursive(Node* curr);
	bool IsHeightBalanced(Node* root, int& rootMax);
	string toLower(const string& title);

public:
	bool IsBalanced();
	void MinHeights();
	bool CheckValid();
	int TreeHeight();
	bool Insert(Anime anime);
	Anime* Find(int id);
	Anime* Find(string title);
	void PrintInorder();
	void readfiles();
	void displayinfo(string userInput);
	RBTree(bool sortID = false);
	~RBTree();
};

