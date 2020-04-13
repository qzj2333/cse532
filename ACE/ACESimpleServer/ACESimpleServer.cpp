#include <iostream>
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"
using namespace std;

// 2-5
class Event: public ACE_Event_Handler
{
	ACE_INET_Addr* server;
	ACE_SOCK_Stream stream;
	ACE_SOCK_Acceptor acceptor;
public:
	Event()
	{
		server = new ACE_INET_Addr(1025, ACE_LOCALHOST);
		int call = acceptor.open(*server, 1);
		if(call >= 0)
		{
			ACE_TCHAR address[INET6_ADDRSTRLEN];
			server->addr_to_string(address, sizeof(address));
			ACE_OS::printf("%s\n", address);
		}
	};
	
	virtual int handle_input (ACE_HANDLE h = ACE_INVALID_HANDLE)
	{
		int connect = acceptor.accept(stream);
		if(connect>=0)
		{
			char msg[BUFSIZ];
			while(stream.recv(msg, sizeof(msg)) > 0)
			{
				cout << msg << endl;
			}
			stream.close();
			cout << "client.leave" << endl;
			return 0;
		}
		else
		{
			cerr << "Fail to connect" << endl;
			return 1;
		}
	};
	
	virtual ACE_HANDLE get_handle() const
	{
		return acceptor.get_handle();
	};
};

int main(int argc, char* argv[])
{
	if(argc > 1)
	{
		cerr << "has command line arguments" << endl;
		return 1;
	}
	else
	{
		// 2
		//cout << argv[0] << endl;
		
		// 4
		/*ACE_INET_Addr server(1025, ACE_LOCALHOST);
		ACE_SOCK_Stream stream;
		ACE_SOCK_Acceptor acceptor;
		int call = acceptor.open(server, 1);
		
		if (call >= 0) 	// call success
		{
			ACE_TCHAR address[INET6_ADDRSTRLEN];
			server.addr_to_string(address, sizeof(address));
			ACE_OS::printf("%s\n", address);
			int connect = acceptor.accept(e.stream);
			while(connect >= 0)
			{
				char msg[BUFSIZ];
				while(stream.recv(msg, sizeof(msg)) > 0)
				{
					cout << msg << endl;
				}
				stream.close();
				cout << "client.leave" << endl;
				connect = acceptor.accept(e.stream);
			}
		}	
		else
		{
			cerr << "Fail to connect" << endl;
		}*/
		// 2-5
		Event e;
		/*while(true)
		{
			e.handle_input();
		}*/
		
		// 2-6
		ACE_Reactor::instance()->register_handler(&e, ACE_Event_Handler::ACCEPT_MASK);
		ACE_Reactor::instance()->run_reactor_event_loop();
	}
	return 0;
}
