#include <iostream>
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Event_Handler.h"
#include "ace/Timer_Queue_Adapters.h"
#include "ace/Timer_Heap.h"
#include "ace/Reactor.h"
using namespace std;

class Event: public ACE_Event_Handler
{
	int size;
	char** msg;
	ACE_INET_Addr* server;
	ACE_SOCK_Stream stream;
	ACE_SOCK_Connector connector;
public:
	Event(int argc, char* argv[])
	{
		size = argc;
		msg = argv;
		server = new ACE_INET_Addr(1025, ACE_LOCALHOST);
	};

	virtual int handle_timeout (const ACE_Time_Value & atv, const void* p=0)
	{
		int call = connector.connect(stream, *server);
		if (call >= 0) 	// call success
		{
			for(int i = 1; i < size; i++)
			{
				stream.send_n(msg[i], sizeof(msg[i]));
			}
			stream.close();
		}
		else
		{
			cerr << "Fail to connect server" << endl;
		}
		return 0;
	};
};

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		cerr << "no command line arguments" << endl;
		return 1;
	}
	else
	{
		// 1-2
		/*for(int i = 1; i < argc; i++)
		{
			cout << argv[i] << endl;
		}*/
		
		// 1-5
		/*ACE_INET_Addr server(1025, ACE_LOCALHOST);
		ACE_SOCK_Stream stream;
		ACE_SOCK_Connector connector;

		int call = connector.connect(stream, server);
		
		if (call >= 0) 	// call success
		{
			for(int i = 1; i < argc; i++)
			{
				stream.send_n(argv[i], sizeof(argv[i]));
			}
			stream.close();
		}
		else
		{
			cerr << "Fail to connect server" << endl;
		}*/
		
		// 2-4
		ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> timer;
		timer.activate();
		ACE_Time_Value intervalTime(3, 0);
		/*ACE_Time_Value startTime = ACE_OS::gettimeofday();
		startTime += intervalTime;*/
		ACE_Time_Value delayTime (0,0);

		Event e(argc, argv);
		//e.handle_timeout(intervalTime); // 2-3
		
		/*timer.schedule(&e, 0, startTime, intervalTime);
		ACE_Thread_Manager::instance()->wait();*/
		ACE_Reactor::instance()->schedule_timer(&e, 0, delayTime, intervalTime);
		ACE_Reactor::instance()->run_event_loop();
		
		return 0;
	}
}