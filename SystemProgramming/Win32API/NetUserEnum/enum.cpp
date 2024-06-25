#include <Windows.h>
#include <LMaccess.h>
#include <LMAPIbuf.h>
#include <stdio.h>
#include <locale.h>
#pragma comment(lib,"netapi32")

int main() {
	setlocale(LC_ALL, "KOREAN");

	PUSER_INFO_0 buffer;
	DWORD rcnt = 0;
	DWORD total = 0;

	NetUserEnum(NULL, 0, 0, (LPBYTE*)&buffer, 0xFFFFFFFF, &rcnt, &total, NULL);
	/*차례대로 
	    NetUserEnum(NULL,//서버 이름(NULL이면 로컬 컴퓨터)
        0, //데이터 정보 수준 0일 때 USER_INFO_0 구조체 배열
        0, //계정유형 (0이면 모든 일반 사용자, 트로스트 데이터 및 컴퓨터 계정 데이터 포함)
        (LPBYTE*)&buffer,//수신할 버퍼의 포인터(시스템에 의해 할당)
        0xFFFFFFFF, //데이터의 기본 설정 최대 길이(바이트)
        &rcnt,//실제 열거한 요소 수
        &total,//다시 시작에서 열거할 수 있는 총 항목 수(힌트로만 고려)
        NULL);//계속 검색할 때 사용하는 핸들
	*/
    for (DWORD i = 0; i < total; i++)
    {
        wprintf(TEXT("%s\n"), buffer[i].usri0_name);
    }
    NetApiBufferFree(buffer);//메모리 해제
    return 0;
}
