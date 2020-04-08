#include "Player.h"

Player::Player(Play& p) :currPlay(p), end(false)
{
	thread newT([this]()
		{
			this->prepare();
		});
	t = move(newT);
};

Player::~Player()
{
	cv.notify_all();
	if (t.joinable())
	{
		t.join();
	}
}

void Player::prepare()
{
	while (!fragment_queue.empty()) {
		unique_lock<mutex> lk(m);
		
		cv.wait(lk, [this] 
		{
				return !this->fragment_queue.empty() || end;
		});
		if (end)
		{
			return;
		}
		shared_ptr<Fragment> fragment = fragment_queue.front();
		fragment_queue.pop();
		lk.unlock();
		cv.notify_all();
		read(fragment);
	}
}

void Player::read(shared_ptr<Fragment>& f)
{
	string line;
	size_t pos;
	unsigned int lineNum;
	content.clear();
	ifstream ifs (f->filename);
	if (ifs.is_open())
	{
		while (getline(ifs, line))
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
					c.characterName = f->character_name;
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
	currPlay.enter(f);
	act(f);
	currPlay.exit(f);
}

void Player::act(shared_ptr<Fragment>& f)
{
	vector<container>::iterator iter = content.begin();
	while(iter != content.end())
	{		
		currPlay.recite(iter, f->fragment_number);
		// iter is increment in recite method, no need to increment iter here
	}
}

void Player::enter(shared_ptr<Fragment>& fragment)
{
	unique_lock<mutex> lk(m);
	fragment_queue.push(fragment);
	lk.unlock();
	cv.notify_all();
}