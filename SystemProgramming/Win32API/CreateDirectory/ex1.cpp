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

	return 0;
}
