#include <Windows.h>

int main() {
	SHELLEXECUTEINFO info = { sizeof(SHELLEXECUTEINFO) };

	info.lpVerb = TEXT("runas");
	// runas는 Windows에서 프로그램을 관리자 권한으로 실행할 때 사용하는 특수한 동사입니다. 
	// 이 동사를 사용하면 UAC(사용자 계정 컨트롤)
	// 프롬프트가 나타나며 사용자는 관리자 권한으로 프로그램을 실행할 지 여부를 선택할 수 있습니다.
	// ShellExecuteEx 함수가 지정된 프로그램(cmd.exe)을 관리자 권한으로 실행하도록 지정하는 부분입니다.

	info.lpFile = TEXT("cmd.exe");

	info.lpParameters = TEXT("/c tasklist > C:\\tasklist.txt");
	//cmd.exe에 전달할 인자(tasklist 명령어를 실행하고 결과를 파일로 디라이렉트)

	info.nShow = SW_SHOW;
	//보일 것인지, 말 것인지 숨기려면 -> SW_HIDE

	ShellExecuteEx(&info);
	//구조체 정보로 쉘 실행-> 해당 구조체에 설정된 정보를 사용하여
	//지정된 작업을 수애한다는 것을 의미

	return 0;
}
