// beginthreadSample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <process.h> // _beginthreadex 함수를 사용하기 위해 필요한 헤더 파일
#include <windows.h>

// 스레드에서 실행될 함수 정의
UINT WINAPI ThreadFunction(LPVOID pParam)
{
    std::cout << "ThreadFunction() - Begin" << std::endl;
    std::cout << (const char*)pParam << std::endl; // 전달된 파라미터 출력
    std::cout << "ThreadFunction() - End" << std::endl;
    return 0;
}

int main()
{
    std::cout << "main() - Begin" << std::endl;

    UINT nThreadId = 0; // 새로 생성되는 스레드의 ID를 저장할 변수
    HANDLE hThread = (HANDLE)::_beginthreadex(
        NULL, // 보안 설정을 나타내는 매개변수 (기본값 사용)
        0,    // 스택 크기 (0으로 설정하면 기본값 사용)
        ThreadFunction, // 실행할 함수
        (LPVOID)"PARAM", // 스레드에 전달할 파라미터
        0,    // 스레드 플래그 (0으로 설정하면 기본값 사용)
        &nThreadId); // 생성된 스레드의 ID를 저장할 변수

    if (hThread == NULL) {
        // 스레드 생성에 실패한 경우 처리할 내용 추가
    }

    ::WaitForSingleObject(hThread, INFINITE); // 스레드가 종료될 때까지 대기
    ::CloseHandle(hThread); // 스레드 핸들을 닫음

    std::cout << "main() - End" << std::endl;

    return 0;
}
