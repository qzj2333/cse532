#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

enum returnValue
{
	success = 0,
	fileDoesNotExist = 1,
	numberArgumentIncorrect = 2,
	inputOutputFileSameName = 3
};

enum constants
{
	enterLength = 6,
	exitLength = 5,
	endLength = 2,
	one = 1,
	config_suffix = 5
};

enum arg
{
	programName = 0,
	inputFile = 1,
	rightNumberArgument = 2
};

// if input arguments contains input file, extract information from the file and produce config files and character files
int main(int argc, char** argv)
{
	if (argc == rightNumberArgument)
	{
		string input = argv[inputFile];
		string output = "partial_" + input.substr(0, input.find_last_of('.')) + "_script.txt";

		// construct config file name
		string token;
		istringstream inputName(input);
		string configName;
		while (getline(inputName, token, '_'))
		{
			if (token.compare("act") != 0 && token.compare("scene") != 0)
			{
				configName += token + "_";
			}
		}
		configName = configName.substr(0, configName.length() - config_suffix);

		ifstream ifs;
		ifs.open(input);
		if (ifs.is_open())
		{
			ofstream ofs;
			ofs.open(output);
			map<string, vector<string>> characters;	// charName --> vector of all lines (each line has order & text)
			string line;
			string characterName;
			int i = one;
			size_t posSpace;
			size_t posDot;
			size_t pos;
			bool changeCharacter = true;
			bool readTitle = true;
			bool exit = false;
			char fragment = 'a';
			while (getline(ifs, line))
			{
				if (line.empty())	// then next line is character name
				{
					exit = false;
					changeCharacter = true;
					continue;
				}
				else if (readTitle)
				{
					exit = false;
					ofs << "[scene] " << line << endl;;
					readTitle = false;
				}
				else
				{
					if (line.substr(0, enterLength).compare("[Enter") == 0 && line.substr(line.length() - endLength).compare(".]") == 0)
					{
						continue;
					}
					if(!exit && line.substr(0, exitLength).compare("[Exit") == 0 && line.substr(line.length() - 2).compare(".]") == 0)
					{
						exit = true;
						posSpace = line.find(' ', 0);
						posDot = line.find('.', 0);
						string leavingCharacter = line.substr(posSpace + 1, posDot - posSpace - 1);
						ofstream config;
						config.open(configName + fragment + "_config.txt");
						ofstream characterFile;
						for (auto character : characters)
						{
							characterFile.open(character.first + "_" + configName + fragment + ".txt");
							if (characterFile.is_open())
							{
								for (string text : character.second)
								{
									characterFile << text << endl;
								}

							}
							else
							{
								cout << "character file not open" << endl;
							}
							characterFile.close();
							config << character.first << " " << character.first << "_" << configName << fragment << ".txt" << endl;	// write config file
						}
						ofs << configName << fragment << ".txt" << endl;
						config.close();
						characters.erase(leavingCharacter);	// delete exit character
						fragment++;
						i = one;
						// clear all texts from non-leaving characters
						characters.clear(); 
					}
					else
					{
						exit = false;
						if (changeCharacter)
						{
							changeCharacter = false;
							characterName = line;
							pos = line.find('.', 0);
							characterName = characterName.substr(0, pos);
							if (characters.find(characterName) == characters.end())	// name not in the map
							{
								vector<string> vec;
								characters.insert({ characterName, vec });
							}
						}
						else
						{
							string text = to_string(i) + " " + line;
							characters[characterName].push_back(text);
							i++;
						}
					}
				}
			
			}
			ifs.close();
			ofs.close();
			return success;
		}
		else
		{
			cout << "File can not open." << endl;
			return fileDoesNotExist;
		}

	}
	else
	{
		cout << "usage: " << argv[programName] << " <inputFileName>" << endl;
		return numberArgumentIncorrect;
	}
}