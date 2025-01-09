#include <iostream>
#include <windows.h>

int main()
{
    // 문자열 상수를 가리키는 포인터, 기본적으로 읽기 전용 메모리 영역에 저장
    const char* pszHello = "Hello World!\n";
    std::cout << pszHello;

    // const 속성을 제거하여 데이터를 수정 가능하게 함 (비정상적인 사용)
    char* pszNewHello = const_cast<char*>(pszHello);

    // 메모리 보호를 변경하기 전에는 수정 시도 시 런타임 에러 발생
    // ::VirtualProtect를 사용하지 않으면, 아래 코드 활성화 시 오류 발생
    // pszNewHello[4] = '\0';  // 런타임 에러

    // 메모리 보호를 PAGE_READWRITE로 변경하여 읽기-쓰기 가능하게 함
    DWORD dwOldProtect = 0;
    ::VirtualProtect(
        (LPVOID)pszHello, 8, PAGE_READWRITE, &dwOldProtect);

    // 널 문자를 삽입하여 문자열을 잘라냄
    pszNewHello[4] = '\0';

    // 수정된 문자열 출력: "Hell"
    std::cout << pszHello;

    return 0;
}
