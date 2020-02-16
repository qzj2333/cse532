#include <iostream>
#include <chrono> 
#include <thread>
using namespace std;


enum arg
{
	singleArgument = 1,
	success = 0
};




void searching(int n, int matrix[], int srow, int erow, int scol, int ecol, int col) {
	int c = 0;
	for (int i = srow; i < erow; i++)
	{
		for (int j = scol; j < ecol; j++)
		{
			if (matrix[i * col + j] == n) {
				c++;
			}
		}
	}
	cout << "value: " << n << ", " << c << " times." << endl;
}

void addition(int arr1[], int arr2[], int arr3[], int srow, int erow, int scol, int ecol, int col) {
	for (int i = srow; i < erow; i++) {
		for (int j = scol; j < ecol; j++) {
			arr3[i * col + j] = arr1[i * col + j] + arr2[i * col + j];
		}
	}
}

void multiply(int arr1[], int arr2[], int arr3[], int arr1srow, int arr1erow, int arr1scol, int arr1ecol,
	int arr2srow, int arr2erow, int arr2scol, int arr2ecol,
	int arr1col, int arr2col) 
{
	for (int i = arr1srow; i < arr1erow; i++) {
		for (int j = arr2scol; j < arr2ecol; j++) {
			int c = 0;
			for (int k = arr2srow; k < arr2erow; k++) {
				c += arr1[i * arr1col + k] * arr2[k * arr2col + j];
			}
			arr3[i * arr2col + j] = c;
		}
	}
}



int main(int argc, char** argv)
{
	/*
	int arr1[4][16] = {};
	int arr2[16][4] = {};
	int arr3[8][8] = {};

	cout << "array 1 (4 x 16): " << &arr1  <<  endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 16; j++) {
			arr1[i][j] = i + j;
			cout << "row " << i << " column " << j << " pointer " << &arr1[i][j] << endl;
		}
	}

	cout << "array 2 (16 x 4): " << endl;
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 4; j++) {
			arr2[i][j] = i + j;
			cout << "row " << i << " column " << j << " pointer " << &arr2[i][j] << endl;
		}
	}

	cout << "array 3 (8 x 8): " << endl;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			arr3[i][j] = i + j;
			cout << "row " << i << " column " << j << " pointer " << &arr3[i][j] << endl;
		}
	}
	*/

	unsigned int con_threads;

	con_threads = thread::hardware_concurrency();
	
	/* 3
	thread t1([&arr1] {searching(5, (int *)arr1, 0, 4, 0, 6, 16); });
	thread t2([&arr1] {searching(5, (int*)arr1, 2, 4, 3, 6, 16); });
	thread t3([&arr1] {searching(5, (int*)arr1, 0, 1, 0, 10, 16); });
	thread t4([&arr1] {searching(5, (int*)arr1, 1, 3, 0, 1, 16); });

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	thread t5([&arr1] {searching(5, (int*)arr1, 0, 4, 0, 6, 16); });
	thread t6([&arr1] {searching(8, (int*)arr1, 0, 4, 0, 6, 16); });
	thread t7([&arr1] {searching(10, (int*)arr1, 0, 4, 0, 6, 16); });
	thread t8([&arr1] {searching(26, (int*)arr1, 0, 4, 0, 6, 16); });
	
	
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	*/


	/* 4
	int arr4[8][8];
	int arr5[8][8];
	int arr6[8][8];

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			arr4[i][j] = i + j;
			arr5[i][j] = i + j;
		}
	}

	thread t1([&arr4, &arr5, &arr6] {addition((int*)arr4, (int*)arr5, (int*)arr6, 0, 2, 0, 8, 8); });
	thread t2([&arr4, &arr5, &arr6] {addition((int*)arr4, (int*)arr5, (int*)arr6, 2, 4, 0, 8, 8); });
	thread t3([&arr4, &arr5, &arr6] {addition((int*)arr4, (int*)arr5, (int*)arr6, 4, 6, 0, 8, 8); });
	thread t4([&arr4, &arr5, &arr6] {addition((int*)arr4, (int*)arr5, (int*)arr6, 6, 8, 0, 8, 8); });

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << arr6[i][j] << " ";
		}
		cout << endl;
	}
	*/

	int arr7[4][4];
	int arr8[4][6];
	int arr9[4][6];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			arr7[i][j] = i + j;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			arr8[i][j] = i + j;
		}
	}


	thread t1([&arr7, &arr8, &arr9] {multiply((int*)arr7, (int*)arr8, (int*)arr9, 0, 2, 0, 4, 0, 4, 0, 3, 4, 6); });
	thread t2([&arr7, &arr8, &arr9] {multiply((int*)arr7, (int*)arr8, (int*)arr9, 0, 2, 0, 4, 0, 4, 3, 6, 4, 6); });
	thread t3([&arr7, &arr8, &arr9] {multiply((int*)arr7, (int*)arr8, (int*)arr9, 2, 4, 0, 4, 0, 4, 0, 3, 4, 6); });
	thread t4([&arr7, &arr8, &arr9] {multiply((int*)arr7, (int*)arr8, (int*)arr9, 2, 4, 0, 4, 0, 4, 3, 6, 4, 6); });

	t1.join();
	t2.join();
	t3.join();
	t4.join();


	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			cout << arr9[i][j] << " ";
		}
		cout << endl;
	}
	return success;
}