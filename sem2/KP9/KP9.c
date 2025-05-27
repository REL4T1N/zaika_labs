#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MIN_ELEMENTS 22
#define NUM_ELEMENTS_TO_USE 22
#define MAX_TEXT_LEN 200
#define MAX_POEM_LINES 64
#define EPSILON 1e-6
#define STACK_SIZE 64

typedef struct {
    double a, b, c, d;
} Quaternion;

typedef struct {
    Quaternion key;
    char text[MAX_TEXT_LEN];
} TableRecord;

// Глобальное хранилище строк стихотворения
char poem_lines[MAX_POEM_LINES][MAX_TEXT_LEN];
int lines_count = 0;

// Чтение стихотворения
void load_poem(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        strcpy(poem_lines[0], "Нет файла: используем заготовку 1.");
        strcpy(poem_lines[1], "Нет файла: используем заготовку 2.");
        strcpy(poem_lines[2], "Нет файла: используем заготовку 3.");
        lines_count = 3;
        return;
    }
    lines_count = 0;
    while (lines_count < MAX_POEM_LINES &&
           fgets(poem_lines[lines_count], MAX_TEXT_LEN, file)) {
        poem_lines[lines_count][strcspn(poem_lines[lines_count], "\r\n")] = 0;
        if (strlen(poem_lines[lines_count]) > 0) {
            lines_count++;
        }
    }
    fclose(file);

    if (lines_count == 0) {
        strcpy(poem_lines[0], "Файл пуст — подставляем строку 1");
        strcpy(poem_lines[1], "Файл пуст — подставляем строку 2");
        strcpy(poem_lines[2], "Файл пуст — подставляем строку 3");
        lines_count = 3;
    }
}

// Сравнение double с эпсилоном
int double_equal(double a, double b) {
    return fabs(a - b) < EPSILON;
}

// СЛексикографическое сравнение кватернионов
int cmp_quaternion(const Quaternion* q1, const Quaternion* q2) {
    if (!double_equal(q1->a, q2->a)) return (q1->a < q2->a) ? -1 : 1;
    if (!double_equal(q1->b, q2->b)) return (q1->b < q2->b) ? -1 : 1;
    if (!double_equal(q1->c, q2->c)) return (q1->c < q2->c) ? -1 : 1;
    if (!double_equal(q1->d, q2->d)) return (q1->d < q2->d) ? -1 : 1;
    return 0;
}

// Вывод кватерниона
void print_quaternion(const Quaternion* q) {
    printf("(%.6lf, %.6lf, %.6lf, %.6lf)", q->a, q->b, q->c, q->d);
}

// Печать таблицы
void print_table(const char* title, TableRecord* table, int n) {
    printf("\n--- %s ---\n", title);
    for (int i = 0; i < n; ++i) {
        printf("%2d: ", i + 1);
        print_quaternion(&table[i].key);
        printf("  ->  %s\n", table[i].text);
    }
}

// Быстрая нерекурсивная сортировка Хоара
void quicksort_nonrec(TableRecord* arr, int n) {
    int stack[STACK_SIZE], top = -1;
    int l, r, i, j;
    TableRecord temp;

    stack[++top] = 0;
    stack[++top] = n - 1;

    while (top >= 0) {
        r = stack[top--];
        l = stack[top--];

        while (l < r) {
            i = l;
            j = r;
            TableRecord pivot = arr[(l + r) / 2];

            do {
                while (cmp_quaternion(&arr[i].key, &pivot.key) < 0) i++;
                while (cmp_quaternion(&arr[j].key, &pivot.key) > 0) j--;
                if (i <= j) {
                    temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                    i++;
                    j--;
                }
            } while (i <= j);

            if (j - l < r - i) {
                if (i < r) { stack[++top] = i; stack[++top] = r; }
                r = j;
            } else {
                if (l < j) { stack[++top] = l; stack[++top] = j; }
                l = i;
            }
        }
    }
}

// Бинарный поиск по кватерниону
int binary_search(TableRecord* arr, int n, Quaternion* key) {
    int left = 0, right = n-1, result = -1;
    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = cmp_quaternion(&arr[mid].key, key);
        if (cmp == 0) {
            result = mid;
            right = mid - 1; // проверка дальше
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return result;
}

// Заполнение таблицы
void populate_table(TableRecord* table, int n, int mode) {
    if (lines_count == 0) {
        fprintf(stderr, "Стихотворение не загружено. Заполнение невозможно.\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        switch (mode) {
            case 0: // Упорядоченная
                table[i].key.a = i;
                table[i].key.b = i + 0.1;
                table[i].key.c = i + 0.2;
                table[i].key.d = i + 0.3;
                break;
            case 1: // Обратная
                table[i].key.a = n - i;
                table[i].key.b = n - i + 0.1;
                table[i].key.c = n - i + 0.2;
                table[i].key.d = n - i + 0.3;
                break;
            case 2: // Случайная
                table[i].key.a = (rand() % 2001 - 1000) / 10.0;
                table[i].key.b = (rand() % 2001 - 1000) / 10.0;
                table[i].key.c = (rand() % 2001 - 1000) / 10.0;
                table[i].key.d = (rand() % 2001 - 1000) / 10.0;
                break;
        }
        strcpy(table[i].text, poem_lines[i % lines_count]);
    }
}

// Демонстрация поиска по ключу
void perform_searches(TableRecord* table, int n) {
    printf("\n== Примеры ключей для поиска (скопируйте для теста) ==\n");
    for (int i = 0; i < n; i++) {
        print_quaternion(&table[i].key);
        printf("\n");
    }

    Quaternion search_key;
    char input_buffer[256];
    printf("Вводите кватернион (a b c d) или q для выхода\n");
    while (printf("> "), fgets(input_buffer, sizeof(input_buffer), stdin)) {
        if (input_buffer[0] == 'q' || input_buffer[0] == 'Q') break;
        if (sscanf(input_buffer, "%lf %lf %lf %lf", &search_key.a, &search_key.b, &search_key.c, &search_key.d) == 4) {
            int idx = binary_search(table, n, &search_key);
            if (idx >= 0) {
                printf("  Найден: ");
                print_quaternion(&table[idx].key);
                printf(" | %s\n", table[idx].text);
            } else {
                printf(" Не найден.\n");
            }
        } else {
            printf("Ошибка: введите 4 числа через пробел или q для выхода\n");
        }
    }
}

int main() {
    srand(time(NULL));
    load_poem("text.txt");
    if (lines_count == 0) {
        fprintf(stderr, "Ошибка: нет строк для таблицы.\n");
        return EXIT_FAILURE;
    }
    TableRecord table[NUM_ELEMENTS_TO_USE];
    int n = NUM_ELEMENTS_TO_USE;

    printf("=== Быстрая сортировка Хоара (нерекурсивная) и поиск по кватерниону ===\n");
    printf("Структура: ключ — кватернион, данные — строка\n");
    printf("Количество записей: %d\n\n", n);

    printf("\n----- Сценарий 1: исходные данные по возрастанию -----\n");
    populate_table(table, n, 0);
    print_table("Исходная таблица (упорядочена)", table, n);
    quicksort_nonrec(table, n);
    print_table("Таблица после сортировки", table, n);
    perform_searches(table, n);

    printf("\n----- Сценарий 2: обратный порядок -----\n");
    populate_table(table, n, 1);
    print_table("Исходная таблица (обратный порядок)", table, n);
    quicksort_nonrec(table, n);
    print_table("Таблица после сортировки", table, n);
    perform_searches(table, n);

    printf("\n----- Сценарий 3: случайные кватернионы -----\n");
    populate_table(table, n, 2);
    print_table("Исходная таблица (не упорядочена)", table, n);
    quicksort_nonrec(table, n);
    print_table("Таблица после сортировки", table, n);
    perform_searches(table, n);

    printf("\nРабота программы завершена.\n");
    return EXIT_SUCCESS;
}
