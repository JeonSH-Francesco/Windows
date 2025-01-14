#include <iostream>
#include <windows.h>
#include <psapi.h>

using namespace std;

//Release X64, 최적화 옵션 Off, 빌드 종속성->사용자 지정 빌드 masm 체크, Micro Macro Assembler 설정

//replacedTargetFunc는 어셈블리에서 호출될 함수임을 명시(C 스타일 이름 사용)
extern "C" void replacedTargetFunc(int, int);

//원래 호출될 함수(후킹 전)
//임의기준주소 Off
int targetFunc(int a, int b)
{
    puts("targetFunc() - Begin");
    int result = a + b;
    puts("targetFunc() - End");
    return result;
}

//후킹 후 호출될 함수
extern "C" int newTargetFunc(int a, int b)
{
    puts("newTargetFunc() - Begin");
    int result = a * b;
    printf("result: %d\n", result);
    puts("newTargetFunc() - End\n");
    return result;
}

//함수 후킹을 구현하는 함수
void hookTargetFunc(void* pfOrg, void* pfNew, BYTE* pOrgBytes)
{
    DWORD dwOldProtect = 0; //기존 메모리 보호 상태 저장
    BYTE pBuf[12] = { 0x48, 0xB8, }; // MOV RAX, 절대 주소 명령어 시작

    //함수 메모리 보호 설정 변경(쓰기 가능하게 설정)
    ::VirtualProtect((LPVOID)pfOrg, 12,
        PAGE_EXECUTE_READWRITE, &dwOldProtect);

    //원래 함수의 첫 12 bytes를 백업
    memcpy(pOrgBytes, pfOrg, 12);

    // MOV RAX, 절대주소(64-bit).(pfNew의 주소를 복사)
    memcpy(&pBuf[2], &pfNew, 8);
    //JMP RAX 명령어 추가
    pBuf[10] = 0xFF;
    pBuf[11] = 0xE0;

    // Hook - 12 byte 패치(JMP XXXX)
    //원래 함수의 첫 12 bytes를 후킹 코드로 덮어 씀
    memcpy(pfOrg, pBuf, 12);
}

int main()
{   
    //후킹 전 targetFunc의 동작 확인
    printf("Return: %d (Before hook)\n\n", targetFunc(3, 4));

    BYTE originalCodes[12]; //원래 코드 저장 공간
    //targetFunc를 replacedTargetFunc로 후킹
    hookTargetFunc(targetFunc, replacedTargetFunc, originalCodes);

    //후킹 후 targetFunc의 동작 확인
    int result = targetFunc(3, 4);
    printf("Return: %d (After hook)\n\n", result);
    return 0;
}
/*
targetFunc() - Begin
targetFunc() - End
Return: 7 (Before hook)

newTargetFunc() - Begin
result: 12
newTargetFunc() - End

targetFunc() - Begin
targetFunc() - End
Return: 7 (After hook)


*/
