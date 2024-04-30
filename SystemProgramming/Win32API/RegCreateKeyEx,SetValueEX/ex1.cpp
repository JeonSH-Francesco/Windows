#include <iostream>
#include <Windows.h>
using namespace std;

/*The registry contains following information :
Windows version number, build number& registered user.
Information about the computer's processor type.
User specific information.
Security information.
Installed Services.
File name and extension.*/

//HKLM : it stores physical info about machine along with installed software.
//HKEY_USERS : It defines user configuration information.
//HKEY_CURRENT_CONFIG : It contains current settings of user such as font and resolution.
//HKEY_CLASSES_ROOT : It contains subordinate entries to define mappings from file extension to classes.
//HKEY_CURRENT_USER : It contains user specific information.

int main() {
	LONG lReg;
	HKEY hKey;
	DWORD dwData = 1234;
	lReg = RegCreateKeyEx(HKEY_LOCAL_MACHINE,L"Software\\Test\\Product\\SmartId",
		0,NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY,
		NULL, &hKey,NULL);

	if (lReg != ERROR_SUCCESS) {
		cout << "RegSetValueEx creation failed & Error No. " << GetLastError() << endl;
	}
	cout << "RegSetValueEX creation Success" << endl;

	lReg = RegSetValueEx(hKey, L"VALUE", NULL, REG_DWORD,(LPBYTE)&dwData,sizeof(DWORD));
	if (lReg != ERROR_SUCCESS) {
		cout << "Registry creation failed & Error No. " << GetLastError() << endl;
	}
	cout << "Registry creation Success" << endl;
	RegCloseKey(hKey);
	return 0;
}
