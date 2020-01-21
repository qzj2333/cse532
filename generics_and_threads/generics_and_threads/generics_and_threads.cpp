#include <iostream>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <thread>
#include <future>
using namespace std;

int calculateRowSum(int* beginPos, int* endPos)
{
	return accumulate(beginPos, endPos, 0);
}

int main(int argc, char** argv)
{
	// 3
	int arr[] = { -2, 19, 80, -47, 80, 80, -2 };
	int* endPos = &arr[sizeof(arr)/ sizeof(int)];
	ostream_iterator<int> result(cout, " ");
	copy(&arr[0], endPos, result);
	cout << endl;

	// 4
	int arr1[3][2] = { {-2, 19}, {80, -47}, {80, 80} };
	int rows = sizeof(arr1) / sizeof(arr1[0]);
	int cols = sizeof(arr1[0]) / sizeof(int);
	for (int i = 0; i < rows; ++i)
	{
		cout << accumulate(begin(arr1[i]), end(arr1[i]), 0) << endl;
	}

	// 5
	for (int i = 0; i < rows; ++i)
	{
		auto result = async(calculateRowSum, begin(arr1[i]), end(arr1[i]));
		cout << result.get() << endl;
	}
	return 0;
}