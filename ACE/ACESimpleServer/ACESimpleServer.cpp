#include "../Interceptor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"

class Event_Reading: public ACE_Event_Handler
{
	bool flag; // 3-5
	Interceptor* interceptor;	// 4-2
public:
	ACE_SOCK_Stream* stream;  // ! dynamic allocating in set method
	char msg[BUFSIZ];
	Event_Reading(ACE_Reactor* react, ACE_SOCK_Stream* s)
	{
		reactor(react);
		stream = s;
		flag = true;
		interceptor = nullptr;	// 4-2
		reactor()->register_handler(this, ACE_Event_Handler::READ_MASK);
	}
	
	virtual ~Event_Reading()
	{
		flag = false;
		delete stream;
		cout << "destructor of event_reading" << endl;
	}
	// set stream method
	
	virtual ACE_HANDLE get_handle() const
	{
		return stream->get_handle();
	};
	
	virtual int handle_input (ACE_HANDLE h = ACE_INVALID_HANDLE)
	{
		//if recv < 0: "done reading"; call remove handler, and close stream
		while(stream->recv(msg, sizeof(msg)) > 0)
		{
			// 4-3
			if(interceptor != nullptr)
			{
				interceptor->operator()(*msg);
				cout << msg << endl;
			}
			
		}
		return 0;
	};
	
	virtual int handle_close (ACE_HANDLE handle, ACE_Reactor_Mask mask)
	{
		// delete socket & read _event itself
		cout << "ReadEvent::handle_close called " << endl;
		if(mask == ACE_Event_Handler::READ_MASK)
		{
			cout << "has read mask" << endl;
		}
		stream->close();
		if(flag)
		{
			delete this;
			return 1;
		}
		else
		{
			return 0;
		}
		
	};
	
	// 4-3
	void set_interceptor(Interceptor* i)
	{
		interceptor = i;
	};
};

// 2-5
class Event: public ACE_Event_Handler
{
	ACE_INET_Addr* server;
	ACE_SOCK_Stream* stream;
	ACE_SOCK_Acceptor acceptor;
	// bool isDelete;
	Interceptor* interceptor;	// 4-3
public:
	Event_Reading* read;
	Event(ACE_Reactor* react)
	{
		reactor(react);
		//read = re;
		//isDelete = false;
		interceptor = new Rotate(-2);	// 4-3
		interceptor->setNext(new Subtract(1));	// 4-6
		
		server = new ACE_INET_Addr(1027, ACE_LOCALHOST);
		int call = acceptor.open(*server, 1);
		if(call >= 0)
		{
			ACE_TCHAR address[INET6_ADDRSTRLEN];
			server->addr_to_string(address, sizeof(address));
			ACE_OS::printf("%s\n", address);
			this->reactor()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK);	// 2-6
			this->reactor()->register_handler(SIGINT, this);
			this->reactor()->run_reactor_event_loop();	
		}
		else
		{
			ACE_OS::printf("open port fail");
		}
	};
	
	virtual int handle_input (ACE_HANDLE h = ACE_INVALID_HANDLE)
	{
		stream = new ACE_SOCK_Stream();	// 3-6
		if(acceptor.accept (*stream)>=0)
		{
			read = new Event_Reading(this->reactor(), stream);	// 3-6
			read->set_interceptor(interceptor);
			read->handle_input();
			read->handle_close(ACE_INVALID_HANDLE, 0);	// 3-6
			return 0;
		}
		else
		{
			cerr << "Fail to connect" << endl;
			delete stream;
			return 1;
		}
		
	};
	
	virtual ACE_HANDLE get_handle() const
	{
		return acceptor.get_handle();
	};
	
	//virtual int handle_signal (int sig, siginfo_t* = 0, ucontext_t*)
	int handle_signal(int signum, siginfo_t*,ucontext_t*)	// get rid of virtual and change last parameter from siginfo_t to ucontext_t
	{
		cout << "handle signal" << endl;
		this->reactor ()->end_reactor_event_loop();
		this->reactor ()->close();
		acceptor.close();
		return 0;
	};
	
	// 3-2
	virtual int handle_close (ACE_HANDLE handle, ACE_Reactor_Mask mask)
	{
		// 3-5
		/*if(!isDelete)
		{
			int result = read->handle_close(ACE_INVALID_HANDLE, 0);
			if(result != 0)
			{
				isDelete = true;
			}
		}*/
		if(mask == ACE_Event_Handler::ACCEPT_MASK || mask == ACE_Event_Handler::SIGNAL_MASK)
		{
			cout << "Event::handle_close called mask match" << endl;
			
		}
		else
		{
			cout << "Event::handle_close called mask not match" << endl;;
		}
		this->reactor ()->end_reactor_event_loop();
		this->reactor ()->close();
		return 0;
	};
	
	// 4-3
	/*void set_interceptor(Interceptor* i)
	{
		interceptor = i;
	};*/
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
		/*ACE_INET_Addr server(1029, ACE_LOCALHOST);
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
		ACE_Reactor* react = ACE_Reactor::instance();
		// Event_Reading* read = new Event_Reading(react);
		Event e(react);
		//e.reactor()->handle_events();
		/*while(true)
		{
			e.handle_input();
		}*/
		
		//ACE_Reactor::instance()->register_handler(&e, ACE_Event_Handler::ACCEPT_MASK);	// 2-6
		//ACE_Reactor::instance()->register_handler(SIGINT, &e);	// 3-2
		//ACE_Reactor::instance()->run_reactor_event_loop();	// 2-6
		// delete read;	// 3-4
		
		// 4-3
		//e.set_interceptor(p);
		return 0;
	}
	
}
