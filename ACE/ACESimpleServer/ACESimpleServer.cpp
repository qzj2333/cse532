#include <iostream>
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
using namespace std;

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
		ACE_INET_Addr server(1025, ACE_LOCALHOST);
		ACE_SOCK_Stream stream;
		ACE_SOCK_Acceptor acceptor;

		int call = acceptor.open(server, 1);
		
		if (call >= 0) 	// call success
		{
			ACE_TCHAR address[INET6_ADDRSTRLEN];
			server.addr_to_string(address, sizeof(address));
			ACE_OS::printf("%s\n", address);
			int connect = acceptor.accept(stream);
			if(connect >= 0)
			{
				char msg[BUFSIZ];
				while (stream.recv(msg, sizeof(msg)) > 0)
				{
					cout << msg << endl;
				}
				stream.close();
			}
			else
			{
				cerr << "Fail to connect" << endl;
			}
		}
		return 0;
	}
}