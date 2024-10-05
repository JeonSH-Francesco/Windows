#include <iostream>
#include <winsock2.h>
#include <iphlpapi.h>
//이 코드는 IP Helper API의 헤더 파일을 포함
//IP Helper는 Windows System에서 Network Adapter와 관련된 정보를 제공하는 API의 모음이다.
#pragma comment(lib, "iphlpapi.lib")
//컴파일러에게 iphlpapi.lib 라이브러리를 링크하라고 지시합니다.
//GetAdapterInfo같은 IP Helper API 함수들은 헤더파일에 정의되어 있지만, 실제로 함수가 동작하려면
//해당 라이브러리가 연결되어야 함
#pragma comment(lib, "ws2_32.lib")
//ws2_32.lib는 Winsock 2 API를 위한 라이브러리
//네트워크와 관련된 작업을 할 때, IP 주소나 소켓 통신 등을 처리하기 위해 Winsock API가 사용되는데, 
//이를 동작시키기 위해서는 ws2_32.lib 라이브러리를 링크

//usage : #pragma comment(type, "name")
//라이브러리 파일을 명시적으로 링커에 포함시킵니다. 
//이를 통해 소스 파일 안에서 바로 라이브러리를 링크할 수 있으며, 빌드 설정에서 별도로 설정할 필요가 없습니다.

using namespace std;

void GetNetworkInfo()
{
    // 어댑터 정보 구조체 할당
    IP_ADAPTER_INFO adapterInfo[16];
    DWORD bufLen = sizeof(adapterInfo);

    // 어댑터 정보를 가져옴
    DWORD status = GetAdaptersInfo(adapterInfo, &bufLen);
    if (status != ERROR_SUCCESS) {
        cerr << "GetAdaptersInfo failed with error: " << status << endl;
        return;
    }

    // 첫 번째 어댑터 정보 가져오기
    PIP_ADAPTER_INFO pAdapter = adapterInfo;
    while (pAdapter) {
        cout << "Adapter Name: " << pAdapter->AdapterName << endl;
        cout << "IP Address: " << pAdapter->IpAddressList.IpAddress.String << endl;
        cout << "Subnet Mask: " << pAdapter->IpAddressList.IpMask.String << endl;
        cout << endl;
        pAdapter = pAdapter->Next;
    }
}

int main()
{
    GetNetworkInfo();
    return 0;
}
