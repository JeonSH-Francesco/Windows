#include <Windows.h>
#include <iostream>
using namespace std;

int main() {
	HANDLE hMutex;
	cout << "OPEN MUTEX FUN" << endl;
	getchar();

	hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"MyMutex");
	if (hMutex == NULL) {
		cout << "Mutex Open Failed & Error-" << GetLastError() << endl;
	}

	cout << "Mutex Open Success" << endl;
	CloseHandle(hMutex);

	system("PAUSE");
	return 0;
}
