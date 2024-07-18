#include <windows.h>
#include <iostream>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")
using namespace std;

int main()
{
    // 다운로드할 URL
    const wchar_t* url = L"https://drive.google.com/uc?export=download&id=1VIHQCDe3lx_UMHl2fxljf4A_6V0SM7f7";

    // 저장할 파일 경로 (파일 이름 포함)
    const wchar_t* filePath = L"C:\\downloaded_file.txt";  // 원하는 파일 이름으로 변경

    // URL에서 파일 다운로드
    HRESULT hr = URLDownloadToFile(
        NULL,           // 바인딩 인터페이스 포인터 (NULL 사용 가능)
        url,            // 다운로드할 URL
        filePath,       // 저장할 파일 경로
        0,              // 예약된 플래그 (0 사용)
        NULL            // IBindStatusCallback 인터페이스 포인터 (NULL 사용 가능)
    );

    if (SUCCEEDED(hr)) {
        cout << "파일 다운로드 성공: " << filePath << endl;
    }
    else {
        cout << "파일 다운로드 실패: 0x" << hex << hr << endl;
    }
    //URLDownloadToFile 함수는 URL에서 파일을 다운로드하는 동기적인 함수로, 
    //파일 다운로드가 완료되면 함수가 반환될 때 자동으로 리소스가 해제됩니다.
    return 0;
}
