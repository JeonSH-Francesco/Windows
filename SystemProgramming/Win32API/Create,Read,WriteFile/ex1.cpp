#include <Windows.h>
#include <iostream>
using namespace std;

int main(){
	HANDLE hFile;
	BOOL bFile;
	char chBuffer[] = "This is the Sample program of Write & Read API";
	DWORD dwNoByteToWrite = strlen(chBuffer);
	DWORD dwNoByteWritten = 0;
	DWORD dwNoByteToRead = strlen(chBuffer);
	DWORD dwNoByteRead = 0;

	hFile = CreateFile(L"C:\\wintest\\createfiletest.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	//Check Condition if fails
	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "CreateFile Failed" << endl;

	}
	cout << "CreateFile Success " << endl;
	
	//WriteFile
	bFile = WriteFile(hFile, chBuffer, dwNoByteToWrite, &dwNoByteWritten, NULL);

	if (bFile == FALSE) {
		cout << "WriteFile Failed" << GetLastError() << endl;
	}
	cout << "WriteFile Success" << endl;
	
	//ReadFile
	bFile = ReadFile(hFile, chBuffer,dwNoByteToRead, &dwNoByteRead,NULL);
	if (bFile == FALSE) {
		cout << "ReadFile Success"<< endl;
	}
	cout << "DATA READ FROM FILE ->" << chBuffer << endl;

	//Closing the Handle
	CloseHandle(hFile);
	return 0;
}
