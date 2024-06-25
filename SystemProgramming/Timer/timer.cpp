#include <stdio.h>
#include <time.h>
#include <windows.h>

int main() {
    while (1) {
        // 현재 시각 가져오기
        time_t now = time(NULL);
        struct tm currentTime;
        localtime_s(&currentTime, &now);

        // 현재 시각을 디지털 시계 형식으로 출력
        printf("\r%02d:%02d:%02d", currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec);
        fflush(stdout);

        // 1초 대기
        Sleep(1000);
    }

    return 0;
}
