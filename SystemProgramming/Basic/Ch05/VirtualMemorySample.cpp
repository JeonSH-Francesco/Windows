#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <windows.h>

int main()
{
    const char* pwwa = "Hello, world!";
    printf("%s\n", pwwa);

    DWORD dwOldProtect = 0;
    ::VirtualProtect((LPVOID)pwwa,
        16, PAGE_READWRITE, &dwOldProtect);

    strcpy_s((char*)pwwa, 16, "Hi, Jeonshnarf");
    printf("%s\n", pwwa);
}

