#include <Windows.h>
#include <stdio.h>

HHOOK hHook;

// 키보드 후킹 프로시저
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    // 키 다운 이벤트 확인
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        // 키보드 후킹 구조체
        PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;
        // 키 이벤트가 HC_ACTION이고 WM_KEYDOWN인 경우에만 처리
        if (nCode == HC_ACTION && (int)wParam == WM_KEYDOWN) {
            printf("Key Code: %d, Char: %c\n", pKey->vkCode, pKey->vkCode);
        }
    }
    // 다음 후킹으로 전달
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

// 후킹 시작 함수
void HookStart() {
    // 현재 모듈의 핸들을 얻어옴
    HMODULE hInstance = GetModuleHandle(NULL);
    // Low-level 키보드 후킹 설정
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);
}

// 후킹 종료 함수
void HookStop() {
    // 후킹 제거
    UnhookWindowsHookEx(hHook);
}

int main() {
    // 후킹 시작
    HookStart();
    MSG msg;

    // 메시지 루프
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 후킹 종료
    HookStop();

    return 0;
}
