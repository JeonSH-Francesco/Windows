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
    // 함수 포인터 배열 선언 및 초기화
    int(__cdecl * operationTable[4])(int, int) = {
        add, subtract, multiply, divide
    };

    int input = 0;
    int a, b;

    do {
        // 사용자로부터 연산 선택 및 두 숫자 입력 받기
        printf("Select operation: 1(Add), 2(Subtract), 3(Multiply), 4(Divide), 0(Exit): ");
        scanf_s("%d", &input);

        if (input > 0 && input < 5) {
            printf("Enter two integers (a b): ");
            scanf_s("%d %d", &a, &b);

            // 선택된 연산 수행
            int result = operationTable[input - 1](a, b);
            printf("Result: %d\n", result);
        }
        else if (input != 0) {
            printf("Invalid input. Try again.\n");
        }

    } while (input != 0);

    printf("Program exited.\n");
    return 0;
}
