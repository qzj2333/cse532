#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
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
	inputFile = 1,
	outputFile = 2,
	playName = 3,
	rightNumberArgument = 4
};

int extraCredit(string inputFileName, string outputFileName, string playName)
{
	ifstream ifs;
	ofstream ofs;
	ifs.open(inputFileName);
	ofs.open(outputFileName);
	if (ifs.is_open() && ofs.is_open())
	{
		map<string, string> characters;
		string line;
		string characterName;
		ofstream characterFile;
		int i = 1;
		int pos;
		bool changeCharacter = true;
		ofs << playName << endl;

		while (getline(ifs,line))
		{

			if (line.empty())	// then next line is character name
			{
				changeCharacter = true;
				continue;
			}
			if(changeCharacter)
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

int main(int argc, char** argv)
{
	if (argc == rightNumberArgument)
	{
		return extraCredit(argv[inputFile], argv[outputFile], argv[playName]);
	}
	
	else
	{
		cout << "usage: " << argv[programName] << " <inputFileName> <outputFileName> <playName>" << endl;
		return numberArgumentIncorrect;
	}
}