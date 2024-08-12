/*MailSlots is another IPC Mechanism.

Used as one way communication.

Mailslots genrally used for Transmit to Short messages i.e.Datagram.

Mailslots can be used either locally or over the network.

Mailslots is a Client - Server interface.

In Mailslots, Only Client wirte the message which is stored on mailslots and later Server
can retrieve or read the messages.

Generally Mailslots is message exchanging communication.

Mailslots does not provide any confirmation that a message has been received.

we can send Only 424 bytes message.

Server -> CreateMailslots, ReadFile, CloseHandle

Client -> CreateFile, WriteFile, CloseHandle


*/

#include <Windows.h>
#include <iostream>
using namespace std;

int main() {

    cout << "\t\t ... MailSlots Server ..." << endl;

    // CreateMailSlots Local Variable
    HANDLE hSlots;
    // ReadFile Local Variable
    BOOL bReadFile;
    DWORD dwNoBytesRead;
    char szReadFileBuffer[1023];  // Fix the typo in the variable name
    DWORD dwReadFileBufferSize = sizeof(szReadFileBuffer);

    // Step 1: CreateMailSlots def.
    hSlots = CreateMailslot(
        L"\\\\.\\mailslot\\MYMAILSLOTS",  // Fix the path
        0,
        MAILSLOT_WAIT_FOREVER,
        NULL
    );

    if (hSlots == INVALID_HANDLE_VALUE)
    {
        cout << "MailSlots Creation Failed & Error No - " << GetLastError() << endl;
        return 1;  // Exit the program if the MailSlot creation failed
    }
    cout << "MailSlots Creation Success" << endl;

    // Step 2: ReadFile def.
    bReadFile = ReadFile(
        hSlots,
        szReadFileBuffer,
        dwReadFileBufferSize,
        &dwNoBytesRead,
        NULL
    );

    if (bReadFile == FALSE) {
        cout << "ReadFile Failed & Error No - " << GetLastError() << endl;
        CloseHandle(hSlots);  // Close the handle before exiting
        return 1;
    }

    cout << "ReadFile Success" << endl;
    cout << "Data Reading from mailslots client: " << szReadFileBuffer << endl;

    CloseHandle(hSlots);

    system("PAUSE");
    return 0;
}
