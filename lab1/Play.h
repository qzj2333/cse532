#if ! defined (PLAY_H)
#define PLAY_H

#pragma once
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <mutex>
#include <memory>
#include <condition_variable>

using namespace std;

enum returnValues
{
	success = 0,
	inputNumberNotCorrect = 1,
	threadingError = 2,
	FileNotExist = 3,
	noValidInfo = 4
};

enum arg
{
	programName = 0,
	configureFile = 1,
	rightNumberArgument = 2
};

// should move to Player class later
struct container
{
	unsigned int order;
	string characterName;
	string text;

	bool operator<(const container& c);
};

class Play
{
private:
	mutex m;
	condition_variable cv;
	string name;
	int counter;
	string currCharacter;
public:
	Play(string n);
	//play* operator<<(container c);
	//void print(ostream& o);
	void recite(vector<container>::iterator it);
};

#endif /* defined PLAY_H */