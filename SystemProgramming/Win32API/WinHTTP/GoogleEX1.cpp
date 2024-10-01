#include <windows.h>
#include <winhttp.h>
#include <stdio.h>

#pragma comment(lib, "winhttp.lib")

int main() {
    // 세션 핸들 선언
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL bResults = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;

    // 1. WinHttpOpen: HTTPS 세션 열기
    hSession = WinHttpOpen(L"HTTPS Example/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        printf("Error %d in WinHttpOpen.\n", GetLastError());
        return 1;
    }

    // 2. WinHttpConnect: www.google.com에 HTTPS 연결 설정
    hConnect = WinHttpConnect(hSession, L"www.google.com",
        INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        printf("Error %d in WinHttpConnect.\n", GetLastError());
        WinHttpCloseHandle(hSession);
        return 1;
    }

    // 3. WinHttpOpenRequest: HTTPS GET 요청 생성
    hRequest = WinHttpOpenRequest(hConnect, L"GET", NULL,
        NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE);
    if (!hRequest) {
        printf("Error %d in WinHttpOpenRequest.\n", GetLastError());
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return 1;
    }

    // 4. WinHttpSendRequest: 서버로 요청 전송
    bResults = WinHttpSendRequest(
        hRequest,
        WINHTTP_NO_ADDITIONAL_HEADERS, // 추가 헤더가 없으므로
        0,                              // 헤더의 길이
        WINHTTP_NO_REQUEST_DATA,       // 선택적 요청 데이터
        0,                              // 선택적 데이터의 길이
        0,                              // 플래그
        0                               // 컨텍스트
    );
    if (!bResults) {
        printf("Error %d in WinHttpSendRequest.\n", GetLastError());
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return 1;
    }

    // 5. WinHttpReceiveResponse: 서버 응답 받기
    bResults = WinHttpReceiveResponse(hRequest, NULL);
    if (!bResults) {
        printf("Error %d in WinHttpReceiveResponse.\n", GetLastError());
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return 1;
    }

    // 6. WinHttpQueryDataAvailable: 응답 데이터 크기 확인
    do {
        dwSize = 0;
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
            printf("Error %d in WinHttpQueryDataAvailable.\n", GetLastError());
            break; // 오류 발생 시 루프 종료
        }

        // 7. 데이터 읽기
        if (dwSize > 0) {
            pszOutBuffer = new char[dwSize + 1];
            if (!pszOutBuffer) {
                printf("Out of memory\n");
                dwSize = 0;
            }
            else {
                ZeroMemory(pszOutBuffer, dwSize + 1);
                if (WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                    printf("%s", pszOutBuffer);
                }
                else {
                    printf("Error %d in WinHttpReadData.\n", GetLastError());
                }
                delete[] pszOutBuffer;
            }
        }
    } while (dwSize > 0);

    // 8. 핸들 닫기
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return 0;
}
