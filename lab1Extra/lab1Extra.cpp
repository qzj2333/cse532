#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

enum returnValue
{
	success = 0,
	fileDoesNotExist = 1,
	numberArgumentIncorrect = 2
};

enum arg
{
	programName = 0,
	inputFile0 = 1,
	outputFile0 = 2,
	playNameStart0 = 3,
	rightNumberArgument0 = 4,
	inputFile1 = 2,
	outputFile1 = 3,
	playNameStart1 = 4,
	rightNumberArgument1 = 5,
	scrambleLoc = 1
};

int extraCredit0(string inputFileName, string outputFileName, string playName)
{
	ifstream ifs;
	ofstream ofs;
	ifs.open(inputFileName);
	ofs.open(outputFileName);
	if (ifs.is_open() && ofs.is_open())
	{
		map<string, string> characters;	// character name --> character file
		string line;
		string characterName;
		ofstream characterFile;
		int i = 1;
		size_t pos;
		bool changeCharacter = true;
		ofs << playName << endl;

		while (getline(ifs, line))
		{

			if (line.empty())	// then next line is character name
			{
				changeCharacter = true;
				continue;
			}
			if (changeCharacter)
			{
				changeCharacter = false;
				characterName = line;
				pos = line.find('.', 0);
				characterName = characterName.substr(0, pos);
				if (characters.find(characterName) == characters.end())	// name not in the map
				{
					characters.insert({ characterName, characterName + ".txt" });
				}

				// switch file
				if (characterFile.is_open())
				{
					characterFile.close();
				}
				characterFile.open(characterName + ".txt", ios_base::app);
			}
			else
			{
				characterFile << i << " " << line << endl;
				i++;
			}
		}
		if (characterFile.is_open())
		{
			characterFile.close();
		}

		for (auto character : characters)
		{
			ofs << character.first << " " << character.second << endl;
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

int extraCredit1(string inputFileName, string outputFileName, string playName)
{
	ifstream ifs;
	ofstream ofs;
	ifs.open(inputFileName);
	ofs.open(outputFileName);
	if (ifs.is_open() && ofs.is_open())
	{
		map<string, vector<string>> characters;	// charName --> vector of all lines (each line has order & text)
		string line;
		string characterName;
		int i = 1;
		size_t pos;
		bool changeCharacter = true;
		ofs << playName << endl;

		while (getline(ifs, line))
		{
			if (line.empty())	// then next line is character name
			{
				changeCharacter = true;
				continue;
			}
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
				string text = to_string(i) + " "  + line;
				characters[characterName].push_back(text);
				i++;
			}
		}
		ofstream characterFile;
		for (auto character : characters)
		{
			characterFile.open(character.first+ ".txt");
			random_shuffle(character.second.begin(), character.second.end());
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
			ofs << character.first << " " << character.first << ".txt" << endl;	// write config file
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

int main(int argc, char** argv)
{
	if (argc >= rightNumberArgument1 && string(argv[scrambleLoc]).compare("SCRAMBLE") == 0)
	{
		string pName, s;
		for (int i = playNameStart1; i < argc; i++)
		{
			s = argv[i];
			pName += s + " ";
		}
		return extraCredit1(argv[inputFile1], argv[outputFile1], pName);
	}
	else if (argc >= rightNumberArgument0)
	{
		string pName, s;
		for (int i = playNameStart0; i < argc; i++)
		{
			s = argv[i];
			pName += s + " ";
		}
		return extraCredit0(argv[inputFile0], argv[outputFile0], pName);
	}
	else
	{
		cout << "usage: " << argv[programName] << " <inputFileName> <outputFileName> <playName> or " << argv[programName] << " SCRAMBLE <inputFileName> <outputFileName> <playName>" << endl;
		return numberArgumentIncorrect;
	}
}