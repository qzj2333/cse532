#include "Player.h"

// construct a new thread
Player::Player(Play& p) : currPlay(p), end(false)
{
	thread newT([this]()
		{
			this->prepare();
		});
	t = move(newT);
	t.detach();	// new change from join to detach
};

// Get one fragment from working queue, call read to process all works for that fragment
void Player::prepare()
{
	while((!fragment_queue.empty() || (!end && !Play::end)) && !isStop()) 
	{
		unique_lock<mutex> lk(m);
		cv.wait_for(lk, 400ms, [this] 
		{
			return !this->fragment_queue.empty() || end || Play::end || isStop();
		});
		if(!this->fragment_queue.empty())
		{
			shared_ptr<Fragment> fragment = fragment_queue.front();
			fragment_queue.pop();

			lk.unlock();
			cv.notify_all();
			int result = read(fragment);
			if (result == play_end || result == stopped)
			{
				if(result == stopped)
				{
					currPlay.reset();	// reset play
				}
				cv.notify_all();
				break;
			}
		}
		else
		{
			lk.unlock();
			if(isStop())
			{
				currPlay.reset();	// reset play
			}
			cv.notify_all();
		}
	}
	if(isStop())
	{
		currPlay.reset();	// reset play
		cv.notify_all();
	}
}

// load all contents from given fragment f to data member content
// enter current fragment's character
// act all contents in current fragment
// exit current fragment's character
int Player::read(shared_ptr<Fragment>& f)
{
	if(isStop())
	{
		return stopped;
	}
	string line, first;
	size_t pos;
	int lineNum;
	content.clear();
	ifstream ifs (f->filename);
	if (ifs.is_open())
	{
		while (getline(ifs, line))
		{
			if (!line.empty())	// skip empty line
			{
				pos = line.find(' ');
				if (pos == line.npos)	// did not find white space
				{
					cerr << "badly formatted line read" << endl;
				}
				else
				{
					istringstream iss(line);
					if (iss >> lineNum && lineNum > 0)
					{
						line = line.substr(pos + 1);
						// trim the line (Source: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring)
						line.erase(line.begin(), find_if(line.begin(), line.end(), [](int ch) {
							return !isspace(ch);
							}));
						line.erase(find_if(line.rbegin(), line.rend(), [](int ch) {
							return !isspace(ch);
							}).base(), line.end());
						container c;
						c.order = lineNum;
						c.characterName = f->character_name;
						c.text = line;
						content.push_back(c);
					}
					else
					{
						cerr << "bad line number read" << endl;
					}
				}
			}
		}
		currPlay.enter(f);
		act(f);
		if(!isStop())
		{
			return currPlay.exit(f);
		}
		else
		{
			return stopped;
		}
	}
	else
	{
		cerr << f->filename << " player file can not open" << endl;
		return FileNotExist;
	}
	return success;
}

// call play's recite to display all contents in current fragment with other fragments in correct order
void Player::act(shared_ptr<Fragment>& f)
{
	vector<container>::iterator iter = content.begin();
	while(iter != content.end() && !isStop())
	{		
		currPlay.recite(iter, f->fragment_number);	// iter is increment in recite method, no need to increment iter here
	}
}

// add given fragment to the working queue
void Player::enter(shared_ptr<Fragment>& fragment)
{
	unique_lock<mutex> lk(m);
	fragment_queue.push(fragment);
	lk.unlock();
	cv.notify_all();
}

bool Player::isStop()
{
	if (futureObj.wait_for(chrono::milliseconds(0)) == future_status::timeout)
		return false;
	return true;
}

void Player::stop()
{
	exitSignal.set_value();
}