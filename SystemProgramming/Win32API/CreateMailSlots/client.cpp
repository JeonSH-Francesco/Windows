#include <Windows.h>
#include <iostream>
#include <cstring> // For strlen and strcmp
#include <cstdio>  // For fgets

using namespace std;

int main() {
    cout << "\t\t ... MailSlots Client ..." << endl;

    // CreateFile Local Variable
    HANDLE hFile;
    // WriteFile Local Variable
    BOOL bWriteFile;
    DWORD dwNoBytesWritten;
    char szWriteFileBuffer[1023];
    DWORD dwWriteFileBufferSize;

    // Step 1: CreateFile to open the MailSlot created by the server
    hFile = CreateFile(
        L"\\\\.\\mailslot\\MYMAILSLOTS",
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        cout << "File Creation Failed & Error No - " << GetLastError() << endl;
        return 1;
    }
    cout << "File Creation Success" << endl;

    cout << "Enter messages to send. Type 'q' to quit." << endl;

    while (true) {
        // Get user input
        cout << "Message: ";

        // Using fgets to read user input
        if (fgets(szWriteFileBuffer, sizeof(szWriteFileBuffer), stdin) == NULL) {
            cout << "Failed to read input." << endl;
            CloseHandle(hFile);
            return 1;
        }

        // Remove newline character if present
        size_t len = strlen(szWriteFileBuffer);
        if (len > 0 && szWriteFileBuffer[len - 1] == '\n') {
            szWriteFileBuffer[len - 1] = '\0';
        }

        // Calculate the actual length of the input message
        dwWriteFileBufferSize = strlen(szWriteFileBuffer) + 1; // +1 for the null terminator

        // Step 2: WriteFile to write data into the MailSlot
        bWriteFile = WriteFile(
            hFile,
            szWriteFileBuffer,
            dwWriteFileBufferSize,
            &dwNoBytesWritten,
            NULL
        );

        if (bWriteFile == FALSE) {
            cout << "WriteFile failed & Error No - " << GetLastError() << endl;
            CloseHandle(hFile);
            return 1;
        }

        // Check if the message is 'q' to quit
        if (strcmp(szWriteFileBuffer, "q") == 0) {
            cout << "Termination signal sent. Exiting..." << endl;
            break;
        }

        cout << "WriteFile Success" << endl;
    }

    // Step 3: Close the handle to the MailSlot
    CloseHandle(hFile);
    return 0;
}
