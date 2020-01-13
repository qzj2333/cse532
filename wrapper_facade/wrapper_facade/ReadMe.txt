1. Nicole Wang
2. /cygdrive/h/cse532/wrapper_facade/x64/Debug
3. Hello World
4. 
void printLineMsg()
{
	cout << "Hello World" << endl;
}

int main(int argc, char** argv)
{
	thread t(printLineMsg);
	t.join();
	return 0;
}

5. 
void printLineMsg()
{
	cout << "Hello World" << endl;
}

int main(int argc, char** argv)
{
	thread t1(printLineMsg);
	thread t2(printLineMsg);
	t1.join();
	t2.join();

	return 0;
}

cmd result: Hello WorldHello World