#include "Play.h"

bool container::operator<(const container& c)
{
	return order < c.order;
}

Play::Play(vector<string>& n):names(n),line_counter(1), scene_fragment_counter(0), on_stage(0), currCharacter("")
{
	it = names.begin();
	if (!names.empty())
	{
		cout << *it << endl;
		it++;
	}
	end = false;
};

void Play::recite(vector<container>::iterator& iter, unsigned int& scene_fragment_number) // currently only print one container / line
{
	unique_lock<mutex> lk(m);
	while (scene_fragment_counter < scene_fragment_number || (scene_fragment_counter == scene_fragment_number && line_counter < iter->order))
	{
		cv.wait(lk, [this, iter, scene_fragment_number] 
		{
			return ((line_counter == iter->order) && (scene_fragment_counter == scene_fragment_number)); 
		});
	}
	if (scene_fragment_counter == scene_fragment_number && line_counter == iter->order)
	{
		if (iter->characterName != currCharacter)
		{
			currCharacter = iter->characterName;
			cout << endl << currCharacter << "." << endl;
		}
		cout << iter->text << endl;
		line_counter++;	// self add
		
	}
	else if (scene_fragment_counter > scene_fragment_number || (scene_fragment_counter == scene_fragment_number && line_counter > iter->order))
	{
		cerr << "counter greater than order number " << iter->text << endl;
	}
	
	lk.unlock();
	cv.notify_all();
	iter++;
	return;
}

int Play::enter(shared_ptr<Fragment> f)
{
	unique_lock<mutex> lk(m);
	if (f->fragment_number < scene_fragment_counter)
	{
		lk.unlock();
		return enter_fail;
	}
	else if(f->fragment_number > scene_fragment_counter)
	{
		cv.wait(lk, [this, f]
		{
				return (scene_fragment_counter == f->fragment_number);
		});
	}
	cout << "[Enter " << f->character_name << ".]" << endl;
	on_stage++;
	lk.unlock();
	return success;
}

int Play::exit(shared_ptr<Fragment> f)
{
	lock_guard<mutex> lk(m);
	if (on_stage > one)
	{
		on_stage--;
		cout << "[Exit " << f->character_name << ".]" << endl;
		return success;
	}
	else if (on_stage < one)
	{
		return exit_fail;
	}
	else // on_state == one
	{
		on_stage--;
		cout << "[Exit " << f->character_name << ".]" << endl;
		scene_fragment_counter++;
		currCharacter = "";
		line_counter = 1;	// self add
		if (it != names.end())
		{
			if (it->compare("") != 0)
			{
				cout << *it << endl << endl;
			}
			it++;
		}
		cv.notify_all();
		return success;
	}
}

