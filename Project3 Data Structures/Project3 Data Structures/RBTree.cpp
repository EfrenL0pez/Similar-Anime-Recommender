#include "src/RBTree.h"


RBTree::Node::Node(Anime* anime, Node* parent) 
{
	this->anime.title = anime->title;
	this->anime.upperTitle = anime->upperTitle;
	this->anime.id = anime->id;
	this->anime.description = anime->description;
	this->anime.genre = anime->genre;
	this->anime.score = anime->score;
	this->parent = parent;
}

RBTree::RBTree(bool sortByID) 
{
	sortID = sortByID;
}

void RBTree::MinHeights() 
{
	queue<Node*> nodes;

	nodes.push(root);

	while (!nodes.empty())
	{
		Node* curr = nodes.front();
		if (curr->left != nullptr && curr->right != nullptr)
		{
			curr->minHeight = fmin(curr->left->height, curr->right->height);
			nodes.push(curr->left);
			nodes.push(curr->right);
			nodes.pop();
		}
		else if (curr->left != nullptr)
		{
			curr->minHeight = curr->left->height;
			nodes.push(curr->left);
			nodes.pop();
		}
		else if (nodes.front()->right != nullptr)
		{
			curr->minHeight = curr->right->height;
			nodes.push(curr->right);
			nodes.pop();
		}
		else
		{
			curr->minHeight = curr->height;
			nodes.pop();
		}
	}
}

RBTree::Node* RBTree::RotLeft(Node* curr) 
{
	Node* tempParent = curr->parent;
	Node* tempSibling = curr->sibling;
	Node* temp = curr->right->left;
	curr->parent = curr->right;
	curr->sibling = curr->right->sibling;
	curr->right->sibling = tempSibling;
	curr->right->parent = tempParent;
	curr->right->left = curr;
	curr->right = temp;
	
	return curr->parent;
}

RBTree::Node* RBTree::RotRight(Node* curr)
{
	Node* tempParent = curr->parent;
	Node* tempSibling = curr->sibling;
	Node* temp = curr->left->right;
	curr->parent = curr->left;
	curr->sibling = curr->left->sibling;
	curr->left->sibling = tempSibling;
	curr->left->parent = tempParent;
	curr->left->right = curr;
	curr->left = temp;
	return curr->parent;
}

bool RBTree::Insert(Anime anime) 
{
	successfulInsert = false;
	balanced = false;
	if (sortID) 
	{
		root = InsertRecursive(&anime, root).first;
	}
	else 
	{
		root = InsertRecursiveTitle(&anime, root).first;
	}
	root->black = true;
	return successfulInsert;
}

pair<RBTree::Node*, RBTree::Node*> RBTree::InsertRecursive(Anime* anime, Node* curr) 
{
	Node* grandchild = nullptr;
	Node* child = nullptr;
	if (curr == nullptr) 
	{
		curr = new Node(anime, nullptr);
		size++;
		successfulInsert = true;
		return make_pair(curr, nullptr);
	}
	else if (curr->anime.id < anime->id) 
	{
		pair<Node*, Node*> temp = InsertRecursive(anime, curr->right);
		curr->right = temp.first;
		grandchild = temp.second;
		child = temp.first;
		if (child != nullptr) 
		{
			child->parent = curr;
			child->sibling = curr->left;
			if (curr->left != nullptr) 
			{
				curr->left->sibling = child;
			}
		}
	}
	else if (curr->anime.id > anime->id) 
	{
		pair<Node*, Node*> temp = InsertRecursive(anime, curr->left);
		curr->left = temp.first;
		grandchild = temp.second;
		child = temp.first;
		if (child != nullptr) 
		{
			child->parent = curr;
			child->sibling = curr->right;
			if (curr->right != nullptr)
			{
				curr->right->sibling = child;
			}
		}
	}
	else 
	{
		return make_pair(nullptr, nullptr);
	}

	if (child != nullptr && child->black)
	{
		balanced = true;
		return make_pair(curr, child);
	}
	else if (child != nullptr && grandchild != nullptr && !child->black && !grandchild->black)
	{
		if (child->sibling != nullptr && !child->sibling->black)
		{
			child->black = true;
			child->sibling->black = true;
			curr->black = false;
			balanced = true;
			return make_pair(curr, child);
		}
		else
		{
			if (curr->left == child && child->left == grandchild)
			{
				pair<Node*, Node*> temp = make_pair(RotRight(curr), grandchild);
				bool color = curr->parent->black;
				curr->parent->black = curr->black;
				curr->black = color;
				balanced = true;
				return temp;
			}
			else if (curr->right == child && child->right == grandchild)
			{
				pair<Node*, Node*> temp = make_pair(RotLeft(curr), grandchild);
				bool color = curr->parent->black;
				curr->parent->black = curr->black;
				curr->black = color;
				balanced = true;
				return temp;
			}
			else if (curr->left == child && child->right == grandchild)
			{
				curr->left = RotLeft(child);
				pair<Node*, Node*> temp = make_pair(RotRight(curr), child);
				bool color = curr->parent->black;
				curr->parent->black = curr->black;
				curr->black = color;
				balanced = true;
				return temp;
			}
			else 
			{
				curr->right = RotRight(child);
				pair<Node*, Node*> temp = make_pair(RotLeft(curr), child);
				bool color = curr->parent->black;
				curr->parent->black = curr->black;
				curr->black = color;
				balanced = true;
				return temp;
			}
		}
	}
	else 
	{
		return make_pair(curr, child);
	}
}

void RBTree::PrintInorder() 
{
	PrintInorderRecursive(root);
}

void RBTree::PrintInorderRecursive(Node* curr) 
{
	if (curr == nullptr) 
	{
		return;
	}
	PrintInorderRecursive(curr->left);
	cout << curr->anime.title << " " << curr->anime.id << endl;
	cout << "Genre(s): " << curr->anime.genre << "   Score: " << curr->anime.score << endl;
	cout << curr->anime.description << endl << endl << endl;
	PrintInorderRecursive(curr->right);
}

RBTree::~RBTree() 
{
	DeleteTree(root);
}

void RBTree::DeleteTree(Node* curr) 
{
	if (curr == nullptr) 
	{
		return;
	}

	DeleteTree(curr->left);
	DeleteTree(curr->right);
	delete curr;
}

Anime* RBTree::Find(int id) 
{
	successfulInsert = false;
	if (!sortID) 
	{
		cout << "Wrong Find function for this RBTree" << endl;
		return nullptr;
	}
	Anime* temp = FindRecursive(id, root);
	if (!successfulInsert)
	{
		cout << "Unsuccessful" << endl;
	}
	return temp;
}

Anime* RBTree::FindRecursive(int id, Node* curr) 
{
	if (curr == nullptr) 
	{
		return nullptr;
	}
	else if (id < curr->anime.id) 
	{
		Anime* temp = FindRecursive(id, curr->left);
		return temp;
	}
	else if (id > curr->anime.id) 
	{
		Anime* temp = FindRecursive(id, curr->right);
		return temp;
	}
	else 
	{
		successfulInsert = true;
		return &(curr->anime);
	}
}

int RBTree::TreeHeightRecursive(Node* curr) 
{
	// reached a leaf node
	if (curr == nullptr)
	{
		return 0;
	}


	// Add 1 for every node back up the tree
	curr->height = 1 + fmax(TreeHeightRecursive(curr->left), TreeHeightRecursive(curr->right));
	return curr->height;
}

int RBTree::TreeHeight() 
{
	return TreeHeightRecursive(root);
}

pair<RBTree::Node*, RBTree::Node*> RBTree::InsertRecursiveTitle(Anime* anime, Node* curr)
{
	Node* grandchild = nullptr;
	Node* child = nullptr;
	if (curr == nullptr)
	{
		curr = new Node(anime, nullptr);
		size++;
		successfulInsert = true;
		return make_pair(curr, nullptr);
	}
	else if (curr->anime.title < anime->title)
	{
		pair<Node*, Node*> temp = InsertRecursiveTitle(anime, curr->right);
		curr->right = temp.first;
		grandchild = temp.second;
		child = temp.first;
		if (child != nullptr)
		{
			child->parent = curr;
			child->sibling = curr->left;
			if (curr->left != nullptr)
			{
				curr->left->sibling = child;
			}
		}
	}
	else if (curr->anime.title > anime->title)
	{
		pair<Node*, Node*> temp = InsertRecursiveTitle(anime, curr->left);
		curr->left = temp.first;
		grandchild = temp.second;
		child = temp.first;
		if (child != nullptr)
		{
			child->parent = curr;
			child->sibling = curr->right;
			if (curr->right != nullptr)
			{
				curr->right->sibling = child;
			}
		}
	}
	else
	{
		return make_pair(nullptr, nullptr);
	}

	if (child != nullptr && child->black)
	{
		balanced = true;
		return make_pair(curr, child);
	}
	else if (child != nullptr && grandchild != nullptr && !child->black && !grandchild->black)
	{
		if (child->sibling != nullptr && !child->sibling->black)
		{
			child->black = true;
			child->sibling->black = true;
			curr->black = false;
			balanced = true;
			return make_pair(curr, child);
		}
		else
		{
			if (curr->left == child && child->left == grandchild)
			{
				pair<Node*, Node*> temp = make_pair(RotRight(curr), grandchild);
				bool color = curr->parent->black;
				curr->parent->black = curr->black;
				curr->black = color;
				balanced = true;
				return temp;
			}
			else if (curr->right == child && child->right == grandchild)
			{
				pair<Node*, Node*> temp = make_pair(RotLeft(curr), grandchild);
				bool color = curr->parent->black;
				curr->parent->black = curr->black;
				curr->black = color;
				balanced = true;
				return temp;
			}
			else if (curr->left == child && child->right == grandchild)
			{
				curr->left = RotLeft(child);
				pair<Node*, Node*> temp = make_pair(RotRight(curr), child);
				bool color = curr->parent->black;
				curr->parent->black = curr->black;
				curr->black = color;
				balanced = true;
				return temp;
			}
			else
			{
				curr->right = RotRight(child);
				pair<Node*, Node*> temp = make_pair(RotLeft(curr), child);
				bool color = curr->parent->black;
				curr->parent->black = curr->black;
				curr->black = color;
				balanced = true;
				return temp;
			}
		}
	}
	else
	{
		return make_pair(curr, child);
	}
}

Anime* RBTree::Find(string title)
{
	successfulInsert = false;
	if (sortID) 
	{
		cout << "Wrong Find function for this type of RBTree" << endl;
		return nullptr;
	}
	Anime* temp = FindRecursive(title, root);
	return temp;
}

Anime* RBTree::FindRecursive(string title, Node* curr)
{
	if (curr == nullptr)
	{
		return nullptr;
	}
	else if (title < curr->anime.title)
	{
		Anime* temp = FindRecursive(title, curr->left);
		return temp;
	}
	else if (title > curr->anime.title)
	{
		Anime* temp = FindRecursive(title, curr->right);
		return temp;
	}
	else
	{
		return &(curr->anime);
	}
}

bool RBTree::CheckValidRecursive(Node* curr) 
{
	if (curr->left == nullptr && curr->right == nullptr) 
	{
		return true;
	}
	else if (curr->right == nullptr) 
	{
		if (!curr->black && !curr->left->black)
		{
			return false;
		}
		else
		{
			return CheckValidRecursive(curr->left);
		}
	}
	else if (curr->left == nullptr) 
	{
		if (!curr->black && !curr->right->black) 
		{
			return false;
		}
		else 
		{
			return CheckValidRecursive(curr->right);
		}
	}
	else 
	{
		if ((!curr->black && !curr->left->black) || (!curr->black && !curr->right->black)) 
		{
			return false;
		}
		else 
		{
			return CheckValidRecursive(curr->left) && CheckValidRecursive(curr->right);
		}
	}
}

bool RBTree::IsBalanced() 
{
	int rootMax = 0;
	return IsHeightBalanced(root, rootMax);
}

bool RBTree::IsHeightBalanced(Node* root, int& rootMax)
{
	// Base case
	if (root == nullptr) {
		return true;
	}

	// to hold the maximum height of the left and right subtree
	int leftMax = 0, rightMax = 0;

	// proceed only if both left and right subtrees are balanced
	if (IsHeightBalanced(root->left, leftMax) &&
		IsHeightBalanced(root->right, rightMax))
	{
		// calculate the minimum and maximum height of the left and right subtree
		int rootMin = min(leftMax, rightMax) + 1;
		rootMax = max(leftMax, rightMax) + 1;

		// return true if the root node is height-balanced
		return (rootMax <= 2 * rootMin);
	}


	// return false if either left or right subtree is unbalanced
	return false;
}

void RBTree::readfiles() 
{
	fstream file("data/anime.csv");
	RBTree library(true);

	auto start = chrono::high_resolution_clock::now();

	if (file.is_open())
	{

		//reads first line and ignores it

		string buffer;

		getline(file, buffer);

		while (!file.eof())
		{

			//read it all in and decide which things to store in object later - maybe change data types of stuff that can be compared

			string id;

			getline(file, id, '\t');

			string url;

			getline(file, url, '\t');

			string title;

			getline(file, title, '\t');

			string synopsis;

			getline(file, synopsis, '\t');

			string mainPic;

			getline(file, mainPic, '\t');

			string type;

			getline(file, type, '\t');

			string source;

			getline(file, source, '\t');

			string episodes;

			getline(file, episodes, '\t');

			string status;

			getline(file, status, '\t');

			string startDate;

			getline(file, startDate, '\t');

			string endDate;

			getline(file, endDate, '\t');

			string season;

			getline(file, season, '\t');

			string studios;

			getline(file, studios, '\t');

			string genres;

			getline(file, genres, '\t');

			string score;

			getline(file, score, '\t');

			string scoreCount;

			getline(file, scoreCount, '\t');

			string scoreRank;

			getline(file, scoreRank, '\t');

			string popRank;

			getline(file, popRank, '\t');

			string memCount;

			getline(file, memCount, '\t');

			string favCount;

			getline(file, favCount, '\t');

			//computes score if it isn't in the dataset

			if (score == "")

			{

				float average = 0;

				float total = 0;

				for (int i = 10; i > 0; i--)

				{

					string buffer;

					getline(file, buffer, '\t');

					if (buffer != "")

					{

						total += stof(buffer);

						float x = stof(buffer);

						average += (x * (float)i);

					}

				}



				score = (average == 0) ? "N/A" : to_string(average / total);

			}



			//if the ID is less than 5 digits, appends 0s until it is 

			while (id.length() < 5)
			{

				id = "0" + id;

			}
			
			string upperTitle = title;
			//title = toLower(title);

			Anime temp(title, upperTitle, stoi(id), synopsis, genres, score);
			library.Insert(temp);
			this->Insert(temp);

			getline(file, buffer); //goes through useless data

		}

	}
	file.close();

	file.open("data/anime_anime.csv");
	if (file.is_open())
	{
		string buffer;
		getline(file, buffer);

		while (!file.eof())
		{
			string idA;
			string idB;
			string rec;
			string recURL;
			string recNum;
			string rel;
			string relType;
			getline(file, idA, '\t');
			getline(file, idB, '\t');
			getline(file, rec, '\t');
			getline(file, recURL, '\t');
			getline(file, rel, '\t');
			getline(file, relType, '\n');
			if (file.eof())
			{
				break;
			}


			try
			{
				int idFrom = stoi(idA);
				int idTo = stoi(idB);
				if (library.Find(idFrom) != nullptr && library.Find(idTo) != nullptr)
				{
					string title = library.Find(idFrom)->title;
					string titleRelated = library.Find(idTo)->title;
					this->Find(title)->related.push_back(this->Find(titleRelated));
				}
			}
			catch (exception error)
			{
				cout << "unsuccessful conversion" << endl;
			}
		}
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	cout << "Time to insert all anime and their relations: " << (duration.count() / 1000.0) << " seconds" << endl;
}

void RBTree::displayinfo(string userInput) 
{
	auto start = chrono::high_resolution_clock::now();

	toLower(userInput);
	if (this->Find(userInput) == nullptr)
	{
		cout << "Anime not found :(" << endl;
	}
	else
	{
		Anime userAnime = *(this->Find(userInput));
		sort((&userAnime)->related.begin(), (&userAnime)->related.end(),

			[](Anime* lhs, Anime* rhs)

			{

				return (lhs->score > rhs->score);

			});
		int counter = 0;
		for (int i = 0; i < userAnime.related.size(); i++)
		{
			if (counter >= 5)
			{
				break;
			}
			if (userAnime.id == userAnime.related.at(i)->id || recommended.find(userAnime.related.at(i)->upperTitle) != recommended.end())
			{
				continue;
			}
			cout << "Title: " << userAnime.related.at(i)->upperTitle << endl;
			cout << "Genre(s): " << userAnime.related.at(i)->genre << "   Score: " << userAnime.related.at(i)->score << endl;
			cout << "Description: " << userAnime.related.at(i)->description << endl;
			cout << endl << endl;
			recommended.insert(userAnime.related.at(i)->upperTitle);
			counter++;
		}
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	cout << "Time to find an anime and its top 5 relations: " << duration.count() / 1000.0 << " seconds" << endl;
}

string RBTree::toLower(const string& title) 
{
	string output = title;
	for (int i = 0; i < title.length(); i++) 
	{
		if (title.at(i) >= 65 && title.at(i) <= 90) 
		{
			output.at(i) = (title.at(i) + 32);
		}
	}
	return output;
}

bool RBTree::CheckValid() 
{
	return CheckValidRecursive(root);
}