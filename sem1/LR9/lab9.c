#include <stdio.h>

int max(int x, int y) {
    if (x >= y) return x;
    else return y;
}

int min(int x, int y) {
    if (x <= y) return x;
    else return y;
}

int abs(int x) {
    return (x >= 0) ? x : (0 - x);
}

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int location(int x, int y) {
    return (((x - 20) * (x - 20) )) + ((y * y) * 4) <= 100;
}

int div(int x, int y) {
    if (x == y * (x / y)) return x / y;
    else if (sign(x) == sign(y)) return x / y;
    else return x / y - 1;
}
int modulo(int x, int y) {
    int z = div(x, y);
    return x - z * y;
}

int main() {
int a, b;
while (2 == scanf("%d%d", &a, &b)) {
    printf("%d %d\n\n", div(a, b), modulo(a, b));
}

    int i_0 = -24, j_0 = 4, l_0 = -3;
    int i0 = i_0;
    int j0 = j_0;
    int l0 = l_0;
    int max_k = 50;
    int k = 0;
    while (k < max_k && !location(i0, j0)) {
        int temp_i = modulo(abs((i0 + k) * (j0 + 2 * k) * (l0 + 3 * k)), 35);
        int temp_j = sign(max(i0, j0)) * min((modulo((i0 + k), 20)), (modulo((j0 + l0), 20)));
        int temp_l = div(i0, 3) - abs(i0 - k) * sign(l0 - j0);
        i0 = temp_i;
        j0 = temp_j;
        l0 = temp_l;
        k++;
    }
    if (k == max_k) printf("no one hit in %d steps", k);
    else printf("k = %d, i = %d, j = %d, l = %d\n", k, i0, j0, l0);

}