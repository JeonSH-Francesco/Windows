#include <windows.h>
#include <iostream>

int main() {
    // 프로세스 생성 정보 설정
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    // 실행할 응용 프로그램과 명령행 인수 설정
    LPCSTR lpApplicationName = "C:\\Windows\\System32\\notepad.exe"; // 노트패드의 절대 경로
    LPSTR lpCommandLine = nullptr; // 명령행 인수를 넣지 않음

    // CreateProcess 함수 호출
    if (!CreateProcessA(lpApplicationName, lpCommandLine, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        std::cerr << "CreateProcess failed: " << GetLastError() << std::endl;
        return 1;
    }

    // 프로세스가 끝날 때까지 대기
    WaitForSingleObject(pi.hProcess, INFINITE);

    // 프로세스 핸들 닫기
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
