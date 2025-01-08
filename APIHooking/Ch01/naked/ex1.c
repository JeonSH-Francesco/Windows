#include <stdio.h>

__declspec(naked) void testFunc(void)
{
    puts("testFunc()");
    printf("a: %d\n", 10);
}

int main()
{
    testFunc();
    return 0;
}

