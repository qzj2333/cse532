#include "Player.h"

bool Player::end = false;

Player::Player(Play& p) :currPlay(p) 
{
	thread newT([this]()
		{
			prepare();
		});
	t = move(newT);
	
};

Player::~Player()
{
	if (t.joinable())
	{
		cout << "join!!!!!!!! " << endl;
		t.join();
	}
}

void Player::prepare()
{
	while (!fragment_queue.empty() || !end) {
		unique_lock<mutex> lk(m);
		cv.wait(lk, [this] 
		{
			return !this->fragment_queue.empty();
		});
		shared_ptr<Fragment> fragment = fragment_queue.front();
		fragment_queue.pop();
		if (fragment_queue.empty())
		{
			Player::end = true;
		}
		lk.unlock();
		read(fragment);
	}
}

void Player::read(shared_ptr<Fragment>& f)
{
	string line;
	size_t pos;
	unsigned int lineNum;
	content.empty();
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
	sort(content.begin(), content.end());
	cout << "ready to act!" << f->fragment_number << " " << content.size() << endl;
	act(f->fragment_number);
	currPlay.exit();
}

void Player::act(unsigned int frag_num)
{
	for (vector<container>::iterator iter = content.begin(); iter != content.end(); ++iter)
	{
		currPlay.recite(iter, frag_num);
	}
}

void Player::enter(shared_ptr<Fragment>& fragment)
{
	unique_lock<mutex> lk(m);
	fragment_queue.push(fragment);
	lk.unlock();
	cv.notify_all();
}

void Player::exit()
{
	unique_lock<mutex> lk(m);
	while (!fragment_queue.empty())
	{
		fragment_queue.pop();
	}
	lk.unlock();
	cv.notify_all();
}