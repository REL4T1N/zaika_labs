// таска
// написать 2 программы
// найти интересную область и собрать по ней табличку
// пример футбола:
// команда1:команда2:сколько_забила1:сколько_забила2:колвоМатчей:странаПервойКоманды:странаВторойКоманды
// СОБРАТЬ СВОЮ ДЛИННУЮ ТАБЛИЦУ(минимум 100 строк)
// ПРОГРАММА 1: получим текстовый файл->перевести в бинарный файл
// ПРОГРАММА 2: придумать какой-то абстрактный сбор статистики, чтобы он считался не за 1 проход по файлу -> несколько вариантов -> принимает путь файла в качестве аргумента командной строку -> 2 программа получает также в качестве аргумента формат работы с файлом


// typedef struct {
//     float diag;
//     int resx;
//     int resy;
//     int freg;
//     char matrix[10];
//     bool hdr;
//     char model[42];
//     char proizvod[52];
// } Monitor;
// перегнать потом в "monitor.h"


#include "stdio.h"
#include "stdbool.h"

typedef struct {
    char stamp[15];
    char model[15];
    int r_year;
    int hp;
    float engine_volume;
    float overclock;
    char fuel_type[20];
    char country[10];
} Cars;


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Ошибка: укажите входной и выходной файлы\n");
        return 1;
    }

    FILE* fin = fopen(argv[1], "r");
    FILE* fout = fopen(argv[2], "wb");
    
    if (!fin) {
        fprintf(stderr, "Ошибка: не удалось открыть входной файл %s\n", argv[1]);
        return 2;
    }
    if (!fout) {
        fprintf(stderr, "Ошибка: не удалось открыть выходной файл %s\n", argv[2]);
        fclose(fin);
        return 3;
    }

    Cars car;
    int count = 0;
    
    while (fscanf(fin, "%14[^:]:%14[^:]:%d:%d:%f:%f:%19[^:]:%9s\n",
                  car.stamp, 
                  car.model, 
                  &car.r_year, 
                  &car.hp, 
                  &car.engine_volume, 
                  &car.overclock, 
                  car.fuel_type, 
                  car.country) == 8) {
        if (fwrite(&car, sizeof(Cars), 1, fout) != 1) {
            fprintf(stderr, "Ошибка записи в файл\n");
            break;
        }
        count++;
    }

    if (ferror(fin)) {
        fprintf(stderr, "Ошибка чтения входного файла\n");
    }

    printf("Успешно обработано %d записей\n", count);

    fclose(fin);
    fclose(fout);
    return 0;
}

