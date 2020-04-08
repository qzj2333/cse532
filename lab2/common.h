// common enums and struct
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

enum returnValues // return label values
{
	success = 0,
	inputNotCorrect = 1,
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

enum arg // argument position	
{
	programName = 0,
	rightNumberArgumentLow = 2,
	rightNumberArgumentHigh = 3,
	rightNumberArgumentExtra = 4,
	script_file = 1,
	min_players_pos = 2,
	override_pos = 3
};

struct Fragment // a fragment of the scene
{
	unsigned int fragment_number = 0;
	string character_name = "";
	string filename = "";	// file contains line number of the text of the character in current fragment
};

#endif
