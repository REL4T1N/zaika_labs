/*
Вариант 59: Разложить на множители квадрат разности -- (a - b) ^ 2 = (a ^ 2 ) - 2 * (a * b) + (b ^ 2)
transformTree(): Разложение квадрата разности
printExpression(): Вывод выражения в инфиксной форме
printTree(): Вывод дерева
buildTree(): Построение дерева из постфиксной формы
infixToPostfix(): Перевод из инфиксной формы в постфиксную
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Structure for tree nodes
typedef struct TreeNode {
    char* value;          // оператор или операнд
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Stack for characters (used in shunting-yard)
typedef struct CharStack {
    char* data;
    int top;
    int capacity;
} CharStack;

CharStack* createCharStack(int capacity) {
    CharStack* stack = (CharStack*)malloc(sizeof(CharStack));
    stack->data = (char*)malloc(capacity * sizeof(char));
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

void pushChar(CharStack* stack, char c) {
    if (stack->top < stack->capacity - 1) {
        stack->data[++stack->top] = c;
    }
}

char popChar(CharStack* stack) {
    if (stack->top >= 0) {
        return stack->data[stack->top--];
    }
    return '\0';
}

char peekChar(CharStack* stack) {
    if (stack->top >= 0) {
        return stack->data[stack->top];
    }
    return '\0';
}

void freeCharStack(CharStack* stack) {
    free(stack->data);
    free(stack);
}

// Стек для узлов дерева (используется при построении дерева)
typedef struct NodeStack {
    TreeNode** data;
    int top;
    int capacity;
} NodeStack;

NodeStack* createNodeStack(int capacity) {
    NodeStack* stack = (NodeStack*)malloc(sizeof(NodeStack));
    stack->data = (TreeNode**)malloc(capacity * sizeof(TreeNode*));
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

void pushNode(NodeStack* stack, TreeNode* node) {
    if (stack->top < stack->capacity - 1) {
        stack->data[++stack->top] = node;
    }
}

TreeNode* popNode(NodeStack* stack) {
    if (stack->top >= 0) {
        return stack->data[stack->top--];
    }
    return NULL;
}

void freeNodeStack(NodeStack* stack) {
    free(stack->data);
    free(stack);
}

// Создать новый узел дерева
TreeNode* createNode(const char* value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->value = strdup(value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void freeTree(TreeNode* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root->value);
        free(root);
    }
}

// Найти приоритет оператора
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*') return 2;
    if (op == '^') return 3;
    return 0;
}

int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '^');
}

// Алгоритм Дейкстры перевода из инфиксной в постфиксную форму
void infixToPostfix(char* infix, char* postfix) {
    CharStack* stack = createCharStack(strlen(infix));
    int j = 0;
    for (int i = 0; infix[i]; i++) {
        if (isspace(infix[i])) continue;
        if (isalnum(infix[i])) {
            while (isalnum(infix[i])) {
                postfix[j++] = infix[i++];
            }
            i--;
            postfix[j++] = ' ';
        } else if (infix[i] == '(') {
            pushChar(stack, '(');
        } else if (infix[i] == ')') {
            while (peekChar(stack) != '(') {
                postfix[j++] = popChar(stack);
                postfix[j++] = ' ';
            }
            popChar(stack); // Удалить '('
        } else if (isOperator(infix[i])) {
            while (peekChar(stack) && precedence(peekChar(stack)) >= precedence(infix[i])) {
                postfix[j++] = popChar(stack);
                postfix[j++] = ' ';
            }
            pushChar(stack, infix[i]);
        }
    }
    while (peekChar(stack)) {
        postfix[j++] = popChar(stack);
        postfix[j++] = ' ';
    }
    postfix[j - 1] = '\0'; // Убрать пробел в конце
    freeCharStack(stack);
}

// Построение дерева из постфиксной формы
TreeNode* buildTree(char* postfix) {
    NodeStack* stack = createNodeStack(strlen(postfix));
    char token[10];
    int ti = 0;

    for (int i = 0; postfix[i]; i++) {
        if (isspace(postfix[i])) {
            if (ti > 0) {
                token[ti] = '\0';
                TreeNode* node = createNode(token);
                pushNode(stack, node);
                ti = 0;
            }
        } else if (isOperator(postfix[i])) {
            token[0] = postfix[i];
            token[1] = '\0';
            TreeNode* node = createNode(token);
            node->right = popNode(stack);
            node->left = popNode(stack);
            pushNode(stack, node);
        } else {
            token[ti++] = postfix[i];
        }
    }
    if (ti > 0) {
        token[ti] = '\0';
        pushNode(stack, createNode(token));
    }
    TreeNode* root = popNode(stack);
    freeNodeStack(stack);
    return root;
}

// Вывод дерева
void printTree(TreeNode* root, int depth) {
    if (!root) return;
    printTree(root->right, depth + 1);
    for (int i = 0; i < depth; i++) printf("  ");
    printf("%s\n", root->value);
    printTree(root->left, depth + 1);
}

// Вывод выражения в инфиксной записи
void printExpression(TreeNode* root) {
    if (!root) return;
    int needParens = root->left && isOperator(root->value[0]);
    if (needParens) printf("(");
    printExpression(root->left);
    printf("%s", root->value);
    printExpression(root->right);
    if (needParens) printf(")");
}

// Разложение квадрата разности: (a-b)^2 → a^2-2ab+b^2
TreeNode* transformTree(TreeNode* root) {
    if (!root) return NULL;
    root->left = transformTree(root->left);
    root->right = transformTree(root->right);

    if (strcmp(root->value, "^") == 0 && root->right && strcmp(root->right->value, "2") == 0 &&
        root->left && strcmp(root->left->value, "-") == 0) {
        TreeNode* a = root->left->left;
        TreeNode* b = root->left->right;

        TreeNode* a2 = createNode("^"); 
        a2->left = createNode(a->value); 
        a2->right = createNode("2");

        TreeNode* b2 = createNode("^"); 
        b2->left = createNode(b->value); 
        b2->right = createNode("2");

        TreeNode* ab = createNode("*"); 
        ab->left = createNode(a->value); 
        ab->right = createNode(b->value);

        TreeNode* two_ab = createNode("*"); 
        two_ab->left = createNode("2"); 
        two_ab->right = ab;

        TreeNode* minus1 = createNode("-"); 
        minus1->left = a2; 
        minus1->right = two_ab;

        TreeNode* plus = createNode("+"); 
        plus->left = minus1; 
        plus->right = b2;

        freeTree(root->left); 
        free(root->right->value); 
        free(root->right); 
        free(root->value); 
        free(root);
        return plus;
    }
    return root;
}


void doExercise() {
    char input[256];
    printf("Введите арифметическое выражение:\n");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // удалить \n

    // 1. Преобразуем в постфиксную запись
    char postfix[512];
    infixToPostfix(input, postfix);

    // 2. Строим дерево выражения
    TreeNode* tree = buildTree(postfix);

    // 3. Печатаем исходное выражение и дерево
    printf("\nИсходное выражение: %s\n", input);
    printf("Дерево выражения:\n");
    printTree(tree, 0);

    printf("Инфиксная форма: ");
    printExpression(tree);
    printf("\n");

    // 4. Выполняем преобразование
    tree = transformTree(tree);

    // 5. Печатаем преобразованное выражение и дерево
    printf("\nПреобразованное дерево:\n");
    printTree(tree, 0);

    printf("Преобразованная инфиксная форма: ");
    printExpression(tree);
    printf("\n");

    freeTree(tree);
}

int main() {
    doExercise();
    return 0;
}