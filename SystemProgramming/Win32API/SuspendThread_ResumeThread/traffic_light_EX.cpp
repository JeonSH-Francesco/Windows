#include <iostream>
#include <thread>
#include <chrono>
#include <Windows.h>

using namespace std;

HANDLE hCarThread;
HANDLE hPedestrianThread;
HANDLE hTrafficLightThread;
bool redLightActive = true;
bool greenLightActive = false;
bool yellowLightActive = false;

DWORD WINAPI TrafficLightControl(LPVOID param) {
    int cycleCount = 0;  // 반복 주기 횟수 조정
    auto startTime = chrono::steady_clock::now();  // 시작 시간 기록

    while (true) {
        auto currentTime = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();

        if (elapsed >= 15) {  // 15초 후 자동 종료
            cout << "15 seconds elapsed. Stopping traffic light control." << endl;
            break;
        }

        // RED 상태 시작
        redLightActive = true;
        greenLightActive = false;
        yellowLightActive = false;
        cout << "<<<Traffic Light: RED (Stop)>>>" << endl;
        int elapsedTime = 0;
        while (elapsedTime < 5) {
            // 차량 스레드를 실행하고 보행자 스레드를 일시 중지
            SuspendThread(hPedestrianThread);
            cout << endl;
            cout << "---------------------------" << endl;
            cout << "SuspendThread called for Pedestrian Thread" << endl;
            ResumeThread(hCarThread);
            cout << "ResumeThread called for Car Thread" << endl;
            cout << "---------------------------" << endl;
            cout << "Car is passing through and Pedestrian is stopped. (RED)" << endl;
            Sleep(1000);
            elapsedTime++;
        }

        // YELLOW 상태 전환
        redLightActive = false;
        greenLightActive = false;
        yellowLightActive = true;
        cout << endl;
        cout << "<<<Traffic Light: YELLOW (Caution)>>>" << endl;
        elapsedTime = 0;
        while (elapsedTime < 3) {
            // 차량 스레드를 일시 중지하고 보행자 스레드를 실행
            SuspendThread(hCarThread);
            cout << endl;
            cout << "---------------------------" << endl;
            cout << "SuspendThread called for Car Thread" << endl;
            SuspendThread(hPedestrianThread);
            cout << "SuspendThread called for Pedestrain Thread" << endl;
            cout << "---------------------------" << endl;
            cout << "Wait for the red light. (YELLOW)" << endl;
            Sleep(1000);
            elapsedTime++;
        }

        // GREEN 상태 전환
        redLightActive = false;
        greenLightActive = true;
        yellowLightActive = false;
        cout << endl;
        cout << "<<<Traffic Light: GREEN (Go)>>>" << endl;
        elapsedTime = 0;
        while (elapsedTime < 8) {
            // 보행자 스레드를 일시 중지하고 차량 스레드를 실행
            SuspendThread(hCarThread);
            cout << endl;
            cout << "---------------------------" << endl;
            cout << "SuspendThread called for Car Thread" << endl;
            ResumeThread(hPedestrianThread);
            cout << "ResumeThread called for Pedestrain Thread" << endl;
            cout << "---------------------------" << endl;
            cout << "Car is stopped and Pedestrian is crossing. (GREEN)" << endl;
            Sleep(1000);
            elapsedTime++;
        }

        cycleCount++;
    }

    // 트래픽 라이트 제어 완료
    cout << "Traffic light control completed." << endl;
    return 0;
}

DWORD WINAPI CarThread(LPVOID param) {
    while (true) {
        if (greenLightActive) {
            // GREEN 상태에서 차량은 멈춤
            Sleep(1000);
        }
        else if (redLightActive) {
            // RED 상태에서 차량이 통과
            Sleep(1000);
        }
    }
    return 0;
}

DWORD WINAPI PedestrianThread(LPVOID param) {
    while (true) {
        if (greenLightActive) {
            // GREEN 상태에서 보행자가 건너는 중
            Sleep(1000);
        }
        else if (redLightActive) {
            // RED 상태에서 보행자는 멈춤
            Sleep(1000);
        }
    }
    return 0;
}

int main() {
    // 트래픽 라이트 제어 스레드 생성
    hTrafficLightThread = CreateThread(NULL, 0, TrafficLightControl, NULL, 0, NULL);

    // 차량 스레드 생성
    hCarThread = CreateThread(NULL, 0, CarThread, NULL, 0, NULL);

    // 보행자 스레드 생성
    hPedestrianThread = CreateThread(NULL, 0, PedestrianThread, NULL, 0, NULL);

    // 트래픽 라이트 제어 스레드 종료 대기
    WaitForSingleObject(hTrafficLightThread, INFINITE);

    // 모든 스레드 종료
    CloseHandle(hTrafficLightThread);
    CloseHandle(hCarThread);
    CloseHandle(hPedestrianThread);

    return 0;
}

/*
result : 

<<<Traffic Light: RED (Stop)>>>

---------------------------
SuspendThread called for Pedestrian Thread
ResumeThread called for Car Thread
---------------------------
Car is passing through and Pedestrian is stopped. (RED)

---------------------------
SuspendThread called for Pedestrian Thread
ResumeThread called for Car Thread
---------------------------
Car is passing through and Pedestrian is stopped. (RED)

---------------------------
SuspendThread called for Pedestrian Thread
ResumeThread called for Car Thread
---------------------------
Car is passing through and Pedestrian is stopped. (RED)

---------------------------
SuspendThread called for Pedestrian Thread
ResumeThread called for Car Thread
---------------------------
Car is passing through and Pedestrian is stopped. (RED)

---------------------------
SuspendThread called for Pedestrian Thread
ResumeThread called for Car Thread
---------------------------
Car is passing through and Pedestrian is stopped. (RED)

<<<Traffic Light: YELLOW (Caution)>>>

---------------------------
SuspendThread called for Car Thread
SuspendThread called for Pedestrain Thread
---------------------------
Wait for the red light. (YELLOW)

---------------------------
SuspendThread called for Car Thread
SuspendThread called for Pedestrain Thread
---------------------------
Wait for the red light. (YELLOW)

---------------------------
SuspendThread called for Car Thread
SuspendThread called for Pedestrain Thread
---------------------------
Wait for the red light. (YELLOW)

<<<Traffic Light: GREEN (Go)>>>

---------------------------
SuspendThread called for Car Thread
ResumeThread called for Pedestrain Thread
---------------------------
Car is stopped and Pedestrian is crossing. (GREEN)

---------------------------
SuspendThread called for Car Thread
ResumeThread called for Pedestrain Thread
---------------------------
Car is stopped and Pedestrian is crossing. (GREEN)

---------------------------
SuspendThread called for Car Thread
ResumeThread called for Pedestrain Thread
---------------------------
Car is stopped and Pedestrian is crossing. (GREEN)

---------------------------
SuspendThread called for Car Thread
ResumeThread called for Pedestrain Thread
---------------------------
Car is stopped and Pedestrian is crossing. (GREEN)

---------------------------
SuspendThread called for Car Thread
ResumeThread called for Pedestrain Thread
---------------------------
Car is stopped and Pedestrian is crossing. (GREEN)

---------------------------
SuspendThread called for Car Thread
ResumeThread called for Pedestrain Thread
---------------------------
Car is stopped and Pedestrian is crossing. (GREEN)

---------------------------
SuspendThread called for Car Thread
ResumeThread called for Pedestrain Thread
---------------------------
Car is stopped and Pedestrian is crossing. (GREEN)

---------------------------
SuspendThread called for Car Thread
ResumeThread called for Pedestrain Thread
---------------------------
Car is stopped and Pedestrian is crossing. (GREEN)
15 seconds elapsed. Stopping traffic light control.
Traffic light control completed.

*/
