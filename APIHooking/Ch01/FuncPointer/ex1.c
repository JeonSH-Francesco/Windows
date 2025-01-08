#include <stdio.h>

// __cdecl 호출 규약을 사용하는 덧셈 기능의 함수
int __cdecl add(int a, int b) {
    printf("add(%d, %d) called\n", a, b);
    return a + b;
}

// __cdecl 호출 규약을 사용하는 뺄셈 기능의 함수
int __cdecl subtract(int a, int b) {
    printf("subtract(%d, %d) called\n", a, b);
    return a - b;
}

// __cdecl 호출 규약을 사용하는 곱셈 기능의 함수
int __cdecl multiply(int a, int b) {
    printf("multiply(%d, %d) called\n", a, b);
    return a * b;
}

// __cdecl 호출 규약을 사용하는 나눗셈 기능의 함수
int __cdecl divide(int a, int b) {
    if (b == 0) {
        printf("Error: Division by zero is undefined!\n");
        return 0;  // 예외 상황에서 0 반환
    }
    printf("divide(%d, %d) called\n", a, b);
    return a / b;
}

int main() {
    // 함수 포인터 선언 및 초기화
    int(__cdecl * operation)(int, int);

    // 덧셈 호출
    operation = add;
    int result1 = operation(10, 5);
    printf("Result of add: %d\n", result1);

    // 뺄셈 호출
    operation = subtract;
    int result2 = operation(10, 5);
    printf("Result of subtract: %d\n", result2);

    // 곱셈 호출
    operation = multiply;
    int result3 = operation(10, 5);
    printf("Result of multiply: %d\n", result3);

    // 나눗셈 호출 (정상적인 경우)
    operation = divide;
    int result4 = operation(10, 5);
    printf("Result of divide: %d\n", result4);

    // 나눗셈 호출 (0으로 나누는 경우)
    int result5 = operation(10, 0);
    printf("Result of divide by zero: %d\n", result5);

    return 0;
}
