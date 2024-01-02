// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "pch.h"
#include <stdio.h>


__declspec(dllexport) void WINAPI DllTestfunction(int);


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


void WINAPI DllTestfunction(int nParam) {
    printf("DLL TEST : DllTestFunction(param: %d)\n",nParam);
}

//DL라이브러리는 실행파일이 로딩이 되어야 하는데 
//그럴때 마다 DllMain함수를 호출 해야 한다.
//호출되는 이유가 총 4가지(case)이다. 프로세스(또는 스레드)가 Attach 될 때, Detach 될 때,
