#include "Windows.h"
#include "stdio.h"

HHOOK hHook;
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam){
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN){
		PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;
		if (nCode == 0 && (int)wParam == WM_KEYDOWN){
			printf("%d : %c\n", pKey->vkCode, pKey->vkCode);
		}
	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void HookStart(){
	HMODULE hInstance = GetModuleHandle(NULL);
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, NULL);
}

void HookStop(){
	UnhookWindowsHookEx(hHook);
}

int main(){
	HookStart();
	MSG msg;

	while (GetMessage(&msg, (HWND)NULL, 0, 0)){
		if (!CallMsgFilter(&msg, 0))
			DispatchMessage(&msg);
	}
	HookStop();
}
