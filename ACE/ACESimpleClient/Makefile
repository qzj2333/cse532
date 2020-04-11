all:
	g++ -L$(ACE_ROOT)/ace -I$(ACE_ROOT) -lACE -Wall -std=c++11 -pthread -DTEMPLATE_HEADERS_INCLUDE_SOURCE -o ACESimpleClient ACESimpleClient.cpp -I../ACE_wrappers

clean:
	rm ACESimpleClient *~