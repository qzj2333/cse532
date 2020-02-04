#include "lab0.h"

struct container
{
	unsigned int order;
	string characterName;
	string text;

	bool operator<(const container& c) 
	{
		return order < c.order;
	}
};

class play
{
public:
	mutex m;
	string name;
	vector<container> content;

	play(string n)
	{
		name = n;
	};

	play* operator<<(container c)
	{
		lock_guard<mutex> guard(m);
		content.push_back(c);
		return this;
	}

	void print(ostream& o)
	{
		//lock_guard<mutex> guard(m);
		sort(content.begin(), content.end());
		string currCharacter;
		for (container c : content)
		{
			if (currCharacter == "" || currCharacter.compare(c.characterName) != 0) // if change character or beginning of the play
			{
				o << "\n" << c.characterName << "." << endl;
			}
			o << c.text << endl; 
		}	
	}
};

void func(play& p, const string& name, ifstream& part)
{
	string line;
	size_t pos;
	unsigned int lineNum;
	cout << "in thread func before while" << endl;
	if (part.is_open())
	{
		cout << "part is open!" << endl;
		while (getline(part, line))
		{
			cout << "readline: " << line << endl;
			if (!line.empty())	// skip empty line
			{
				pos = line.find(' ', 0);
				lineNum = stoi(line.substr(0, pos)); // convert string to int
				line = line.substr(pos + 1);
				if (lineNum > 0 && !line.empty()) // lineNum valid > 0 ?????????????
				{
					container c;
					c.order = lineNum;
					c.characterName = name;
					c.text = line;
					p << c;
				}
			}
		}
	}
	else
	{
		cout << "ifs not open" << endl;
	}
}

int main(int argc, char** argv)
{
	if (argc != rightNumberArgument)
	{
		cout << "usage: " << argv[programName] << " <configuration_file_name>" << endl;
		return 1;
	}
	fstream configFile;
	configFile.open(argv[configureFile]);
	if (configFile.is_open())
	{
		string line;
		// get first non-blank line as play name
		while (getline(configFile,line) && line.empty())
		{
			continue;
		}
		string playName = line;
		play p(playName);
		vector<thread> threads;
		while (getline(configFile, line))
		{
			if (!line.empty())
			{
				cout << "open: " << line << endl;
				size_t pos = line.find(' ', 0);
				string characterName = line.substr(0, pos);
				string fName = line.substr(pos + 1);
				ifstream characterFile;
				characterFile.open(fName);
				if (characterFile.is_open())
				{
					cout << "starting a thread" << endl;
					threads.emplace_back(thread(func, ref(p), ref(characterName), ref(characterFile)));
					//characterFile.close();
				}
			}
		}
		cout << "start join!" << endl;
		for (thread &t : threads)
		{
			t.join();
		}
		p.print(cout);
	}

	configFile.close();
	return 0;
}