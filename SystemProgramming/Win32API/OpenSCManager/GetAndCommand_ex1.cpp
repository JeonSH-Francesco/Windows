#include <windows.h>
#include <iostream>

using namespace std;

int main() {
    // 관리자 권한을 얻기 위한 서비스 제어 관리자 핸들
    SC_HANDLE hSCManager = OpenSCManager(
        NULL,                // 로컬 컴퓨터
        NULL,                // 기본 서비스 데이터베이스
        SC_MANAGER_ALL_ACCESS // 모든 접근 권한 요청
    );

    // OpenSCManager가 실패한 경우
    if (hSCManager == NULL) {
        cerr << "OpenSCManager 실패: " << GetLastError() << endl;
        return 1;
    }

    cout << "OpenSCManager 성공. 관리자 권한 획득." << endl;

    // STARTUPINFO 및 PROCESS_INFORMATION 구조체 초기화
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // cmd.exe에서 실행할 명령어 설정
    const wchar_t* command = L"/c tasklist > C:\\tasklist.txt";

    // STARTUPINFO 설정 (창을 숨김)
    si.dwFlags = STARTF_USESHOWWINDOW; // ShowWindow 플래그 사용
    si.wShowWindow = SW_HIDE;           // 창을 숨김 모드로 설정

    // CreateProcess를 사용하여 cmd.exe 실행
    if (!CreateProcess(
        L"C:\\Windows\\System32\\cmd.exe", // cmd.exe의 전체 경로
        const_cast<LPWSTR>(command),       // 커맨드라인 인수
        NULL,                               // 프로세스 보안 속성
        NULL,                               // 스레드 보안 속성
        FALSE,                              // 핸들 상속 불가능
        0,                                  // 플래그
        NULL,                               // 환경 변수
        NULL,                               // 현재 디렉토리
        &si,                                // 시작 정보
        &pi                                 // 프로세스 정보
    )) {
        cerr << "CreateProcess 실패: " << GetLastError() << endl;
        CloseServiceHandle(hSCManager); // 핸들 정리
        return 1;
    }

    // 프로세스 종료 대기
    WaitForSingleObject(pi.hProcess, INFINITE);

    // 핸들 정리
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseServiceHandle(hSCManager); // 서비스 제어 관리자 핸들 정리

    cout << "tasklist 명령어가 성공적으로 실행되었습니다." << endl;
    cout << "결과는 C:\\tasklist.txt 파일에 저장되었습니다." << endl;

    return 0;
}
