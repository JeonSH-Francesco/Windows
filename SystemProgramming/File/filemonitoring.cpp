#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <locale.h>

void HandleError(const char* errorMessage) {
    fprintf(stderr, "%s. Error: %lu\n", errorMessage, GetLastError());
    exit(1);
}

void WatchDirectory(LPCTSTR directory) {
    HANDLE hDir = CreateFile(
        directory,
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );

    if (hDir == INVALID_HANDLE_VALUE) {
        HandleError("CreateFile failed");
    }

    BYTE buffer[1024];
    DWORD bytesReturned;
    FILE_NOTIFY_INFORMATION* fni;
    TCHAR action[256];

    // Create an event object for the timeout
    HANDLE hTimeoutEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (hTimeoutEvent == NULL) {
        HandleError("CreateEvent failed");
    }

    // 잘 먹히는지는 모르겠으나, 1분 설정이..
    HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (hTimer == NULL) {
        HandleError("CreateWaitableTimer failed");
    }
    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -600000000LL; // 1 minute in 100-nanosecond intervals
    if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, FALSE)) {
        HandleError("SetWaitableTimer failed");
    }

    HANDLE handles[] = { hDir, hTimer };

    while (TRUE) {
        DWORD waitStatus = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
        if (waitStatus == WAIT_OBJECT_0) {
         
            if (ReadDirectoryChangesW(
                hDir,
                buffer,
                sizeof(buffer),
                TRUE,
                FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_ATTRIBUTES |
                FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SECURITY,
                &bytesReturned,
                NULL,
                NULL
            )) {
                fni = (FILE_NOTIFY_INFORMATION*)buffer;

                do {
                    switch (fni->Action) {
                    case FILE_ACTION_ADDED:
                        _stprintf_s(action, _T("File added: %.*s"), fni->FileNameLength / sizeof(WCHAR), fni->FileName);
                        break;
                    case FILE_ACTION_REMOVED:
                        _stprintf_s(action, _T("File removed: %.*s"), fni->FileNameLength / sizeof(WCHAR), fni->FileName);
                        break;
                    case FILE_ACTION_MODIFIED:
                        _stprintf_s(action, _T("File modified: %.*s"), fni->FileNameLength / sizeof(WCHAR), fni->FileName);
                        break;
                    case FILE_ACTION_RENAMED_OLD_NAME:
                        _stprintf_s(action, _T("File renamed from: %.*s"), fni->FileNameLength / sizeof(WCHAR), fni->FileName);
                        break;
                    case FILE_ACTION_RENAMED_NEW_NAME:
                        _stprintf_s(action, _T("File renamed to: %.*s"), fni->FileNameLength / sizeof(WCHAR), fni->FileName);
                        break;
                    default:
                        _stprintf_s(action, _T("Unknown action: %.*s"), fni->FileNameLength / sizeof(WCHAR), fni->FileName);
                        break;
                    }
                    _tprintf(_T("%s\n"), action);
                    fni = (FILE_NOTIFY_INFORMATION*)((BYTE*)fni + fni->NextEntryOffset);
                } while (fni->NextEntryOffset != 0);
            }
            else {
                HandleError("ReadDirectoryChangesW failed");
            }
        }
        else if (waitStatus == WAIT_OBJECT_0 + 1) {
            // Timeout
            _tprintf(_T("Timeout: No changes detected in the last minute.\n"));
            break;
        }
        else {
            HandleError("WaitForMultipleObjects failed");
        }
    }

    CloseHandle(hDir);
    CloseHandle(hTimeoutEvent);
    CloseHandle(hTimer);
}

int _tmain() {
    // 로케일 설정
    setlocale(LC_ALL, "korean");

    LPCTSTR directory = _T("C:\\Users\\82107\\바탕 화면\\RansomwareTest1"); // 여기에 감시할 디렉터리 경로를 하드코딩하십시오.

    _tprintf(_T("Watching directory: %s\n"), directory);
    WatchDirectory(directory);

    return 0;
}
