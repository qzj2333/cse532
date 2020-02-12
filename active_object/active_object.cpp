#include "active_object.h"

template <class T>
active_object<T>::active_object()
{

};

template <class T>
void active_object<T>::enqueue(T item)
{
	l.push_front(item);
}

template <class T>
void active_object<T>::run_service()
{
	cout << l.pop_back() << endl;
}

int main(int argc, char** argv)
{
	
	active_object<int> obj;
	obj.enqueue(1);
	obj.run_service();
	obj.enqueue(2);
	obj.run_service();
	obj.enqueue(3);
	return 0;
}
