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
using namespace std;

enum returnValues
{
	success = 0,
	inputNumberNotCorrect = 1,
	threadingError = 2,
	FileNotExist = 3,
	noValidInfo = 4,
	enter_fail = 5,
	exit_fail = 6
};

enum constants
{
	one = 1
};

enum arg
{
	programName = 0,
	configureFile = 1,
	rightNumberArgument = 2
};
