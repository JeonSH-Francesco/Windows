#include <Windows.h>
#include <iostream>
using namespace std;

int main() {
	BOOL bFile;
	bFile = MoveFile(L"C:\\wintest\\hello1.txt", L"C:\\wintest\\hello2.txt");

	if (bFile == FALSE) {
		cout << "MoveFile Failed & Error No" << GetLastError << endl;
	}
	cout << "MoveFile Success" << endl;

	return 0;
}
