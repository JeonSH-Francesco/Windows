#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <wincrypt.h>

#pragma comment(lib, "advapi32.lib")

#define PIPE_NAME TEXT("\\\\.\\pipe\\SamplePipe")

void HandleError(const char* errorMessage) {
    fprintf(stderr, "%s. Error: %lu\n", errorMessage, GetLastError());
    exit(1);
}

void EncryptMessage(LPCTSTR message, BYTE** encryptedMessage, DWORD* encryptedMessageLen) {
    HCRYPTPROV hProv = 0;
    HCRYPTKEY hKey = 0;
    HCRYPTHASH hHash = 0;
    DWORD messageLen = (DWORD)(_tcslen(message) + 1) * sizeof(TCHAR);

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

    // Determine the required size for the encrypted message
    DWORD bufferLen = messageLen;
    if (!CryptEncrypt(hKey, 0, TRUE, 0, NULL, &bufferLen, 0)) {
        HandleError("CryptEncrypt size determination failed");
    }

    // Allocate memory for the encrypted message
    *encryptedMessage = (BYTE*)malloc(bufferLen);
    if (*encryptedMessage == NULL) {
        HandleError("Memory allocation failed");
    }

    // Copy the message to the encrypted buffer
    memcpy(*encryptedMessage, message, messageLen);

    // Encrypt the message
    if (!CryptEncrypt(hKey, 0, TRUE, 0, *encryptedMessage, &messageLen, bufferLen)) {
        HandleError("CryptEncrypt failed");
    }

    *encryptedMessageLen = bufferLen;

    // Cleanup
    CryptDestroyKey(hKey);
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
}

int _tmain(int argc, TCHAR* argv[]) {
    HANDLE hPipe;
    LPCTSTR lpvMessage = TEXT("Hello from the sender!");
    BYTE* encryptedMessage = NULL;
    DWORD encryptedMessageLen = 0;
    BOOL fSuccess = FALSE;
    DWORD cbWritten = 0;

    // Encrypt the message
    EncryptMessage(lpvMessage, &encryptedMessage, &encryptedMessageLen);

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

    // Send the encrypted message to the pipe server
    fSuccess = WriteFile(
        hPipe,                  // pipe handle
        encryptedMessage,       // encrypted message
        encryptedMessageLen,    // message length
        &cbWritten,             // bytes written
        NULL);                  // not overlapped

    if (!fSuccess) {
        _tprintf(TEXT("WriteFile to pipe failed, GLE=%d.\n"), GetLastError());
        return -1;
    }

    _tprintf(TEXT("Message sent to server\n"));

    CloseHandle(hPipe);
    free(encryptedMessage);

    return 0;
}

