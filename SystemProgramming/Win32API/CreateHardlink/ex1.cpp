#include <Windows.h>
#include <iostream>
using namespace std;

//Hard Link
//->A file can have two different names.
//Hard Link only created for File not for Directory.
//If you delete the original file, then you will be access the hard link file.
//It's created in a memory(Physical memory.)

int main() {
	BOOL bHfile;
	bHfile = CreateHardLink(L"C:\\wintest\\hardlink.txt",L"C:\\wintest\\testfile.txt",NULL);

	if (bHfile == FALSE) {
		cout << "CreateHardlink failed & Error no =" << GetLastError() << endl;
	}
	cout << "CreateHardlink Success." << endl;
	return 0;
}
