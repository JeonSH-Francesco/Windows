#include <iostream>
#include <windows.h>

//컴파일러 최적화 안 함

#pragma pack(push, 1)
typedef struct JUMP_CODE {
    BYTE opCode;
    LPVOID targetAddr;
} JUMP_CODE;
#pragma pack(pop)

int main();

__declspec(naked) void testFunc(void)
{
    puts("__declspec(naked) testFunc() - Begin");
    puts("__declspec(naked) testFunc() - End");
    __asm {
        jmp main + 72h
    }
}

void targetFunc(void)
{
    puts("targetFunc()");
}

int main()
{
    puts("main() - Begin");

    //Before hook
    targetFunc();

    DWORD dwOldProtect = 0;
    BOOL bResult = ::VirtualProtect(
        (LPVOID)targetFunc, 5,
        PAGE_EXECUTE_READWRITE, &dwOldProtect);


    //목적지 - 현재주소 - 5
    //5는 JMP 명령어 크기
    JUMP_CODE jmpCode = { 0 };
    jmpCode.opCode = 0xE9;  //jmp
    jmpCode.targetAddr =
        (void*)((DWORD)testFunc - (DWORD)targetFunc - 5);

    memcpy(targetFunc, &jmpCode, 5);
    //After hook!!
    targetFunc();

    puts("main() - End");
    return 0;
}
