#include "Producer.h"

Producer::Producer()
{
	global_last_id = 0;
	server = new ACE_INET_Addr(1025, ACE_LOCALHOST);
	int call = acceptor.open(*server, 1);
	reactor(ACE_Reactor::instance());
	if (call >= 0) 	// call success
	{
		// print ip address
		ACE_TCHAR address[INET6_ADDRSTRLEN];
		server->addr_to_string(address, sizeof(address));
		ACE_OS::printf("%s\n", address);
		// register events
		this->reactor()->register_handler(SIGINT, this);
		this->reactor()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK);
		this->reactor()->register_handler(ACE_STDIN, this, ACE_Event_Handler::READ_MASK);
		this->reactor()->run_reactor_event_loop();	
	}	
	else
	{
		cerr << "Fail to connect" << endl;
	}
}

void Producer::refresh_list()
{
	cout << "----- play list -----" << endl;
	for(shared_ptr<connection> c: connections)
	{
		for(auto p=c->plays.begin(); p != c->plays.end(); p++)
		{
			if(c->currentRunID != notRunning)
			{
				if(c->currentRunID == p->first)	// director is playing this play
				{
					cout << p->first << " " << p->second << " in progress" << endl; 
				}
				else	// director is playing other play
				{
					cout << p->first << " " << p->second << " unavailable" << endl; 
				}
			}
			else	// director is playing
			{
				cout << p->first << " " << p->second << " available" << endl; 
			}
		}
	}
	cout << "----- list end -----" << endl;
}


int Producer::handle_signal(int signum, siginfo_t*,ucontext_t*)	// get rid of virtual and change last parameter from siginfo_t to ucontext_t
{
	// send to corresponding Director
	string msg = "quit";
	for(shared_ptr<connection> c: connections)
	{
		c->connector->connect(*c->stream, *c->server);
		c->stream->send_n(msg.c_str(), (int)msg.size());
		c->stream->close();
	}
	this->reactor ()->end_reactor_event_loop();
	this->reactor ()->close();
	return success;
};

int Producer::handle_input(ACE_HANDLE h)
{
	if(h==ACE_STDIN)	// for user input
	{
		string user_input;
		getline(cin, user_input);
		istringstream iss(user_input);
		string action;
		int number = -1;
		string extra;
		if(iss >> action)
		{
			if(iss >> number)
			{
				if(iss >> extra)
				{
					cerr << "Invalid Input" << endl;
					return invalid_input;
				}
				if(action.compare("start") != 0 && action.compare("stop") != 0)
				{
					cerr << "No such action." << endl;
					return invalid_input;
				}
				else
				{
					bool find = false;
					for(shared_ptr<connection> c: connections)
					{
						for(auto p=c->plays.begin(); p != c->plays.end(); p++)
						{
							if(p->first == number)
							{
								find = true;
						
								if(action.compare("start") == 0)
								{
									if(c->currentRunID != notRunning)
									{
										cerr << "current director is running." << endl;
										return invalid_input;
									}
									else
									{
										c->currentRunID = number;
										// send to corresponding director
										string msg = "start " + to_string(number-c->factor);
										c->connector->connect(*c->stream, *c->server);
										c->stream->send_n(msg.c_str(), (int)msg.size());
										c->stream->close();
									}
								}
								else	// if(action.compare("stop") == 0)
								{
									if(c->currentRunID != number)
									{
										cerr << "current play is not in progress." << endl;
										return invalid_input;
									}
									else
									{
										c->currentRunID = notRunning;
										// send to corresponding Director with play's director ID
										string msg = "stop " + to_string(number-c->factor);
										c->connector->connect(*c->stream, *c->server);
										c->stream->send_n(msg.c_str(), (int)msg.size());
										c->stream->close();
									}
								}
								break;
							}
						}
					}
					if(!find)
					{
						cerr << "id not valid" << endl;
						return invalid_input;
					}
				}
			}
			else if(action.compare("quit") == 0)
			{
				// send to corresponding Director
				string msg = "quit";
				for(shared_ptr<connection> c: connections)
				{
					c->connector->connect(*c->stream, *c->server);
					c->stream->send_n(msg.c_str(), (int)msg.size());
					c->stream->close();
				}
				handle_close(h, ACE_Reactor::CLR_MASK);
				return success;
			}
		}
		refresh_list();
		return success;
	}
	else	// for director
	{
		if(acceptor.accept(stream)>=0)
		{
			char msg[BUFSIZ];
			while(stream.recv(msg, sizeof(msg)) > 0)
			{
				cout << "receive " << msg << endl;
				
				string message = string(msg);
				int directorID;
				istringstream checkID(message);
				if(checkID >> directorID)
				{
					cout << directorID << endl;
					// find corresponding connection
					for(size_t i = 0; i < connections.size(); i++)
					{
						if(connections[i]->factor == directorID)
						{
							connections.erase(connections.begin()+i);
							refresh_list();
							return success;
						}
					}
				}
				else
				{
					istringstream checkEnd(message);
					string end_key_word;
					if(checkEnd >> end_key_word && end_key_word.compare("end") == 0)
					{
						int playID;
						int directorID;
						if(checkEnd >> playID && checkEnd >> directorID)
						{
							// find corresponding connection
							for(size_t i = 0; i < connections.size(); i++)
							{
								// set director available
								if(connections[i]->factor == directorID)
								{
									connections[i]->currentRunID = notRunning;
								}
							}
						}
						else
						{
							cerr << "invalid end message receiving" << endl;
						}
					}
					else
					{
						istringstream iss(message);
						// make new connection
						string play_name;
						shared_ptr<connection> c(new connection());
						c->factor = global_last_id;
						while(iss >> play_name)
						{
							if(play_name.compare(";") == 0)	// get address
							{
								string a;
								iss >> a;
								ACE_INET_Addr* newServer = new ACE_INET_Addr();
								newServer->string_to_addr(a.c_str());
						
								ACE_SOCK_Stream* newStream = new ACE_SOCK_Stream();
								ACE_SOCK_Connector* connector = new ACE_SOCK_Connector();
								int call = connector->connect(*newStream, *newServer);
								while(call < 0)
								{
									call = connector->connect(*newStream, *newServer);
								}							
								c->stream = newStream;
								c->server = newServer;
								c->connector = connector;
								// clear msg buffer
								for(int i = 0; i < BUFSIZ; i++)
								{
									msg[i] = '\0';
								}
								// send back its global id
								string msg = to_string(c->factor);
								c->stream->send_n(msg.c_str(), (int)msg.size());
								c->stream->close();
							}
							else
							{
								c->plays.insert({global_last_id, play_name});
								global_last_id++;
							}
						}
						connections.push_back(c);
					}
				}
			}
			refresh_list();
			return success;
		}
		cerr << "Fail to connect" << endl;
		return fail_connect;
	}
}

int Producer::handle_close (ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
	this->reactor ()->end_reactor_event_loop();
	this->reactor ()->close();
	return success;
};

ACE_HANDLE Producer::get_handle() const
{
	return acceptor.get_handle();
};

int main(int argc, char* argv[])
{
	
	if(argc > correct_num_args)
	{
		cout << "usage: " << argv[0] << "[port]" << endl;
		return inputNotCorrect;
	}
	else
	{
		Producer p;
		/*ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> timer;
		timer.activate();
		ACE_Time_Value intervalTime(3, 0);
		ACE_Time_Value delayTime (0,0);
	
		ACE_Reactor::instance()->schedule_timer(&p, 0, delayTime, intervalTime);	// handle user input every 3s
		*/
		//ACE_Reactor::instance()->register_handler(SIGINT, &p);	// handle ctrl-C!!!!!!!!!!!!
		
		ACE_Reactor::instance()->run_event_loop();
		return success;
	}
	
}