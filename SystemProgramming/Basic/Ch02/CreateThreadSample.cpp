#include <iostream>
using namespace std;
#include <windows.h>

DWORD WINAPI ThreadFunction(LPVOID pParam)
{
    cout << "ThreadFunction() - Begin" <<endl;
    cout << (const char*)pParam << endl;
    cout << "ThreadFunction() - End" << endl;
    return 0;
}

int main()
{
    cout << "main() - Begin" << endl;
    DWORD dwThreadId = 0;
    HANDLE hThread = ::CreateThread(
        NULL,
        0,
        ThreadFunction,
        (LPVOID)"PARAM",
        0,
        &dwThreadId);
    if (hThread == NULL) {
    }

    cout << "main() - WaitForSingleObject() - before" << endl;

    ::WaitForSingleObject(hThread, INFINITE);
    ::CloseHandle(hThread);

    cout << "main() - End" << endl;
    return 0;
}

