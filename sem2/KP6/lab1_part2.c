#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MAX_CARS 200
#define LENGTH_TOP 3
#define LENGTH_NAME 41
#define MAX_FUEL_TYPES 10
#define MAX_LINE_LENGTH 256
#define EPSILON 1e-6

typedef struct {
    char stamp[20];
    char model[20];
    int r_year;
    int hp;
    float engine_volume;
    float overclock;
    char fuel_type[20];
    char country[20];
} Cars;

// Функция для объединения двух строк
void twostrings(char mass1[20], char mass2[20], char res[LENGTH_NAME]) {
    int pos = 0;
    for (int i = 0; mass1[i] != '\0'; i++) {
        res[pos++] = mass1[i];
    }
    res[pos++] = ' ';
    for (int i = 0; mass2[i] != '\0'; i++) {
        res[pos++] = mass2[i];
    }
    res[pos] = '\0';
}

void printAllCars(Cars cars[MAX_CARS], int count, FILE *fout) {
    fprintf(fout, "Полный список машин:\n");
    for (int i = 0; i < count; i++) {
        fprintf(fout, "%s %s, %d, %d ЛС, %.1fл, %.1fs, %s, %s\n",
                cars[i].stamp, cars[i].model, cars[i].r_year, cars[i].hp,
                cars[i].engine_volume, cars[i].overclock, cars[i].fuel_type,
                cars[i].country);
    }
    fprintf(fout, "\n");
}

void more_power(Cars cars[MAX_CARS], int count, FILE *fout) {
    char name_car[LENGTH_TOP][LENGTH_NAME] = {"", "", ""};
    int hpcar[LENGTH_TOP] = {0, 0, 0};
    char result[LENGTH_NAME];

    for (int i = 0; i < count; i++) {
        if (cars[i].hp > hpcar[0]) {
            twostrings(cars[i].stamp, cars[i].model, result);
            hpcar[2] = hpcar[1];
            hpcar[1] = hpcar[0];
            hpcar[0] = cars[i].hp;
            strcpy(name_car[2], name_car[1]);
            strcpy(name_car[1], name_car[0]);
            strcpy(name_car[0], result);
        }
        else if (cars[i].hp <= hpcar[0]) {
            if (cars[i].hp > hpcar[1]) {
                twostrings(cars[i].stamp, cars[i].model, result);
                hpcar[2] = hpcar[1];
                hpcar[1] = cars[i].hp;
                strcpy(name_car[2], name_car[1]);
                strcpy(name_car[1], result);
            }
            else if ((cars[i].hp == hpcar[1] && cars[i].hp > hpcar[2]) ||
                     (cars[i].hp < hpcar[1] && cars[i].hp > hpcar[2])) {
                twostrings(cars[i].stamp, cars[i].model, result);
                hpcar[2] = cars[i].hp;
                strcpy(name_car[2], result);
            }
        }
    }
    fprintf(fout, "Топ 3 машины по мощности:\n");
    for (int i = 0; i < LENGTH_TOP; i++) {
        fprintf(fout, "%d. %s, %d ЛС\n", i + 1, name_car[i], hpcar[i]);
    }
    fprintf(fout, "\n");
}

void typesFuel(Cars cars[MAX_CARS], int count, FILE *fout) {
    char all_fuels_type[MAX_FUEL_TYPES][20];
    int count_fuel_car[MAX_FUEL_TYPES] = {0};
    int fuel_type_count = 0;

    for (int i = 0; i < count; i++) {
        int found = 0; // Флаг, найден ли тип топлива в массиве
        for (int j = 0; j < fuel_type_count; j++) {
            if (strcasecmp(all_fuels_type[j], cars[i].fuel_type) == 0) {
                count_fuel_car[j]++;
                found = 1;
                break;
            }
        }
        if (!found && fuel_type_count < MAX_FUEL_TYPES) {
            strcpy(all_fuels_type[fuel_type_count], cars[i].fuel_type);
            count_fuel_car[fuel_type_count] = 1;
            fuel_type_count++;
        }
    }

    fprintf(fout, "Типы топлива и количество автомобилей:\n");
    for (int i = 0; i < fuel_type_count; i++) {
        fprintf(fout, "%s: %d\n", all_fuels_type[i], count_fuel_car[i]);
    }
    fprintf(fout, "\n");

    int min_count = count + 1;
    char rarest_fuel[20] = "";
    for (int i = 0; i < fuel_type_count; i++) {
        if (count_fuel_car[i] < min_count) {
            min_count = count_fuel_car[i];
            strcpy(rarest_fuel, all_fuels_type[i]);
        }
    }

    fprintf(fout, "Машины с самым редким типом топлива (%s, используется в %d машинах):\n", 
            rarest_fuel, min_count);
    for (int i = 0; i < count; i++) {
        if (strcasecmp(cars[i].fuel_type, rarest_fuel) == 0) {
            fprintf(fout, "%s %s, %d, %d ЛС, %.1fл, %.1fs, %s, %s\n",
                   cars[i].stamp, cars[i].model, cars[i].r_year, cars[i].hp,
                   cars[i].engine_volume, cars[i].overclock, cars[i].fuel_type,
                   cars[i].country);
        }
    }
    fprintf(fout, "\n");
}

void filter_by_engine_volume(Cars cars[MAX_CARS], int count, float target, FILE *fout) {
    bool found = false;
    fprintf(fout, "Машины с объемом двигателя %.1f:\n", target);
    for (int i = 0; i < count; i++) {
        if (fabs(cars[i].engine_volume - target) < EPSILON) {
            fprintf(fout, "%s %s, %d, %d ЛС, %.1fл, %.1fs, %s, %s\n",
                    cars[i].stamp, cars[i].model, cars[i].r_year, cars[i].hp,
                    cars[i].engine_volume, cars[i].overclock, cars[i].fuel_type,
                    cars[i].country);
            found = true;
        }
    }
    if (!found) {
        fprintf(fout, "Нет машин с указанным объемом двигателя %.1f\n", target);
    }
    fprintf(fout, "\n");
}

int is_duplicate(Cars cars[], int count, Cars new_car) {
    for (int i = 0; i < count; i++) {
        if ((strcmp(cars[i].stamp, new_car.stamp) == 0 &&
             strcmp(cars[i].model, new_car.model) == 0 &&
             cars[i].r_year == new_car.r_year &&
             cars[i].hp == new_car.hp &&
             fabs(cars[i].engine_volume - new_car.engine_volume) < EPSILON &&
             fabs(cars[i].overclock - new_car.overclock) < EPSILON &&
             strcmp(cars[i].fuel_type, new_car.fuel_type) == 0 &&
             strcmp(cars[i].country, new_car.country) == 0)) {
            return 1;
        }
    }
    return 0;
}

// Справка по использованию
void print_usage(const char* progName) {
    fprintf(stderr, "Использование: %s <input_file> <output_file> [опции]\n", progName);
    fprintf(stderr, "Опции:\n");
    fprintf(stderr, "  -m           Вывести топ 3 машины по мощности\n");
    fprintf(stderr, "  -t           Вывести типы топлива и список машин с самым редким типом топлива\n");
    fprintf(stderr, "  -f           Вывести полный список машин\n");
    fprintf(stderr, "  -p <число>   Отобрать и вывести машины с указанным объемом двигателя\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    char *input_filename = argv[1];
    char *output_filename = argv[2];

    // Флаги для различных действий
    bool do_more_power = false;
    bool do_types_fuel = false;
    bool do_full_list = false;
    bool do_volume_filter = false;
    float target_volume = 0.0f;

    if (argc == 3) {
        do_more_power = do_types_fuel = do_full_list = true;
    } else {
        for (int i = 3; i < argc; i++) {
            if (strcmp(argv[i], "-m") == 0) {
                do_more_power = true;
            } else if (strcmp(argv[i], "-t") == 0) {
                do_types_fuel = true;
            } else if (strcmp(argv[i], "-f") == 0) {
                do_full_list = true;
            } else if (strcmp(argv[i], "-p") == 0) {
                if (i + 1 < argc) {
                    do_volume_filter = true;
                    target_volume = atof(argv[i + 1]);
                    i++;
                } else {
                    fprintf(stderr, "Параметр -p требует значения.\n");
                    print_usage(argv[0]);
                    return 1;
                }
            } else {
                fprintf(stderr, "Неизвестный параметр: %s\n", argv[i]);
                print_usage(argv[0]);
                return 1;
            }
        }
    }

    FILE *fin = fopen(input_filename, "r");
    if (!fin) {
        fprintf(stderr, "Ошибка: не удалось открыть входной файл %s\n", input_filename);
        return 2;
    }

    FILE *fout = fopen(output_filename, "w");
    if (!fout) {
        fprintf(stderr, "Ошибка: не удалось открыть выходной файл %s\n", output_filename);
        fclose(fin);
        return 3;
    }

    Cars cars[MAX_CARS];
    int count = 0;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), fin)) {
        Cars new_car;
        if (sscanf(line, "%19[^:]:%19[^:]:%d:%d:%f:%f:%19[^:]:%19[^\n]",
                   new_car.stamp, new_car.model, &new_car.r_year,
                   &new_car.hp, &new_car.engine_volume, &new_car.overclock,
                   new_car.fuel_type, new_car.country) == 8) {
            if (!is_duplicate(cars, count, new_car)) {
                cars[count++] = new_car;
            }
        }
    }

    if (do_full_list) {
        printAllCars(cars, count, fout);
    }

    if (do_more_power) {
        more_power(cars, count, fout);
    }

    if (do_types_fuel) {
        typesFuel(cars, count, fout);
    }

    if (do_volume_filter) {
        filter_by_engine_volume(cars, count, target_volume, fout);
    }

    fclose(fin);
    fclose(fout);

    return 0;
}
