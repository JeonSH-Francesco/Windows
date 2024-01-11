// TestApp.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <windows.h>

#pragma comment(lib, "..\\x64\\Debug\\DllTest.lib")
void WINAPI DllTestFunction(int nParam); //선언

int main()
{
    DllTestFunction(5);
}
