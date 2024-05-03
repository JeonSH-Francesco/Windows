#include <iostream>
#include <Windows.h>
using namespace std;

DWORD WINAPI ThreadFun(LPVOID lpParam) {
	cout << "Thread Running" << endl;
	return 0;
}
//Thread is independent entity which runs within a process.
//1. Thread share some resources to process except STACK and REGISTER.
//2. By Default thread size In WINDOWS OS is 1MB of STACK.
//3. We can create 2048 MAX thread in a Process.(FROM MSDN)
//4. Terminating a thread does not necessarily remove the thread object from the operating system.
//5. For more information, you can reach to MSDN website.
int main() {
	HANDLE hThread;
	DWORD ThreadID=NULL;

	hThread = CreateThread(NULL, 0, ThreadFun, NULL, 0,&ThreadID);
	if (hThread == NULL) {
		cout << "Thread Creation Failed & Error No->" << GetLastError() << endl;
	}
	cout << endl;
	cout << "Thread Creation Success" << endl;
	cout << "ThreadID ->" << ThreadID << endl;
	CloseHandle(hThread);
	return 0;
}
