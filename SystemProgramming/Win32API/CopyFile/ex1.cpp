#include <windows.h>
#include <iostream>
using namespace std;

int main() {
	BOOL bFile;
	bFile = CopyFile(L"C:\\wintest\\hello.txt",L"C:\\wintest\\hello1.txt",TRUE);

	if (bFile == FALSE) {
		cout << "CopyFile Failed & Erro no" << GetLastError() << endl;
	}

	cout << "CopyFile Success"<< endl;
	return 0;
}
