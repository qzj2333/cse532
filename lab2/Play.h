#if ! defined (PLAY_H)
#define PLAY_H
#include "common.h"

// should move to Player class later
struct container
{
	unsigned int order;
	string characterName;
	string text;

	bool operator<(const container& c);
};

class Play
{
private:
	mutex m;
	condition_variable cv;
	vector<string> names;
	vector<string>::iterator it;
	unsigned int line_counter;
	unsigned int scene_fragment_counter;
	unsigned int on_stage;	// # of characters currently on stage
	string currCharacter;

public:
	Play(vector<string>& n);
	void recite(vector<container>::iterator it, unsigned int scene_fragment_number);
	int enter(unsigned int scene_fragment_number);
	int exit();
};

#endif /* defined PLAY_H */