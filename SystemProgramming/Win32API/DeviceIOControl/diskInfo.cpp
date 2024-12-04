#include <windows.h>
#include <iostream>
#include <iomanip>
using namespace std;

void PrintDriveType(ULONG mediaType) {
	switch (mediaType) {
	case FixedMedia:
		cout << "디스크 유형 : 고정 디스크" << endl;
		break;
	case RemovableMedia:
		cout << "디스크 유형 : 이동식 디스크" << endl;
		break;
	default:
		cout << "디스크 유형 : 알 수 없음" << endl;
		break;
	}
}

int main() {
	// PhysicalDrive0 접근
	HANDLE hDrive = CreateFile(
		L"\\\\.\\PhysicalDrive0",  // 디스크 번호
		GENERIC_READ,             // 읽기 권한
		FILE_SHARE_READ | FILE_SHARE_WRITE,  // 공유 권한
		NULL,                     // 보안 속성
		OPEN_EXISTING,            // 이미 존재하는 장치를 염
		0,                        // 플래그 없음
		NULL                      // 템플릿 없음
	);

	if (hDrive == INVALID_HANDLE_VALUE) {
		cerr << "디스크를 열 수 없습니다. 에러 코드: " << GetLastError() << endl;
		return 1;
	}
	// 디스크의 기본 정보 가져오기
	DISK_GEOMETRY_EX dgEx = { 0 };
	DWORD bytesReturned = 0;

	if (DeviceIoControl(
		hDrive,
		IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,  // 드라이브 정보 + 크기
		NULL,
		0,
		&dgEx,
		sizeof(dgEx),
		&bytesReturned,
		NULL
	)) {
		PrintDriveType(dgEx.Geometry.MediaType);  // 디스크 종류 출력
		double totalSizeGB = static_cast<double>(dgEx.DiskSize.QuadPart) / (1024 * 1024 * 1024);
		cout << "디스크 총 크기: " << fixed << setprecision(2) << totalSizeGB << " GB" << endl;
	}
	else {
		cerr << "DeviceIoControl 실패 (기본 정보). 에러 코드: " << GetLastError() << endl;
	}

	// 디스크 용량 정보 가져오기
	ULARGE_INTEGER freeBytesAvailableToCaller, totalBytes, freeBytes;
	if (GetDiskFreeSpaceEx(
		L"C:\\",  // 드라이브 경로, 예: C 드라이브
		&freeBytesAvailableToCaller,
		&totalBytes,
		&freeBytes
	)) {
		double totalSpaceGB = static_cast<double>(totalBytes.QuadPart) / (1024 * 1024 * 1024);
		double freeSpaceGB = static_cast<double>(freeBytes.QuadPart) / (1024 * 1024 * 1024);
		cout << "전체 용량: " << fixed << setprecision(2) << totalSpaceGB << " GB" << endl;
		cout << "사용 가능한 공간: " << fixed << setprecision(2) << freeSpaceGB << " GB" << endl;
	}
	else {
		cerr << "GetDiskFreeSpaceEx 실패. 에러 코드: " << GetLastError() << endl;
	}

	CloseHandle(hDrive);  // 핸들 닫기
	return 0;

}
