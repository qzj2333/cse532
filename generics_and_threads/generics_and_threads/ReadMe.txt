1. Nicole Wang, Lin Li
2. H:\cse532\generics_and_threads\Debug\generics_and_threads.exe
3. -2 19 80 -47 80 80 -2
4. 
17
33
160
5.
int calculateRowSum(int* beginPos, int* endPos)
{
	return accumulate(beginPos, endPos, 0);
}

int main(int argc, char** argv)
{
	int arr1[3][2] = { {-2, 19}, {80, -47}, {80, 80} };
	int rows = sizeof(arr1) / sizeof(arr1[0]);
	int cols = sizeof(arr1[0]) / sizeof(int);
	
	for (int i = 0; i < rows; ++i)
	{
		auto result = async(calculateRowSum, begin(arr1[i]), end(arr1[i]));
		cout << result.get() << endl;
	}
	return 0;
}
