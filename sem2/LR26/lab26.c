#include <stdio.h>
#include <stdlib.h>

typedef struct elem elem;

struct elem {
    double value;
    elem* next;
    elem* prev;
};

typedef struct {
    elem* first;
    elem* last;
} dyn_deque;

dyn_deque* create_dyn_deque() {
    dyn_deque* res = malloc(sizeof(dyn_deque));

    if (res == NULL) {
        fprintf(stderr, "Creating dynamic deque is failed\n\n");
        return NULL;
    }

    res -> first = NULL;
    res -> last = NULL;
    return res;
}

void destroy_dyn_deque(dyn_deque* ddeq) {
    elem* cur = ddeq -> first;

    while (cur != NULL) {
        elem* next = cur -> next;
        free(cur);
        cur = next;
    }
    free(ddeq);
}

int is_empty_dyn_deque(dyn_deque* ddeq) {
    return ddeq -> first == NULL;
}

int push_back_dyn_deque(dyn_deque* ddeq, double element) {
    elem* new = malloc(sizeof(elem));

    if (new == NULL) {
        fprintf(stderr, "Adding a new element to the back of the dynamic deque is failed\n\n");
        return 0;
    }

    new -> value = element;
    new -> next = NULL;
    new -> prev = ddeq -> last;

    if (ddeq -> last == NULL) {
        ddeq -> last = ddeq -> first = new;
    } else {
        ddeq -> last -> next = new;
        ddeq -> last = new;
    }
    return 1;
}

int push_front_dyn_deque(dyn_deque* ddeq, double element) {
    elem* new = malloc(sizeof(elem));
    if (new == NULL) {
        fprintf(stderr, "Adding a new element to the front of the dynamic deque is failed\n\n");
        return 0;
    }

    new -> value = element;
    new -> next = ddeq -> first;
    new -> prev = NULL;

    if (ddeq -> first == NULL) {
        ddeq -> first = ddeq -> last = new;
    } else {
        ddeq -> first -> prev = new;
        ddeq -> first = new;
    }
    return 1;
}

double pop_back_dyn_deque(dyn_deque* ddeq) {
    if (is_empty_dyn_deque(ddeq)) {
        fprintf(stderr, "Dynamic deque is empty\n\n");
        exit(-1);
    }

    elem* new_last = ddeq -> last -> prev;
    double element = ddeq -> last -> value;    
    free(ddeq -> last);
    ddeq -> last = new_last;

    if (ddeq -> last == NULL) {
        ddeq -> first = NULL;
    } else {
        ddeq -> last -> next = NULL;
    }
    return element;
}

double pop_front_dyn_deque(dyn_deque* ddeq) {
    if (is_empty_dyn_deque(ddeq)) {
        fprintf(stderr, "Dynamic deque is empty\n\n");
        exit(-1);
    }

    elem* new_first = ddeq -> first -> next;
    double element = ddeq -> first -> value;
    free(ddeq -> first);
    ddeq -> first = new_first;

    if (ddeq -> first == NULL) {
        ddeq -> first = ddeq -> last = NULL;
    } else {
        ddeq -> first -> prev = NULL;
    }
    return element;
}

double findMax(dyn_deque* ddeq) {
    if (is_empty_dyn_deque(ddeq)) {
        fprintf(stderr, "Dynamic deque is empty\n\n");
        exit(-1);
    }
    dyn_deque* helped_ddeq = create_dyn_deque(); // Вспомогательный дек
    double max_cur = pop_front_dyn_deque(ddeq); 
    int max_count = 1; // Счетчик вхождений максимума

    while (!is_empty_dyn_deque(ddeq)) {
        double cur = pop_front_dyn_deque(ddeq);
        if (cur > max_cur) {
            for (int i = 0; i < max_count; i++) {
                push_back_dyn_deque(helped_ddeq, max_cur);
            }
            max_cur = cur;
            max_count = 1;
        } else if (cur == max_cur) {
            max_count++;
        } else {
            push_back_dyn_deque(helped_ddeq, cur);
        }
    }

    for (int i = 1; i < max_count; i++) {
        push_back_dyn_deque(helped_ddeq, max_cur);
    }

    while (!is_empty_dyn_deque(helped_ddeq)) {
        double val = pop_front_dyn_deque(helped_ddeq);
        push_back_dyn_deque(ddeq, val);
    }

    destroy_dyn_deque(helped_ddeq); 
    return max_cur; 
}

void choose_sort(dyn_deque* ddeq) {
    dyn_deque* sorted = create_dyn_deque();
    while (!is_empty_dyn_deque(ddeq)) {
        // double max_value = findMax(ddeq);
        push_front_dyn_deque(sorted, findMax(ddeq));
    }
    while (!is_empty_dyn_deque(sorted)) {
        push_back_dyn_deque(ddeq, pop_front_dyn_deque(sorted));  
    }
    destroy_dyn_deque(sorted);
}


int main() {
    dyn_deque* dq = create_dyn_deque();
    double d;
    while (scanf("%lf", &d) == 1){
        push_back_dyn_deque(dq, d);
    }

    choose_sort(dq);

    while (!is_empty_dyn_deque(dq)) {
        printf("%.1f ", pop_front_dyn_deque(dq));
    }

    destroy_dyn_deque(dq);
    printf("\n");
    return 0;
}