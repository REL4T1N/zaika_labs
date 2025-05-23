/*
createEmpty(): Создает пустое дерево (возвращает NULL).
isEmpty(): Проверяет, пустое ли дерево (true, если указатель NULL).
build(): Создает новый узел с заданным значением и указателями на левое и правое поддеревья.
getValue(), getLeft(), getRight(): Возвращают значение узла, левое или правое поддерево соответственно.
destroy(): Освобождает память, занятую одним узлом.
destroyRecursive(): Рекурсивно освобождает память всего дерева.

add(): Добавляет новое значение в дерево.
removeNode(): Удаляет значение из дерева.

Задание: Определить число вершин двоичного дерева, имеющих ровно 2 поддерева.
1)countNodesWithTwoSubtrees(): рекурсивный подсчёт узлов с 2-мя поддеревьями.
2)printTree(): Рекурсивная визуализация дерева.
3)countNodesWithTwoSubtreesWithoutRecursion(): подсчёт узлов с 2-мя поддеревеьями без рекурсии (пока нет). 
*/



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct treeNode {
    double val;
    struct treeNode *right;  
    struct treeNode *left;
} treeNode;

typedef treeNode* tree;

// Создание пустого дерева
tree createEmpty() {
    return NULL;
}

// Проверка, пустое ли дерево
bool isEmpty(tree t) {
    return t == NULL;
}

// Получение значения узла
double getValue(tree t) {
    return t->val;
}

// Получение левого поддерева
tree getLeft(tree t) {
    return t->left;
}

// Получение правого поддерева
tree getRight(tree t) {
    return t->right;
}

// Освобождение памяти узла
void destroy(tree t) {
    free(t);
}

// Создание нового узла
tree build(double val, tree left, tree right) {
    tree res = malloc(sizeof(*res));
    if (res == NULL) {
        return NULL; // Обработка ошибки выделения памяти
    }
    res->left = left;
    res->val = val;
    res->right = right;
    return res;
}

// Вспомогательная функция для проверки существования значения
bool contains(tree t, double val) {
    if (isEmpty(t)) {
        return false;
    }
    if (getValue(t) == val) {
        return true;
    } else if (val < getValue(t)) {
        return contains(getLeft(t), val);
    } else {
        return contains(getRight(t), val);
    }
}

// Добавление нового значения в дерево
tree add(tree t, double val) {
    if (contains(t, val)) {
        printf("Ошибка: значение %.2f уже существует в дереве.\n", val);
        return t;
    }
    if (isEmpty(t)) {
        return build(val, createEmpty(), createEmpty());
    }
    if (val < getValue(t)) {
        tree left = add(getLeft(t), val);
        tree res = build(getValue(t), left, getRight(t));
        destroy(t);
        return res;
    } else if (val > getValue(t)) {
        tree right = add(getRight(t), val);
        tree res = build(getValue(t), getLeft(t), right);
        destroy(t);
        return res;
    }
}

// Удаление значения из дерева
tree removeNode(tree t, double val) {
    if (isEmpty(t)) {
        printf("Ошибка: дерево пустое.\n");
        return t;
    }
    if (!contains(t, val)) {
        printf("Ошибка: значение %.2f не существует в дереве.\n", val);
        return t;
    }
    if (val < getValue(t)) {
        tree left = removeNode(getLeft(t), val);
        tree res = build(getValue(t), left, getRight(t));
        destroy(t);
        return res;
    } else if (val > getValue(t)) {
        tree right = removeNode(getRight(t), val);
        tree res = build(getValue(t), getLeft(t), right);
        destroy(t);
        return res;
    }
    // Нашли узел для удаления
    if (isEmpty(getLeft(t))) {
        tree res = getRight(t);
        destroy(t);
        return res;
    } else if (isEmpty(getRight(t))) {
        tree res = getLeft(t);
        destroy(t);
        return res;
    }
    // У узла есть оба потомка
    tree maxLeft = getLeft(t);
    while (!isEmpty(getRight(maxLeft))) {
        maxLeft = getRight(maxLeft);
    }
    double maxLeftVal = getValue(maxLeft);
    
    tree left = removeNode(getLeft(t), maxLeftVal);
    tree res = build(maxLeftVal, left, getRight(t));
    destroy(t);
    return res;
}

// Рекурсивное освобождение памяти дерева
void destroyRecursive(tree t) {
    if (!isEmpty(t)) {
        destroyRecursive(getLeft(t));
        destroyRecursive(getRight(t));
        destroy(t);
    }
}

// Рекурсивный подсчёт узлов с 2-мя поддеревьями.
int countNodesWithTwoSubtrees(tree t) {
    if (isEmpty(t)) {
        return 0;
    }
    int count = 0;
    if (!isEmpty(getLeft(t)) && !isEmpty(getRight(t))) {
        count = 1;
    }
    return count + countNodesWithTwoSubtrees(getLeft(t)) + countNodesWithTwoSubtrees(getRight(t));
}

// Рекурсивная визуализация дерева.
void printTree(tree t, int depth) {
    if (isEmpty(t)) {
        return;
    }
    printTree(getRight(t), depth + 1);
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("%.2f\n", getValue(t));
    printTree(getLeft(t), depth + 1);
}

// Текстовое меню
void textMenu(tree t) {
    double val;
    int choice;
    while (1) {
        printf("\nМеню:\n");
        printf("1. Добавить новый узел\n");
        printf("2. Визуализировать дерево\n");
        printf("3. Удалить узел\n");
        printf("4. Подсчитать число узлов с двумя поддеревьями\n");
        printf("5. Выйти\n");
        printf("Введите ваш выбор: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: неверный ввод.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: // Добавить новый узел
                printf("Введите значение для добавления: ");
                if (scanf("%lf", &val) == 1) {
                    tree newTree = add(t, val);
                    if (newTree != t) {
                        t = newTree;
                        printf("Узел добавлен.\n");
                    }
                } else {
                    printf("Ошибка: неверный ввод.\n");
                    while (getchar() != '\n');
                }
                break;

            case 2: // Визуализировать дерево
                if (isEmpty(t)) {
                    printf("Дерево пустое.\n");
                } else {
                    printf("Дерево:\n");
                    printTree(t, 0);
                }
                break;

            case 3: // Удалить узел
                printf("Введите значение для удаления: ");
                if (scanf("%lf", &val) == 1) {
                    tree newTree = removeNode(t, val);
                    if (newTree != t) {
                        t = newTree;
                        printf("Узел удален.\n");
                    }
                } else {
                    printf("Ошибка: неверный ввод.\n");
                    while (getchar() != '\n');
                }
                break;

            case 4: // Подсчитать число узлов с двумя поддеревьями
                {
                    int count = countNodesWithTwoSubtrees(t);
                    printf("Число узлов с двумя поддеревьями: %d\n", count);
                }
                break;

            case 5: // Выйти
                destroyRecursive(t);
                printf("Программа завершена.\n");
                return;

            default:
                printf("Ошибка: неверный выбор.\n");
        }
    }
}


int main() {
    tree t = createEmpty();
    textMenu(t);
    return 0;
}