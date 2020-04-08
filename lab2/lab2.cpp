#include "Director.h"

// main method of lab 2
int main(int argc, char** argv)
{
	// user only specified script file
	if (argc == rightNumberArgumentLow)
	{
		try
		{
			Director d(argv[script_file]);
			d.cue();
		}
		catch (...)
		{
			cerr << "Can not open script file" << endl;
			return FileNotExist;
		}
	}
	// user specified script file and min. number of players
	else if (argc == rightNumberArgumentHigh)
	{
		unsigned int min_players;
		istringstream iss(argv[min_players_pos]);
		if (iss >> min_players)
		{
			try
			{
				Director d(argv[script_file], min_players);
				d.cue();
			}
			catch (...)
			{
				cerr << "Can not open script file" << endl;
				return FileNotExist;
			}
		}
		else
		{
			cout << "usage: " << argv[programName] << " <configuration_file_name> || " << argv[programName] << " <configuration_file_name> <min_num_players> || " << argv[programName] << " <configuration_file_name> <num_players> -override " << endl;
			return inputNotCorrect;
		}
	}
	// extra credit: user specified script file and number of players
	else if(argc == rightNumberArgumentExtra && string(argv[override_pos]).compare("-override") == 0)
	{
		unsigned int num_players;
		istringstream iss(argv[min_players_pos]);
		if (iss >> num_players)
		{
			try
			{
				Director d(argv[script_file], num_players, true);
				d.cue();
			}
			catch (...)
			{
				cerr << "Can not open script file" << endl;
				return FileNotExist;
			}
		}
		else
		{
			cout << "usage: " << argv[programName] << " <configuration_file_name> || " << argv[programName] << " <configuration_file_name> <min_num_players> || " << argv[programName] << " <configuration_file_name> <num_players> -override " << endl;
			return inputNotCorrect;
		}
	}
	// wrong input
	else
	{
		cout << "usage: " << argv[programName] << " <configuration_file_name> || " << argv[programName] << " <configuration_file_name> <min_num_players> || " << argv[programName] << " <configuration_file_name> <num_players> -override " << endl;
		return inputNotCorrect;
	}
	
	return success;
}