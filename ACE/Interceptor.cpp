#include "Interceptor.h"

/*void Decryption::operator()(char& c, int n)
{
	subtract(c, n);
};

void Encryption::operator()(char& c, int n)
{
	add(c, n);
};*/

Add::Add(int n)
{
	factor = n;
	next = NULL;
}

Subtract::Subtract(int n)
{
	factor = n;
	next = NULL;
}

Rotate::Rotate(int n)
{
	factor = n;
	next = NULL;
}

void Interceptor::setNext(Interceptor* i)
{
	Interceptor* last = this;

    while (last->next != NULL)
    {
        last = last->next;
    }
    last->next = i;
}

void Add::operator()(char& c)
{
	int dist;
	if(isdigit(c))
	{
		dist = c - '0';
		c = '0' + ((dist + factor) % 10);
	}
	else if(islower(c))
	{
		dist = c - 'a';
		c = 'a' + ((dist + factor) % 26);
	}
	else if(isupper(c))
	{
		dist = c - 'A';
		c = 'A' + ((dist + factor) % 26);
	}
	
	if(next != NULL)
	{
		next->operator()(c);
	}
};

void Subtract::operator()(char& c)
{
	int dist;
	if(isdigit(c))
	{
		dist = c - '0';
		c = '0' + ((dist - factor) % 10);
	}
	else if(islower(c))
	{
		dist = c - 'a';
		c = 'a' + ((dist - factor) % 26);
	}
	else if(isupper(c))
	{
		dist = c - 'A';
		c = 'A' + ((dist - factor) % 26);
	}
	
	if(next != NULL)
	{
		next->operator()(c);
	}
};

void Rotate::operator()(char& c)
{
	c += factor;
	
	if(next != NULL)
	{
		next->operator()(c);
	}
};