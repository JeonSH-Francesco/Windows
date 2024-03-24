#include <windows.h>
#include <iostream>
using namespace std;

int main() {
    WCHAR cur_path[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, cur_path, MAX_PATH);
  //첫번째 변수 : [in, optional] hModule 경로가 요청되는 로드된 모듈에 대한 핸들입니다. 이 매개 변수가 NULL이면 GetModuleFileName 은 현재 프로세스의 실행 파일 경로를 검색합니다.
  //두번째 변수 : [out] lpFilename 모듈의 정규화된 경로를 수신하는 버퍼에 대한 포인터입니다. 경로 길이가 nSize 매개 변수가 지정한 크기보다 작으면 함수가 성공하고 경로가 null로 끝나는 문자열로 반환
  //세번째 변수 : TCHAR의 lpFilename 버퍼 크기
    wcout << cur_path << endl;
    return 0;
}
