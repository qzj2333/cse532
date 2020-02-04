#pragma once
#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <mutex>

using namespace std;

enum returnValues
{
	success = 0,
	inputNumberNotCorrect = 1
};

enum arg
{
	programName = 0,
	configureFile = 1,
	rightNumberArgument = 2
};

