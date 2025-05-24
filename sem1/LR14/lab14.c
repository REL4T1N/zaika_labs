#include <stdio.h>

void Spiral(int n, int matrix[n][n]) {
    int top = 0, bottom = n - 1, left = 0, right = n - 1;
    int direction = 0;
    int s = 0;

    while (s < n * n) {
        if (direction == 0) { // Слева направо
            for (int i = left; i <= right; i++) {
	        printf("%d ", matrix[bottom][i]);
                s++;
            }
            bottom--;
	}

	else if (direction == 1) { // Снизу вверх
            for (int i = bottom; i >= top; i--) {
                printf("%d ", matrix[i][right]);
                s++;
            }
            right--;
        }

	else if (direction == 2) { // Справа налево
            for (int i = right; i >= left; i--) {
                printf("%d ", matrix[top][i]);
                s++;
            }
            top++;
        }

	else if (direction == 3) { // Сверху вниз
            for (int i = top; i <= bottom; i++) {
                printf("%d ", matrix[i][left]);
                s++;
            }
            left++;
        }
        direction = (direction + 1) % 4; // Меняем направление
    }
    printf("\n");
}

int main() {
    int n;
    scanf("%d", &n);
    int matrix[n][n];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

    Spiral(n, matrix);
    return 0;
}
