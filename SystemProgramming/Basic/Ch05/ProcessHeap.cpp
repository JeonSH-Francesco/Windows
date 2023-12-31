#include <iostream>
#include <Windows.h>
#include <tchar.h>

int main()
{
    //현재 실행 중인 프로세스의 기본 힙 핸들을 얻는 함수 : GetProcessHeap()
    //이미 생성된 기본 힙을 사용하므로 별도의 힙을 생성하지 않습니다.
    //따라서 별도의 힙 핸들 생성 및 파괴가 필요하지 않습니다.
    //HANDLE hProcHeap = ::GetProcessHeap();
    HANDLE hProcHeap = ::HeapCreate(0, 0, 0);
    //HeapCreate함수를 사용하여 새로운 프로세스 힙을 생성한다.
    //이 힙은 기본힙과는 다르게 별도로 생성되며 사용자가 직접 관리하고 파괴해야 합니다.

    //4byte. 4MB 할당
    void* pMem1 = ::HeapAlloc(hProcHeap, 0, 4);
    void* pMem2 = ::HeapAlloc(hProcHeap, 0, 1024 * 1024 * 4);

    //할당된 힙들의 정보들을 가지고 있는 것이 PROCESS_HEAP_ENTRY->초기화
    PROCESS_HEAP_ENTRY Entry = { 0 };
    //프로세스 힙의 현재 상태를 확인하는 방법
    //힙을 순회하면서 각 메모리 블록의 정보를 얻는다.
    while (::HeapWalk(hProcHeap, &Entry) != FALSE)
    { //PROCESS_HEAP_ENTRY의 wFlags를 통해 현재 메모리 블록의 상태를 확인
        if ((Entry.wFlags & PROCESS_HEAP_ENTRY_BUSY) != 0) {
            _tprintf(TEXT("Allocated block"));
            //할당된 메모리 블록

            if ((Entry.wFlags & PROCESS_HEAP_ENTRY_MOVEABLE) != 0) {
                _tprintf(TEXT(", movable with HANDLE %#p"), Entry.Block.hMem);
                //해당 블록은 이동 가능
            }

            if ((Entry.wFlags & PROCESS_HEAP_ENTRY_DDESHARE) != 0) {
                _tprintf(TEXT(", DDESHARE"));
                //DDE(Dynamic Data Exchange) 공유를 사용하는 블록
                //윈도우 시스템에서 프로세스 간 데이터 교환을 위한 메커니즘을 제공하는 기술
                // 데이터가 한 응용 프로그램에서 다른 응용 프로그램으로 전달되어 특정 작업을 수행하도록 유도할 수 있습니다. 
                //이는 일반적으로 클립보드를 통해 데이터를 교환하거나, 서로 다른 응용 프로그램 간에 특정 명령을 전송하는 데 사용될 수 있습니다.
            }
        }
        //REGION은 연속된 메모리 블록의 집합을 나타냅니다.
        else if ((Entry.wFlags & PROCESS_HEAP_REGION) != 0) {
            _tprintf(TEXT("Region\n  %d bytes committed\n") \
                TEXT("  %d bytes uncommitted\n  First block address: %#p\n") \
                TEXT("  Last block address: %#p\n"),
                Entry.Region.dwCommittedSize,
                Entry.Region.dwUnCommittedSize,
                Entry.Region.lpFirstBlock,
                Entry.Region.lpLastBlock);
        }
        else if ((Entry.wFlags & PROCESS_HEAP_UNCOMMITTED_RANGE) != 0) {
            _tprintf(TEXT("Uncommitted range\n"));
            //현재 메모리 블록은 할당되지 않은 영역(미할당 영역)
        }
        else {
            _tprintf(TEXT("Block\n"));
            //단순한 메모리 블록
        }

        //각 메모리 블록에 대한 세부 정보 출력-> 데이터 부분의 시작 주소, 크기, 오버헤드, REGION INDEX 출력
        _tprintf(TEXT("  Data portion begins at: %#p\n  Size: %d bytes\n") \
            TEXT("  Overhead: %d bytes\n  Region index: %d\n\n"),
            Entry.lpData,
            Entry.cbData,
            Entry.cbOverhead,
            Entry.iRegionIndex);
    }

    ::HeapFree(hProcHeap, 0, pMem1);
    ::HeapFree(hProcHeap, 0, pMem2);
    //할당된 메모리 해제
    ::HeapDestroy(hProcHeap);
    //힙을 파괴
}
