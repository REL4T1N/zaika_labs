#include <stdio.h>
#include <ctype.h>

typedef unsigned int uint;
#define vowels (1u<<('a'-'a') | 1u<<('e'-'a') | 1u<<('i'-'a') | 1u<<('o'-'a') | 1u<<('u'-'a') | 1u<<('y'-'a'))


int is_sep(int c) {
    return c == ' ' || c == ',' || c == '\t' || c == '\n' || c == EOF;
}

uint char_to_set(int c) {
    c = tolower(c);
    return (c < 'a' || c > 'z') ? 0 : 1u << (c - 'a');
}

 
int main() {
    int c;
    int state = 0, count = 0, flag = 1;
    uint let = 0, final = vowels;
    do {
        c = getchar();
        if (count == 0 && c == EOF) 
            flag = 0;
        else 
            count++;

        if (state == 0) { 
            if (!is_sep(c)) {
                state = 1;
                let |= char_to_set(c);
            }
        }
        else if (state == 1) {
            if (is_sep(c)) {
                state = 0;
                final &= let;
                let = 0;
            }
            else if (!is_sep(c)) {
                let |= char_to_set(c);
            }
        }
    } while (c != EOF);
    
    if (!flag) 
        return 0;

    for (int a = 'a'; a <= 'z'; a++) {
        if ((char_to_set(a) & vowels) && (char_to_set(a) & final))
            printf("Гласная, которая есть везде: %c\n", a);
    }
    return 0;
}