#include "Director.h"

Director::Director(string name, int min_num_players)
{
	ifstream script_file;
	script_file.open(name);
	if (script_file.is_open())
	{
		string line = "";
		size_t firstSpace = 0;
		int max_config_lines = 0;
		int curr_config_lines = 0;
		int last_config_lines = 0;
		bool last_is_scene = false;
		unsigned int fragNum = 0;
		while (getline(script_file, line))
		{
			if (!line.empty())	// need strip line?
			{
				
				firstSpace = line.find_first_of(" ");
				if (firstSpace > 0 && firstSpace < line.size())
				{
					string first = line.substr(0, firstSpace);
					if (first.compare("[scene]") == 0)
					{
						string sceneName = line.substr(firstSpace + 1);
						scene_names.push_back(sceneName);
						last_is_scene = true;
					}
				}
				else // assume config file is correct formatting: (either [scene] or txt)
				{
					ifstream config_file (line);
					string config_line = "";

					if (config_file.is_open())
					{
						while (getline(config_file, config_line))
						{
							if (!line.empty())
							{
								string cName = "";
								string fName = "";
								istringstream iss(config_line);
								if (iss >> cName && iss >> fName)
								{
									shared_ptr<Fragment> f(new Fragment);
									f->character_name = cName;
									f->filename = fName;
									f->fragment_number = fragNum;
									fragments.push_back(f);
								}
								curr_config_lines++;
							}
						}
						max_config_lines = max(max_config_lines, curr_config_lines + last_config_lines);
						last_config_lines = curr_config_lines;
						curr_config_lines = 0;

						if (!last_is_scene)
						{
							scene_names.push_back("");
						}
						last_is_scene = false;
					}
					fragNum++;
				}
			}
		}
		play = new Play(scene_names);
		int num_players = max(max_config_lines, min_num_players);
		for (int i = 0; i < num_players; i++)
		{
			shared_ptr<Player> player(new Player(*play));
			players.push_back(player);
		}
	}
	else
	{
		throw "Director Exception";
	}

}

// return success if not interrupt
//	      interrupt if interrupted
void Director::cue()	// n: nth scene
//string character_name, string file_name, int frag_num
{
	// cout scene name
	// for each fragment, enter player
	int c = 0;
	for (shared_ptr<Fragment> f : fragments)
	{
		players[c%players.size()]->enter(f);	// (try to) equally assign each fragment to different players
		c++;
	}
	if (c < players.size())
	{
		for (int i = c; i < players.size(); i++)
		{
			players[i]->end = true;
		}
	}
}