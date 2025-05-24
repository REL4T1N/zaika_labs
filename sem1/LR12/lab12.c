#include <stdio.h>

int other_code(int num){
    int rev_num = 10, inv_num;
    if (num >= 0) 
        return num;
    else {
        inv_num = num;
        while (inv_num != 0) {
            rev_num *= 10;
            inv_num /= 10;
        }
        return rev_num + num;
    }
}

int main() {
    int number;
    while (scanf("%d", &number) == 1) {
        printf("%d\n", other_code(number));
    }
}