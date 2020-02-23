#include "Play.h"

bool container::operator<(const container& c)
{
	return order < c.order;
}

Play::Play(string n)
{
	name = n;
	counter = 1;
	currCharacter = "";
};

/*play* play::operator<<(container c)
{
	lock_guard<mutex> guard(m);
	content.push_back(c);
	return this;
}

void play::print(ostream& o)
{
	//lock_guard<mutex> guard(m);	// no need to lock because print is called after all threads are joined
	sort(content.begin(), content.end());
	string currCharacter;
	for (container c : content)
	{
		if (currCharacter == "" || currCharacter.compare(c.characterName) != 0) // if change character or beginning of the play
		{
			currCharacter = c.characterName;
			o << "\n" << c.characterName << "." << endl;
		}
		o << c.text << endl;
	}
}*/

void Play::recite(vector<container>::iterator it) // currently only print one container / line
{
	unique_lock<mutex> lk(m);

	if (counter > it->order)
	{
		cerr << "counter greater than iterator order number" << endl;
		it++;
	}
	else
	{
		cv.wait(lk, [this, it] {return counter == it->order; });
		if (currCharacter == "" || currCharacter.compare(it->characterName) != 0) // if change character or beginning of the play
		{
			currCharacter = it->characterName;
			cout << "\n" << it->characterName << "." << endl;
		}

		cout << it->text << endl;
		it++;
		counter++;
	}
	lk.unlock();	// order correct?????????????????????????????
	cv.notify_all();
}

/*void func(Play& p, const string& name, shared_ptr<ifstream> part)
{
	string line;
	size_t pos;
	unsigned int lineNum;

	while (getline(*part.get(), line))
	{
		if (!line.empty())	// skip empty line
		{
			pos = line.find(' ', 0);
			lineNum = stoi(line.substr(0, pos)); // convert string to int
			line = line.substr(pos + 1);
			if (lineNum > 0 && !line.empty()) // lineNum valid > 0
			{
				container c;
				c.order = lineNum;
				c.characterName = name;
				c.text = line;
				p << c;
			}
		}
	}
	part.get()->close();
	// no need to delete part pointer since shared_ptr will delete automatically
}*/

