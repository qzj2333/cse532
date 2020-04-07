#include "Player.h"

class Director
{
private:
	vector<string> scene_names;
	vector<shared_ptr<Fragment>> fragments;
	vector<shared_ptr<Player>> players;
public:
	Director(string name, int min_num_players = 0);
	void cue();
};
