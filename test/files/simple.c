#include <stdio.h>

int main() {
    int num;
    printf("Enter an integer: ");
    scanf("%d", &num);

    char *s_tests = L"Hello World";
    num = num + 1;
    if (num != 0) {
        printf("number bigger than 0.\n");
    }
    // True if the num is even
    if(num % 2 == 0 || num % 3 != 0)
        printf("%d is even.", num);
    else
        printf("%d is odd.", num);
    
    return 0;
}
