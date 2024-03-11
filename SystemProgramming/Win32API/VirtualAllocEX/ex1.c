#include <windows.h>
#include <iostream>

int main() {
    // 대상 프로세스의 핸들을 얻는다. (여기서는 자신의 프로세스를 대상으로 한다.)
    HANDLE hProcess = GetCurrentProcess();

    // 원격 프로세스에 할당할 메모리 크기 설정
    SIZE_T dwSize = 1024; // 1KB

    // 원격 프로세스에 메모리를 할당하고 해당 주소를 가져온다.
    LPVOID lpBaseAddress = VirtualAllocEx(hProcess, nullptr, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (lpBaseAddress == nullptr) {
        std::cerr << "VirtualAllocEx failed: " << GetLastError() << std::endl;
        return 1;
    }

    // 원격 프로세스에 할당된 메모리의 주소 출력
    std::cout << "Allocated memory address in remote process: " << lpBaseAddress << std::endl;

    // 할당된 메모리를 사용할 수 있습니다.

    // 원격 프로세스에 할당된 메모리 해제
    if (!VirtualFreeEx(hProcess, lpBaseAddress, 0, MEM_RELEASE)) {
        std::cerr << "VirtualFreeEx failed: " << GetLastError() << std::endl;
        return 1;
    }

    return 0;
}
