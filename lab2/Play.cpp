#include "Play.h"

bool container::operator<(const container& c)
{
	return order < c.order;
}

Play::Play(vector<string>& n)
{
	names = n;

	it = names.begin();
	if (!names.empty())
	{
		cout << *it << endl;
		it++;
	}
	line_counter = 1;
	scene_fragment_counter = 1;	// ?????????? 1 or 0 ?????????
	on_stage = 0;
	currCharacter = "";
};

void Play::recite(vector<container>::iterator it, unsigned int scene_fragment_number) // currently only print one container / line
{
	unique_lock<mutex> lk(m);
	while (scene_fragment_counter < scene_fragment_number || (scene_fragment_counter == scene_fragment_number && line_counter < it->order))
	{
		cv.wait(lk, [this, it, scene_fragment_number] 
		{
			return ((line_counter == it->order) && (scene_fragment_counter == scene_fragment_number)); 
		});
	}
	if (scene_fragment_counter == scene_fragment_number && line_counter == it->order)
	{
		cout << it->text << endl;
	}
	else if (scene_fragment_counter > scene_fragment_number || (scene_fragment_counter == scene_fragment_number && line_counter > it->order))
	{
		cerr << "counter greater than order number" << endl;
	}
	it++;
	lk.unlock();
	cv.notify_all();
	return;


	/*if (line_counter > it->order)
	{
		cerr << "counter greater than iterator order number" << endl;
		it++;
	}
	else
	{
		
		if (currCharacter == "" || currCharacter.compare(it->characterName) != 0) // if change character or beginning of the play
		{
			currCharacter = it->characterName;
			cout << "\n" << it->characterName << "." << endl;
		}

		cout << it->text << endl;
		it++;
		line_counter++;
	}
	lk.unlock();
	cv.notify_all();*/
}

int Play::enter(unsigned int scene_fragment_number)
{
	unique_lock<mutex> lk(m);
	if (scene_fragment_number < scene_fragment_counter)
	{
		lk.unlock();
		return enter_fail;
	}
	else if(scene_fragment_number > scene_fragment_counter)
	{
		cv.wait(lk, [this, scene_fragment_number]
		{
				return (scene_fragment_counter == scene_fragment_number);
		});
	}
	on_stage++;
	lk.unlock();
	return success;
}

int Play::exit()
{
	if (on_stage > one)
	{
		on_stage--;
		return success;
	}
	else if (on_stage < one)
	{
		return exit_fail;
	}
	else // on_state == one
	{
		on_stage--;
		scene_fragment_counter++;
		if (it != names.end())
		{
			if (it->compare("") != 0)
			{
				cout << *it << endl;
			}
			it++;
		}
		cv.notify_all();
		return success;
	}
}

