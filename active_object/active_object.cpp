#include "active_object.h"
#include <thread>

template <typename T>
active_object<T>::active_object()
{
	isActive = false; // 3
	obj = 0; // 5
};

// 5
template <typename T>
active_object<T>::active_object(active_object *ao)
{
	isActive = false; // 3
	obj = ao;
};

template <typename T>
void active_object<T>::enqueue(T item)
{
	l.push_front(item);
	cout << "push: " << item << endl;
}

template <typename T>
void active_object<T>::run_service()
{
	bool popAll = false;
	if (obj == 0) // 5
	{
		while (isActive && !popAll)
		{
			T item = l.pop_back();
			if (item == NULL)
			{
				popAll = true;
			}
			else
			{
				cout << "pop: " << item << endl;
			}
		}
	}
	else
	{
		while (isActive && !popAll)
		{
			T item = l.pop_back();
			if (item == NULL)
			{
				popAll = true;
			}
			else
			{
				obj->enqueue(item);
				cout << "chain enqueue: " << item << endl;
			}
		}
		
	
	}

}

// 3
template <typename T>
void active_object<T>::active()
{
	if (!isActive)
	{
		isActive = true;
		thread t(&active_object<T>::run_service, this);
		t.detach();
	}
}

// 4
template <typename T>
void active_object<T>::deactive()
{
	if (isActive)
	{
		isActive = false;
		l.shut_down();
	}
}