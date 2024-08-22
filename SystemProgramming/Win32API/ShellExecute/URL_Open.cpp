#include <windows.h>
#include <shellapi.h>

int main() {
    // URL을 유니코드 문자열 리터럴로 지정합니다.
    const wchar_t* url = L"http://www.naver.com";

    // ShellExecute 함수를 사용하여 기본 웹 브라우저에서 URL을 엽니다.
    HINSTANCE result = ShellExecute(NULL, L"open", url, NULL, NULL, SW_SHOWNORMAL);

    // ShellExecute의 반환 값이 오류 코드일 경우, 오류를 처리합니다.
    if ((int)result <= 32) {
        MessageBox(NULL, L"Failed to open URL.", L"Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    return 0;
}
