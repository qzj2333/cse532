#include "Player.h"

Player::Player(Play& p, const string& charName, shared_ptr<ifstream> inputFile) :currPlay(p), name(charName), file(inputFile) {};

void Player::read()
{
	string line;
	int pos, lineNum;
	//cout << "fOpen: " << file->is_open() << endl;
	while (getline(*file, line))
	{
		//cout <<"line"<< line << endl;
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
				//cout << "order: " << lineNum << " charName: " << name << " text: " << line << endl;
				content.push_back(c);
			}
		}
	}
}

void Player::act()
{
	//sort(content.begin(), content.end());
	for (auto iter = content.begin(); iter != content.end(); ++iter)
	{
		currPlay.recite(iter);
	}
}

void Player::enter()
{
	thread newT([this]()
		{
			read();
			act();
		});
	t = move(newT);
	//newT.detach();
}

void Player::exit()
{
	if (t.joinable())
	{
		t.join();
	}
}