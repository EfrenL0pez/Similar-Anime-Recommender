#include <fstream>
#include <chrono>
#include <algorithm>
#include "RBTree.h"
#include "Anime.cpp"
#include "RBTree.cpp"


int main() {
    RBTree Test;
    Test.readfiles();
    string input;
    getline(cin, input);
    Test.displayinfo(input);

    return 0;
}