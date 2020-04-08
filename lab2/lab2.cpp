#include "Director.h"

int main(int argc, char** argv)
{
	if (argc < rightNumberArgumentLow || argc > rightNumberArgumentHigh)
	{
		cout << "usage: " << argv[programName] << " <configuration_file_name> || " <<  argv[programName] << " <configuration_file_name> <min_num_players>" << endl;
		return inputNumberNotCorrect;
	}
	if (argc == rightNumberArgumentLow)
	{
		Director d(argv[script_file]);
		d.cue();
	}
	else if (argc == rightNumberArgumentHigh)
	{
		unsigned int min_players;
		istringstream iss(argv[min_players_pos]);
		if (iss >> min_players)
		{
			Director d(argv[script_file], min_players);
			d.cue();
		}
		else
		{
			cout << "usage: " << argv[programName] << " <configuration_file_name> || " << argv[programName] << " <configuration_file_name> <min_num_players>" << endl;
			return inputNumberNotCorrect;
		}
	}
	
	return success;
}