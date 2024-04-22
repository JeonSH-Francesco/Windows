#include <Windows.h>
#include <iostream>
using namespace std;

int main() {
	BOOL bDir;
	bDir = CreateDirectory(L"C:\\wintest\\Dir1",NULL);

	if (bDir == FALSE) {
		cout << "CreateDirectory Failed & Error no." << GetLastError() << endl;
	}
	cout << "CreateDirectory Success" << endl;
  
	bDir = CreateDirectory(L"C:\\wintest\\Dir2", NULL);
	if (bDir == FALSE) {
		cout << "CreateDirectory Failed & Error no." << GetLastError() << endl;
	}
	cout << "CreateDirectory Success" << endl;

	bDir = RemoveDirectory(L"C:\\wintest\\Dir2");
	if (bDir == FALSE) {
		cout << "RemoveDirectory Failed & Error no" << GetLastError() << endl;
	}

	cout << "RemoveDirectory Success" << endl;

	system("PAUSE");

	return 0;
}
