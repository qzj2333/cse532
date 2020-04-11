#include <iostream>
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"

using namespace std;

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		cerr << "no command line arguments" << endl;
		return 1;
	}
	else
	{
		// 2
		/*for(int i = 1; i < argc; i++)
		{
			cout << argv[i] << endl;
		}*/
		
		// 5
		ACE_INET_Addr server(1025, ACE_LOCALHOST);
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
		}
		
		return 0;
	}
	
}