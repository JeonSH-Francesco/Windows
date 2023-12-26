#include <iostream>
using namespace std;
#include <windows.h>

int main() {
	LARGE_INTEGER freq;
	LARGE_INTEGER begin;
	LARGE_INTEGER end;
	__int64 elapsed;


	//CPU 타이머 주파수 확인
	if (!QueryPerformanceFrequency(&freq)) {
		cerr << "QueryPerformanceFrequency" << endl;
		return -1;
	}

	::QueryPerformanceFrequency(&freq);
	cout << "초당 주파수 : " << freq.QuadPart << endl;

	for (int i = 0; i < 5; i++) {
		if (!QueryPerformanceCounter(&begin)) {
			cerr << "QueryPerformanceCounter failed!" << endl;
			return -1;
		}

		::QueryPerformanceCounter(&begin);
		//우연에 맡기기 위한 코드 한 줄!
		::Sleep(10);

		if (!QueryPerformanceCounter(&end)) {
			cerr << "QueryPerformanceCounter failed!" << endl;
			return -1;
		}

		::QueryPerformanceCounter(&end);

		elapsed = end.QuadPart - begin.QuadPart;

		cout << "실제로 흘러간 시간 : " << elapsed << " ticks" << endl;
		//Ticks는 컴퓨터 시스템에서 발생하는 정밀한 시간 측정 단위입니다. 
		// 보통 하드웨어의 성능 카운터(High-Resolution Performance Counter)나 타이머가 발생시키는 일정 시간 간격을 나타냅니다. 
		//이는 일반적으로 나노초(nanoseconds) 또는 특정 시스템에서는 마이크로초(microseconds) 단위의 작은 시간 간격을 의미합니다.
		cout << "실제로 흘러간 시간(ms) : " << (double)elapsed / freq.QuadPart * 1000 << " ms" << endl;
		cout << "실제로 흘러간 시간(micro) : " << (double)elapsed / freq.QuadPart * 1000 * 1000 << " micro" << endl;
		cout << "랜덤 값(0~100) : " << elapsed % 100 << endl;


	}
	return 0;
}
