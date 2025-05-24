#include <stdio.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
    double root;
    int iterations;
    bool success;
} Result;

// 1. Функция для первого уравнения: 2*x*sin(x) - cos(x) = 0
double f1(double x) {
    return 2 * x * sin(x) - cos(x);
}

double f1_prime(double x) {
    return 2 * sin(x) + 2 * x * cos(x) + sin(x);
}

// 2. Функция для второго уравнения: exp(x) + sqrt(1 + exp(2*x)) - 2 = 0
double f2(double x) {
    return exp(x) + sqrt(1 + exp(2 * x)) - 2;
}

double f2_prime(double x) {
    return exp(x) + 2 * exp(2 * x) / sqrt(1 + exp(2 * x));
}

// Метод дихотомии
Result dichotomy(double (*f)(double), double a, double b, double eps, int max_steps) {
    Result res = {0, 0, true};
    if (f(a) * f(b) > 0) {
        res.success = false;
        return res;
    }
    while ((b - a) / 2.0 > eps && res.iterations < max_steps) {
        res.iterations++;
        double mid = (a + b) / 2.0;
        if (f(mid) == 0) {
            res.root = mid;
            return res;
        } else if (f(a) * f(mid) < 0) {
            b = mid;
        } else {
            a = mid;
        }
    }
    if (res.iterations >= max_steps) {
        res.success = false;
    }
    res.root = (a + b) / 2.0;
    return res;
}

// Метод итераций
Result iteration_method(double (*g)(double), double x0, double eps, int max_steps) {
    Result res = {0, 0, true};
    double x1 = g(x0);
    res.iterations++;
    while (fabs(x1 - x0) > eps) {
        if (res.iterations >= max_steps) {
            res.success = false;
            return res;
        }
        // Проверка достаточного условия сходимости (|g'(x)| < 1)
        if (fabs(x1 - x0) / fabs(x0) >= 1) {
            res.success = false;
            return res;
        }
        x0 = x1;
        x1 = g(x0);
        res.iterations++;
    }
    res.root = x1;
    return res;
}

// Метод Ньютона
Result newton_method(double (*f)(double), double (*f_prime)(double), double x0, double eps, int max_steps) {
    Result res = {0, 0, true};
    if (fabs(f_prime(x0)) < eps) {  // Проверка достаточного условия сходимости
        res.success = false;
        return res;
    }
    double x1 = x0 - f(x0) / f_prime(x0);
    res.iterations++;
    while (fabs(x1 - x0) > eps) {
        if (fabs(f_prime(x1)) < eps || res.iterations >= max_steps) {
            res.success = false;
            return res;
        }
        x0 = x1;
        x1 = x0 - f(x0) / f_prime(x0);
        res.iterations++;
    }
    res.root = x1;
    return res;
}

double g1(double x) {
    return x - (2 * x * sin(x) - cos(x)) / 3;
}

double g2(double x) {
    return log(2 - sqrt(1 + exp(2 * x)));
}

int main() {
    double a1 = 0, b1 = 1, eps = 1e-5;
    double a2 = -1, b2 = 0, eps2 = 1e-5;
    int max_steps = 1000;

    // Метод дихотомии
    Result dichotomy1 = dichotomy(f1, a1, b1, eps, max_steps);
    if (dichotomy1.success) {
        printf("Метод дихотомии, корень 1: %.6f, шагов: %d\n", dichotomy1.root, dichotomy1.iterations);
    } else {
        printf("Метод дихотомии не сошелся для уравнения 1\n");
    }

    Result dichotomy2 = dichotomy(f2, a2, b2, eps2, max_steps);
    if (dichotomy2.success) {
        printf("Метод дихотомии, корень 2: %.6f, шагов: %d\n", dichotomy2.root, dichotomy2.iterations);
    } else {
        printf("Метод дихотомии не сошелся для уравнения 2\n");
    }

    // Метод итераций
    Result iteration1 = iteration_method(g1, 0.5, eps, max_steps);
    if (iteration1.success) {
        printf("Метод итераций, корень 1: %.6f, шагов: %d\n", iteration1.root, iteration1.iterations);
    } else {
        printf("Метод итераций не сошелся для уравнения 1\n");
    }

    Result iteration2 = iteration_method(g2, -0.3, eps2, max_steps);
    if (iteration2.success) {
        printf("Метод итераций, корень 2: %.6f, шагов: %d\n", iteration2.root, iteration2.iterations);
    } else {
        printf("Метод итераций не сошелся для уравнения 2\n");
    }

    // Метод Ньютона
    Result newton1 = newton_method(f1, f1_prime, 0.5, eps, max_steps);
    if (newton1.success) {
        printf("Метод Ньютона, корень 1: %.6f, шагов: %d\n", newton1.root, newton1.iterations);
    } else {
        printf("Метод Ньютона не сошелся для уравнения 1\n");
    }

    Result newton2 = newton_method(f2, f2_prime, -0.3, eps2, max_steps);
    if (newton2.success) {
        printf("Метод Ньютона, корень 2: %.6f, шагов: %d\n", newton2.root, newton2.iterations);
    } else {
        printf("Метод Ньютона не сошелся для уравнения 2\n");
    }

    return 0;
}
