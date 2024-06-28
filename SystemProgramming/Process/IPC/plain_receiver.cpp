#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define PIPE_NAME TEXT("\\\\.\\pipe\\SamplePipe")
#define BUFFER_SIZE 512

int _tmain(int argc, TCHAR* argv[]) {
    HANDLE hPipe;
    TCHAR chBuf[BUFFER_SIZE];
    BOOL fSuccess = FALSE;
    DWORD cbRead;

    // Create a named pipe
    hPipe = CreateNamedPipe(
        PIPE_NAME,             // pipe name
        PIPE_ACCESS_INBOUND,   // read access
        PIPE_TYPE_MESSAGE |    // message type pipe
        PIPE_READMODE_MESSAGE |// message-read mode
        PIPE_WAIT,             // blocking mode
        PIPE_UNLIMITED_INSTANCES, // max instances
        BUFFER_SIZE,           // output buffer size
        BUFFER_SIZE,           // input buffer size
        0,                     // client time-out
        NULL);                 // default security attribute

    if (hPipe == INVALID_HANDLE_VALUE) {
        _tprintf(TEXT("CreateNamedPipe failed, GLE=%d.\n"), GetLastError());
        return -1;
    }

    // Wait for the client to connect
    fSuccess = ConnectNamedPipe(hPipe, NULL) ?
        TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

    if (!fSuccess) {
        _tprintf(TEXT("ConnectNamedPipe failed, GLE=%d.\n"), GetLastError());
        CloseHandle(hPipe);
        return -1;
    }

    // Read the message from the pipe
    fSuccess = ReadFile(
        hPipe,    // pipe handle
        chBuf,    // buffer to receive data
        BUFFER_SIZE * sizeof(TCHAR), // buffer size
        &cbRead,  // number of bytes read
        NULL);    // not overlapped I/O

    if (!fSuccess && GetLastError() != ERROR_MORE_DATA) {
        _tprintf(TEXT("ReadFile from pipe failed, GLE=%d.\n"), GetLastError());
        CloseHandle(hPipe);
        return -1;
    }

    _tprintf(TEXT("Message received from client: %s\n"), chBuf);

    CloseHandle(hPipe);

    return 0;
}
