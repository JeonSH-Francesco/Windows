#include <iostream>
#include <windows.h>

#pragma comment(lib, "..\\x64\\Debug\\DllTest.lib")
void WINAPI DllTestFunction(int nParam); //선언

int main()
{
    DllTestFunction(5);
}
