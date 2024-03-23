#include <Windows.h>
#include <stdio.h>

HHOOK hHook;
//후킹 핸들 선언

// 키보드 후킹 프로시저
//후킹된 이벤트가 발생했을때 호출되는 콜백함수로 후킹된 키보드 이벤트를 처리한다.
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    // 키 다운 이벤트 확인=> 즉, 키를 눌렸을 때 발생하는 메시지
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {

        // 키보드 후킹 구조체(후킹된 키 이벤트에 대한 정보를 포함하고 있다.)
        PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

        // 키 이벤트가 HC_ACTION이고 WM_KEYDOWN인 경우에만 처리
        if (nCode == HC_ACTION && (int)wParam == WM_KEYDOWN) {
            printf("Key Code: %d, Char: %c\n", pKey->vkCode, pKey->vkCode);
        }
    }
    // 다음 후킹으로 전달=>후킹된 메시지를 다음 후킹 프로시저로 전달하여 다른 후킹 프로시저들이 이 메시지를 처리할 수 있도록 한다.
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

// 후킹 시작 함수
void HookStart() {
    // 현재 모듈의 핸들을 얻어옴
    HMODULE hInstance = GetModuleHandle(NULL); //현재 모듈의 핸들을 가져온다.(후킹 설정 시 사용)
    // Low-level 키보드 후킹 설정
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);//SetWindowsHookEx함수를 호출하여 저수준 키보드 후킹 설정
}

// 후킹 종료 함수
void HookStop() {
    // 후킹 제거
    UnhookWindowsHookEx(hHook);
}

int main() {
    // 후킹 시작
    HookStart();
    MSG msg; //MSG 구조체는 윈도우 메시지

    // 메시지 루프=>GetMessage 함수는 메시지가 없을때까지 대기
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);  //특정 메시지 번역
        DispatchMessage(&msg); //윈도우 프로시저로 메시지 보냄.
    }

    // 후킹 종료
    HookStop();

    return 0;
}
