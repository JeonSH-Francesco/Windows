#include <windows.h>
#include <stdio.h>

int main() {
    WCHAR cur_path[MAX_PATH] = { 0 };
    GetCurrentDirectory(MAX_PATH, cur_path);
    printf("%ws\n", cur_path);
    return 0;
}
/*
첫번째 매개변수 : [in] nBufferLength

TCHAR에서 현재 디렉터리 문자열에 대한 버퍼의 길이입니다. 버퍼 길이에는 종료 null 문자에 대한 공간이 포함되어야 합니다.

두번째 매개변수 : [out] lpBuffer

현재 디렉터리 문자열을 수신하는 버퍼에 대한 포인터입니다. 이 null로 끝나는 문자열은 현재 디렉터리에 대한 절대 경로를 지정합니다.
*/
