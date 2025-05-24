#include <stdio.h>

// Проверка на разделитель
int issep(int let) {
    return let == '\n' || let == '\t' || let == ' ' || let == EOF;
}

// Определение состояния на основе символа
int determine_state(int ch) {
    if (ch == '0') return 1; // Нули
    else if (ch == '1') return 2; // Единица
    else if ('2' <= ch && ch <= '6') return 3; // Двойка - Шестерка
    else if (ch == '7') return 4; // Семерка
    return 7; // Пропуск
}

// Преобразование первой цифры в слово
const char* firstDigit_to_Word(char digit) {
    switch (digit) {
        case '1': return "dix";
        case '2': return "vingt";
        case '3': return "trente";
        case '4': return "quarante";
        case '5': return "cinquante";
        case '6': return "soixante";
        default: return "";
    }
}

// Преобразование второй цифры в слово
const char* secondDigit_to_Word(char digit) {
    switch (digit) {
        case '1': return " et un\n";
        case '2': return "-deux\n";
        case '3': return "-trois\n";
        case '4': return "-quatre\n";
        case '5': return "-cinq\n";
        case '6': return "-six\n";
        case '7': return "-sept\n";
        case '8': return "-huit\n";
        case '9': return "-neuf\n";
        case '0': return "\n";
        default: return "";
    }
}

// Преобразование чисел вида "70-77" в слово
const char* lastDigit_to_Word(char digit) {
    switch (digit) {
        case '0': return "soixante-dix\n";
        case '1': return "soixante et onze\n";
        case '2': return "soixante-douze\n";
        case '3': return "soixante-treize\n";
        case '4': return "soixante-quatorze\n";
        case '5': return "soixante-quinze\n";
        case '6': return "soixante-seize\n";
        case '7': return "soixante-dix-sept\n";
        default: return "";
    }
}

int main() {
    int state = 0, minus_flag = 1, digFirst = 0, digSecond = 0, ch;

    do {
        ch = getchar();
        if (state == 0) { // Начальное состояние
            if (ch == '+') {
                state = 6; // После знака
            } else if (ch == '-') {
                state = 6;
                minus_flag = 0; // Минус
            } else if ('0' <= ch && ch <= '7') {
                state = determine_state(ch);
                if (ch != '0') digFirst = ch;
            } else if (issep(ch)) {
                state = 0;
            } else state = 7; // Пропуск
        }

	else if (state == 1) { // Нули
            if ('0' <= ch && ch <= '7') {
                state = determine_state(ch);
                if (ch != '0') digFirst = ch;
	    }
	    else if (issep(ch)) {
	      minus_flag = 1;
	      state = 0;
	    }  
            else state = 7;
        } else if (state == 6) { // После знака
            if ('0' <= ch && ch <= '7') {
                state = determine_state(ch);
                if (ch != '0') digFirst = ch;
            } else if (issep(ch)) {
                minus_flag = 1;
                state = 0;
            } else state = 7;
        } else if (state == 2) { // Единица
            if ('1' <= ch && ch <= '9') {
                state = 8; // Вывод
                digSecond = ch;
            } else state = 7;
        } else if (state == 3) { // Двойка - Шестерка
            if ('0' <= ch && ch <= '9') {
                state = 8;
                digSecond = ch;
            } else state = 7;
        } else if (state == 4) { // Семерка
            if ('0' <= ch && ch <= '7') {
                state = 8;
                digSecond = ch;
            } else state = 7;
        } else if (state == 8) { // Вывод числа
            if (issep(ch)) {
                if (!minus_flag) printf("moins ");
                if (digFirst == '7') {
                    printf("%s", lastDigit_to_Word(digSecond));
                } else {
                    printf("%s%s", firstDigit_to_Word(digFirst), secondDigit_to_Word(digSecond));
                }
                state = 0;
            } else state = 7;
	    minus_flag = 1;
	    digSecond = 0;
	    digFirst = 0;
        } else if (state == 7) { // Пропуск
            minus_flag = 1;
	    digSecond = 0;
	    digFirst = 0;
            if (issep(ch)) state = 0;
        }
    } while (ch != EOF);

    return 0;
}
