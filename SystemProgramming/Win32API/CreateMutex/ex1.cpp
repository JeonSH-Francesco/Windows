#include <Windows.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main() {
	//1. Mutex is a locking Mechanism.
	//2. Mutex has only one state either Locked or Unlocked.
	//3. Mutex has mutual ownership means those who acquire the lock only can release the lock.
	//4. If Mutex is locked more than once in NON-RECURSIVE than DeadLock will occure.
	//5. Mutex is only used for thread Synchronization.
	//6. In Windows:  i.Named Mutex, ii. UnNamed Mutex

	HANDLE hMutex;
	cout << "Mutex 1" << endl;
	hMutex = CreateMutex(NULL, FALSE, L"MyMutex");//Named Mutex

	if (hMutex == NULL) {
		cout << "CreateMutex failed & Error No -" << GetLastError() << endl;
	}

	cout << "CreateMutex Success" << endl;

	getchar();

	//CloseHandle(hMutex);
	system("PAUSE");
	return 0;

}
