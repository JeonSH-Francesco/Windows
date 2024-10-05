#include <windows.h>
#include <iostream>

int main() {
    char systemDir[MAX_PATH] = { 0 }; // 배열을 0으로 초기화
    DWORD length = GetSystemDirectoryA(systemDir, sizeof(systemDir)); // 시스템 디렉터리 가져오기

    if (length > 0) {
        std::cout << "System Directory: " << systemDir << std::endl; // 경로 출력
    }
    else {
        DWORD error = GetLastError(); // 오류 코드 가져오기
        std::cout << "Failed to get system directory. Error code: " << error << std::endl; // 오류 출력
    }

    return 0;
}
