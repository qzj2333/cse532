// header file of director class

#if ! defined (DIRECTOR_H)
#define DIRECTOR_H
#include "Player.h"

class Director
{
private:
	vector<string> scene_names;	// names of all scenes in script file
	vector<shared_ptr<Fragment>> fragments;	// all fragments
	vector<shared_ptr<Player>> players;	// all players
	
public:
	Play* play;	// the play of current script file
	Director(string name, int min_num_players = 0, bool flag = false);
	void cue();
};

#endif
