#include "Player.h"

Player::Player(Play& p) :currPlay(p) {};

void Player::read()
{
	string line;
	size_t pos;
	int lineNum;
	while (getline(*file, line))
	{
		if (!line.empty())	// skip empty line
		{
			pos = line.find(' ', 0);
			lineNum = stoi(line.substr(0, pos)); // convert string to int
			line = line.substr(pos + 1);
			// trim the line (Source: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring)
			line.erase(line.begin(), find_if(line.begin(), line.end(), [](int ch) {
				return !isspace(ch);
				}));
			line.erase(find_if(line.rbegin(), line.rend(), [](int ch) {
				return !isspace(ch);
				}).base(), line.end()); 
			if (lineNum > 0) // lineNum valid > 0 print out empty lines if has valid line number
			{
				container c;
				c.order = lineNum;
				c.characterName = name;
				c.text = line;
				content.push_back(c);
			}
			else
			{
				cerr << "badly formatted line read" << endl;
			}
		}
	}
}

void Player::act()
{
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
}

void Player::exit()
{
	if (t.joinable())
	{
		t.join();
	}
}