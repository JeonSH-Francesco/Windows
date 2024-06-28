
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define PIPE_NAME TEXT("\\\\.\\pipe\\SamplePipe")

int _tmain(int argc, TCHAR* argv[]) {
    HANDLE hPipe;
    // TCHAR을 사용하여 메시지를 정의합니다.
    TCHAR lpvMessage[] = TEXT("Hello from the sender!");
    BOOL fSuccess = FALSE;
    DWORD cbWritten;

    // Attempt to connect to the named pipe
    hPipe = CreateFile(
        PIPE_NAME,           // pipe name
        GENERIC_WRITE,       // write access
        0,                   // no sharing
        NULL,                // default security attributes
        OPEN_EXISTING,       // opens existing pipe
        0,                   // default attributes
        NULL);               // no template file

    if (hPipe == INVALID_HANDLE_VALUE) {
        _tprintf(TEXT("CreateFile failed, GLE=%d.\n"), GetLastError());
        return -1;
    }

    // Send a message to the pipe server
    fSuccess = WriteFile(
        hPipe,                  // pipe handle
        lpvMessage,             // message
        (lstrlen(lpvMessage) + 1) * sizeof(TCHAR), // message length
        &cbWritten,             // bytes written
        NULL);                  // not overlapped

    if (!fSuccess) {
        _tprintf(TEXT("WriteFile to pipe failed, GLE=%d.\n"), GetLastError());
        return -1;
    }

    _tprintf(TEXT("Message sent to server: %s\n"), lpvMessage);

    CloseHandle(hPipe);

    return 0;
}
