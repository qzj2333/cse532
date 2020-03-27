#include "Player.h"

class Director
{
private:
	vector<string> scene_names;
	vector<shared_ptr<Player>> players;
	vector<queue<ifstream, string>> player_queues;
public:
	Director(string name, int min_num_players = 0);
	void cue(string character_name, string file_name, int frag_num);
};
