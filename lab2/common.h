#if ! defined (COMMON_H)
#define COMMON_H
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
#include <cctype>
#include <locale>
#include <queue>
#include <future>
using namespace std;

enum returnValues
{
	success = 0,
	inputNumberNotCorrect = 1,
	threadingError = 2,
	FileNotExist = 3,
	noValidInfo = 4,
	enter_fail = 5,
	exit_fail = 6,
	play_end = 7
};

enum constants
{
	one = 1
};

enum arg
{
	programName = 0,
	rightNumberArgumentLow = 2,
	rightNumberArgumentHigh = 3,
	script_file = 1,
	min_players_pos = 2
};

struct Fragment
{
	unsigned int fragment_number = 0;
	string character_name = "";
	string filename = "";
};

struct Scene
{
	vector<shared_ptr<Fragment>> fragments;
	string name = "";
};

#endif
