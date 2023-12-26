#include <iostream>
using namespace std;
#include <process.h>
#include <windows.h>

UINT WINAPI ThreadFunction(LPVOID pParam) {
	cout << "ThreadFunction() - Begin" << endl;
	cout << (const char*)pParam << endl;
	cout << "ThreadFunction() - End" << endl;
	return 0;
}

DWORD WINAPI ThreadFunction1(LPVOID pParam) {
	cout << "ThreadFunction()1-Begin" << endl;
	cout << (const char*)pParam << endl;
	cout << "ThreadFunction1() - End" << endl;
    return 0;
}

int main() {
    cout << "main() - Begin" << endl;
    //main스레드가 부모 스레드

    UINT nThreadId = 0; // 새로 생성되는 스레드의 ID를 저장할 변수

    HANDLE hThread = (HANDLE)::_beginthreadex(
        NULL, // 보안 설정을 나타내는 매개변수 (기본값 사용)
        0,    // 스택 크기 (0으로 설정하면 기본값 사용)
        ThreadFunction, // 실행할 함수-> 이 시점에서 스레드 생성
        (LPVOID)"PARAM", // 스레드에 전달할 파라미터
        0,    // 스레드 플래그 (0으로 설정하면 기본값 사용)
        &nThreadId); // 생성된 스레드의 ID를 저장할 변수
    //ThreadFunction이 자식 스레드

    if (hThread == NULL) {
        cerr << "Error : Failed to create thread." << endl;
        return 1; // 프로그램 종료 코드 로 설정
    }

    DWORD dwThreadId = 0; // 새로 생성되는 스레드의 ID를 저장할 변수

    HANDLE hThread1 = ::CreateThread(
        NULL, //보안 설정을 나타내는 매개변수 마찬가지로 반환된 핸들을 자식 프로세스에서 상속하지 않음.(기본 값 사용)
        0, //스택 크기
        ThreadFunction1, //실행할 함수명
        (LPVOID)"PARAM1",//스레드에 전달할 파라미터
        0,//스레드 플래그
        &dwThreadId //생성된 스레드의ID를 저장할 변수
    );

    if (hThread1 == NULL) {
        cerr << "Error : Failed to create second thread." << endl;
        ::CloseHandle(hThread);
        return 1;
    }
   
    HANDLE hThreads[2] = { hThread,hThread1};

    //우연이라는 단어가 등장하게 되는 시기
    //ThreadFunction 함수안에 있는 내용이 먼저 될지
    //메인 스레드가 진행되면서 아래의 내용들이 먼저 처리 될지는 우연이다.
	
    cout << "main() - WaitForMultipleObjects-before." << endl;
	
    ::WaitForMultipleObjects(2, hThreads, TRUE, INFINITE); //두개의 스레드가 모두 종료할 때 가지 대기
    ::CloseHandle(hThread);
    ::CloseHandle(hThread1);

    cout << "main() - End" << endl;

    return 0;
}
