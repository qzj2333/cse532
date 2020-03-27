#include "Player.h"

int main(int argc, char** argv)
{
	if (argc != rightNumberArgument)
	{
		cout << "usage: " << argv[programName] << " <configuration_file_name>" << endl;
		return inputNumberNotCorrect;
	}
	fstream configFile;
	configFile.open(argv[configureFile]);
	if (configFile.is_open())
	{
		string line;
		bool valid = false;
		// get first non-blank line as play name
		while (getline(configFile, line) && line.empty())
		{
			continue;
		}
		string playName = line;
		Play p(playName);
		vector<Player> plays;
		while (getline(configFile, line))
		{
			if (!line.empty())
			{
				string cName;
				string fName;
				istringstream iss(line);
				if (iss >> cName && iss >> fName)
				{
					cout << fName << endl;
					ifstream* characterFile = new ifstream(fName);
					// dynamic allocation for different character file ifstream
					shared_ptr<ifstream> pFile(characterFile);
					if (pFile->is_open())
					{
						valid = true;
						plays.emplace_back(Player(p, cName, pFile));

					}
					else
					{
						cerr << "Character file can not be opened." << endl;
					}
				}
				else
				{
					cerr << "badly formatted line." << endl;
				}
			}
		}

		if (!valid)
		{
			cout << "No valid character information extracted" << endl;
			return noValidInfo;
		}
		for (Player& p : plays)
		{
			p.enter();
		}
		for (Player& p : plays)
		{
			p.exit();
		}
	}
	else
	{
		cout << "Configure File can not be opened." << endl;
		return FileNotExist;
	}
	configFile.close();
	return success;
}