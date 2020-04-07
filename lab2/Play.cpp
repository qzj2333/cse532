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
	scene_fragment_counter = 0;
	on_stage = 0;
	currCharacter = "";
};

void Play::recite(vector<container>::iterator it, unsigned int scene_fragment_number) // currently only print one container / line
{
	cout << "enter recite!" << scene_fragment_number  << endl;
	cout << "enter recite!!" << scene_fragment_counter << endl;
	unique_lock<mutex> lk(m);
	cout << "enter recite" << scene_fragment_number << scene_fragment_counter << endl;
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
	lk.unlock();
	it++;
	cv.notify_all();
	return;
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
	lock_guard<mutex> lk(m);
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

