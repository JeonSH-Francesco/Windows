#include <iostream>
#include <winsock2.h>
#include <psapi.h>

void hookTargetFunc(void* pfOrg, void* pfNew, BYTE* pOrgBytes);
void unHookSend(void);
void* g_pfSend;
BYTE g_originalCodes[12] = { 0 };

int WSAAPI mySend(SOCKET s, const char* buf, int len, int flags)
{
    puts("\t***** mySend() - Begin *****");
    unHookSend();

    puts("\tsend() - winsock");
    ::send(s, buf, len, flags);
    //::send(s, "Hello", 6, flags);

    hookTargetFunc(g_pfSend, mySend, g_originalCodes);
    puts("\t****** mySend() - End ******");
    return 0;
}


void hookTargetFunc(void* pfOrg, void* pfNew, BYTE* pOrgBytes)
{
    DWORD dwOldProtect = 0;
    BYTE pBuf[12] = { 0x48, 0xB8, };

    ::VirtualProtect((LPVOID)pfOrg, 12,
        PAGE_EXECUTE_READWRITE, &dwOldProtect);

    memcpy(pOrgBytes, pfOrg, 12);

    // MOV RAX, 절대주소(64-bit).
    memcpy(&pBuf[2], &pfNew, 8);
    //JMP RAX
    pBuf[10] = 0xFF;
    pBuf[11] = 0xE0;

    // Hook - 12 byte 패치(JMP XXXX)
    memcpy(pfOrg, pBuf, 12);
    puts("\t#Hook");
}

void unHookSend(void)
{
    memcpy(g_pfSend, g_originalCodes, 12);
    puts("\t#Unhook");
}
