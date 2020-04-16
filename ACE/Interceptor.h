#include <iostream>
using namespace std;

/*void add(char& c, int n);
void subtract(char& c, int n);
void rotateLeft(char& c, int n);
void rotateRight(char& c, int n);*/

class Interceptor
{
protected:
	int factor;
	Interceptor* next;
public:
	virtual void operator()(char&)=0;
	void setNext(Interceptor* i);
};

/*class Decryption: public Interceptor
{
public:
	virtual void operator()(char& c, int n);
};

class Encryption: public Interceptor
{
public:
	virtual void operator()(char& c, int n);
};*/

class Add: public Interceptor
{
public:
	Add(int n);
	virtual void operator()(char& c);
};

class Subtract: public Interceptor
{
public:
	Subtract(int n);
	virtual void operator()(char& c);
};

class Rotate: public Interceptor
{
public:
	Rotate(int n);
	virtual void operator()(char& c);
};