#include <Windows.h>
#include <iostream>
using namespace std;
//Soft Lnk
//A file can have different name.
//Soft link can create for file & Directory.
//If you delete the original file, then you can not access the soft link file.
//Soft lnik is a shortcut of origianl file.
int main() {
	BOOL bHfile;
	bHfile = CreateSymbolicLink(L"C:\\wintest\\softlinkfile.txt",L"C:\\wintest\\SFtest.txt",0);

	if (bHfile == FALSE) {
		cout << "CreateSymbolickLink Failed & Error No =" << GetLastError() << endl;

	}
	cout << "CreateSymbolicLink Success" << endl;

	return 0;
}
