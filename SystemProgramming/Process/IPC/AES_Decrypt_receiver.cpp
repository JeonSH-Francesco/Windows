#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <wincrypt.h>

#pragma comment(lib, "advapi32.lib")

#define PIPE_NAME TEXT("\\\\.\\pipe\\SamplePipe")
#define BUFFER_SIZE 512

void HandleError(const char* errorMessage) {
    fprintf(stderr, "%s. Error: %lu\n", errorMessage, GetLastError());
    exit(1);
}

void DecryptMessage(BYTE* encryptedMessage, DWORD encryptedMessageLen, LPTSTR* decryptedMessage) {
    HCRYPTPROV hProv;
    HCRYPTKEY hKey;
    HCRYPTHASH hHash;

    // Acquire a cryptographic provider context handle
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        HandleError("CryptAcquireContext failed");
    }

    // Create a hash object
    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        HandleError("CryptCreateHash failed");
    }

    // Hash the password (in this case, "password")
    if (!CryptHashData(hHash, (BYTE*)"password", 8, 0)) {
        HandleError("CryptHashData failed");
    }

    // Derive an AES key from the hash object
    if (!CryptDeriveKey(hProv, CALG_AES_256, hHash, 0, &hKey)) {
        HandleError("CryptDeriveKey failed");
    }

    // Decrypt the message
    DWORD messageLen = encryptedMessageLen;
    if (!CryptDecrypt(hKey, 0, TRUE, 0, encryptedMessage, &messageLen)) {
        HandleError("CryptDecrypt failed");
    }

    // Allocate memory for the decrypted message
    *decryptedMessage = (LPTSTR)malloc(messageLen);
    memcpy(*decryptedMessage, encryptedMessage, messageLen);

    // Cleanup
    CryptDestroyKey(hKey);
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
}

int _tmain(int argc, TCHAR* argv[]) {
    HANDLE hPipe;
    BYTE chBuf[BUFFER_SIZE] = {};
    LPTSTR decryptedMessage;
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

    // Read the encrypted message from the pipe
    fSuccess = ReadFile(
        hPipe,    // pipe handle
        chBuf,    // buffer to receive data
        BUFFER_SIZE, // buffer size
        &cbRead,  // number of bytes read
        NULL);    // not overlapped I/O

    if (!fSuccess && GetLastError() != ERROR_MORE_DATA) {
        _tprintf(TEXT("ReadFile from pipe failed, GLE=%d.\n"), GetLastError());
        CloseHandle(hPipe);
        return -1;
    }

    // Decrypt the message
    DecryptMessage(chBuf, cbRead, &decryptedMessage);

    _tprintf(TEXT("Message received from client: %s\n"), decryptedMessage);
    CloseHandle(hPipe);
    free(decryptedMessage);

    return 0;
}

