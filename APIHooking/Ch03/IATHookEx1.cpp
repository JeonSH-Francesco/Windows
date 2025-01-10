#include <iostream>
#include <Windows.h>

using namespace std;

//후킹 후 대체될 사용자 정의 MessageBox 함수
int MyMessageBox(
    HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
    MessageBoxW(NULL, L"MyMessageBox", L"*** Hooked ***", MB_OK);
    return 0;
}

//현재 모듈의 IAT 정보를 가져오는 함수
PIMAGE_IMPORT_DESCRIPTOR getImportTable()
{
    //현재 실행 중인 모듈의 베이스 주소 가져오기
    LPVOID baseAddress = GetModuleHandle(NULL);
    PIMAGE_DOS_HEADER dosHeaders = (PIMAGE_DOS_HEADER)baseAddress;

    //NT Header를 가져오기 위해 DOS Header의 e_lfanew 값을 이용
    PIMAGE_NT_HEADERS ntHeader =
        (PIMAGE_NT_HEADERS)((DWORD_PTR)baseAddress + dosHeaders->e_lfanew);
    IMAGE_DATA_DIRECTORY importTableDesc =
        ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

    //Import Table의 실제 주소를 계산
    PIMAGE_IMPORT_DESCRIPTOR importTable = NULL;
    importTable = (PIMAGE_IMPORT_DESCRIPTOR)
        ((DWORD_PTR)baseAddress + importTableDesc.VirtualAddress);

    return importTable; //Import Table의 시작 주소 반환
}

//IAT에 있는 특정 함수의 주소를 사용자가 정의한 함수로 대체하는 함수
int hookIAT(const char* pszfuncName, LPVOID fphookFunc,
    PIMAGE_IMPORT_DESCRIPTOR importTable)
{

    LPVOID pBaseAddress = GetModuleHandle(NULL); //현재 모듈의 베이스 주소 가져오기

    //Import Table을 순회
    while (importTable->Name != NULL)
    {
        //INT의 OriginalFirstThunk와 FirstThunk(Address Table)의 시작 주소
        PIMAGE_THUNK_DATA impNameTable = (PIMAGE_THUNK_DATA)
            ((DWORD_PTR)pBaseAddress + importTable->OriginalFirstThunk);
        PIMAGE_THUNK_DATA impAddrTable = (PIMAGE_THUNK_DATA)
            ((DWORD_PTR)pBaseAddress + importTable->FirstThunk);

        //Name Table에서 함수 이름 확인
        while (impNameTable->u1.AddressOfData != NULL)
        {
            //함수 이름 가져오기
            PIMAGE_IMPORT_BY_NAME importByName =
                (PIMAGE_IMPORT_BY_NAME)
                ((DWORD_PTR)pBaseAddress + impNameTable->u1.AddressOfData);
            //함수 이름 출력
            cout << importByName->Name << endl;


            //원하는 함수 이름(pszfuncName)과 일치하는지 확인
            if (strcmp(importByName->Name, pszfuncName) == 0)
            {
                DWORD oldProtect;
                //메모리 보호 설정을 READ, WRITE로 설정
                ::VirtualProtect(&impAddrTable->u1.Function,
                    sizeof(DWORD_PTR), PAGE_READWRITE, &oldProtect);
                //함수 주소를 사용자가 정의한 함수(fpHookFunc)로 대체
                impAddrTable->u1.Function = (DWORD_PTR)fphookFunc;
            }

            impNameTable++; //다음 Name Table 항목으로 이동
            impAddrTable++; //다음 Address Table 항목으로 이동
        }

        importTable++; //다음 Import Descriptor로 이동
    }
    return 0;
}

int main(void)
{   //후킹 전 MessageBoxA 호출
    MessageBoxA(NULL, "Before IAT hook", "Hook test", MB_OK);

    //Import Table 가져오기
    PIMAGE_IMPORT_DESCRIPTOR importTable = getImportTable();
    
    //MessageBoxA의 주소를 MyMessageBoxA로 대체
    hookIAT("MessageBoxA", (LPVOID)MyMessageBox, importTable);

    //후킹 후 MessageBoxA 호출 (MyMessageBox)가 실행됨
    MessageBoxA(NULL, "After IAT hook", "Hook test", MB_OK);
    return 0;
}
