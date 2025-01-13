#include <iostream>
#include <windows.h>
#include <psapi.h>

using namespace std;

#pragma pack(push, 1)
typedef struct JUMP_CODE {
    BYTE opCode;
    LPVOID targetAddr;
} JUMP_CODE;
#pragma pack(pop)


BYTE g_codesBeforeHook[5];

//최적화 하지 않음


int targetFunc(int a, int b)
{
    puts("targetFunc() - Begin");
    int result = a + b;
    puts("targetFunc() - End");
    return result;
}

int newTargetFunc(int a, int b)
{
    puts("newTargetFunc() - Begin");
    int result = a * b;
    puts("newTargetFunc() - End");
    return result;
}

__declspec(naked) int replacedTargetFunc(int a, int b)
{
    __asm
    {
        push        ebp
        mov         ebp, esp
        push        ecx
    }

    int result;
    result = newTargetFunc(a, b);

    __asm
    {
        mov         eax, dword ptr[result]
        mov         esp, ebp
        pop         ebp
        ret
    }
}

void hookTargetFunc(void *pfFunc)
{
    DWORD dwOldProtect = 0;
    BOOL bResult = ::VirtualProtect(
        (LPVOID)targetFunc, 5,
        PAGE_EXECUTE_READWRITE, &dwOldProtect);

    memcpy(g_codesBeforeHook, targetFunc, 5);

    JUMP_CODE jmpCode = { 0 };
    jmpCode.opCode = 0xE9;  //jmp
    jmpCode.targetAddr =
        (void*)((DWORD)pfFunc - (DWORD)targetFunc - 5);

    memcpy(targetFunc, &jmpCode, 5);
}

void unhookTargetFunc(void)
{
    memcpy(targetFunc, g_codesBeforeHook, 5);
}

//32bit inline hook
int main()
{
    printf("Return: %d (Before hook)\n\n", targetFunc(3, 4));

    hookTargetFunc(replacedTargetFunc);
    printf("Return: %d (After hook)\n\n", targetFunc(3, 4));
    unhookTargetFunc();

    printf("Return: %d (main() - End)\n\n", targetFunc(3, 4));
    return 0;
}
