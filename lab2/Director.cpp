#include "Director.h"

Director::Director(string name, int min_num_players)
{
	ifstream script_file;
	script_file.open(name);
	if (script_file.is_open())
	{
		string line;
		size_t firstSpace;
		int max_config_lines = 0;
		int curr_config_lines = 0;
		int last_config_lines = 0;
		bool last_is_scene = false;
		while (getline(script_file, line))
		{
			if (!line.empty())	// need strip line?
			{
				firstSpace = line.find_first_of(" ");
				if (firstSpace > 0 && firstSpace < line.size())
				{
					string first = line.substr(0, firstSpace - 1);
					if (first.compare("[scene]") == 0)
					{
						string sceneName = line.substr(firstSpace + 1);
						scene_names.push_back(sceneName);
						last_is_scene = true;
					}
				}
				else if (firstSpace == line.size())
				{
					ifstream config_file;
					config_file.open(line);
					string config_line;
					if (config_file.is_open())
					{
						while (getline(config_file, config_line))
						{
							if (!line.empty())
							{
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
				}
			}
		}
		shared_ptr<Play> play(new Play(scene_names));
		int num_players = max(max_config_lines, min_num_players);
		for (int i = 0; i < num_players; i++)
		{
			shared_ptr<Player> player(new Player(play));
			players.push_back(player);
		}
	}
	else
	{
		throw "Director Exception";
	}

}

void Director::cue(string character_name, string file_name, int frag_num)
{
	for (shared_ptr<Player> p : players)
	{
		p->enter();
	}
}