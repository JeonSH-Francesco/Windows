#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
using namespace std;

int main() {
	//Take a snapshot of all processes in the system
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		cerr << "CreateToolhelp32Snapshot failed : " << GetLastError() << endl;
		return 1;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	//Retrieve information about the first process
	if (!Process32First(hSnapshot, &pe32)) {
		cerr << "Process32First failed : " << GetLastError() << endl;
		CloseHandle(hSnapshot);
		return 1;
	}

	//Now walk the snapshot of processes
	do{
		wcout << L"Process name : " << pe32.szExeFile << endl;
		cout << "Process ID :" << pe32.th32ProcessID << endl;
		cout << "Thread count :" << pe32.cntThreads << endl;
	} while (Process32Next(hSnapshot, &pe32));

	CloseHandle(hSnapshot);
	return 0;
}

