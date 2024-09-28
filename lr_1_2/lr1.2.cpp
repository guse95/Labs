#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

enum ret_type_t { SUCCSESS, ERROR };

typedef double(*callback)(double);

int is_number(const char* s) {
    if (*s == '\0') return ERROR;

    while (*s == ' ') s++;

    if (*s == '-') s++;

    while (isdigit(*s)) s++;

    if (*s == '.') s++;

    while (isdigit(*s)) s++;

    if (*s == '\0') return SUCCSESS;
    return ERROR;
}

long long faq(const long long n) {
    if (!n) {
        return 1;
    }
    long long res = 1;
    for (int i = 2; i - n <= 0; ++i) {
        res *= i;
    }
    return res;
}

double comb(double m, double n) {
    double res = 1;
    for (double i = m + 1; i <= n; i += 1) {
        res *= i;
    }
    for (double i = 2; i <= (n - m); i += 1) {
        res /= i;
    }
    return res;
}


double Pow(const double x, const double n) {
    double res = x;
    if (n >= 1) {
        for(int _ = 0; _ <= n; ++_) {
            res *= x;
        }
    } else
        res = pow(x, n);
    return res;
}

double ELimFunc(const double n) {
    double res = pow(1 + 1 / n, n);
    return res;
}

double ERowFunc(const double n) {
    double res = 1.0 / faq(n);
    return res;
}

double EEqFunc(const double n) {
    double res = log(n);
    return res;
}


double PiLimFunc(const double n) {
    double res = ((pow(2, 4 * n) /pow(faq(2 * n) , 2)) * pow(faq(n), 4)) / n;
    return res;
}

double PiRowFunc(const double n) {
    double res =  4 * pow(-1, n - 1) / (2 * n - 1);
    return res;
}

double PiEqFunc(const double n) {
    double res =  cos(n) + 1;
    return res;
}


double lnLimFunc(const double n) {
    double res = n * (pow(2, 1/n) - 1);
    return res;
}

double lnRowFunc(const double n) {
    double res = pow((-1), n - 1) / n;
    return res;
}

//double lnEqFunc(const double n) {
//    double res = ;
//    return res;
//}


double SqrtLimFunc(const double n) {
    static double x_n;
    if (n == 1) x_n = -0.5;
    x_n = x_n - x_n * x_n / 2 + 1;
    return x_n;
}

double SqrtMultFunc(const double n) {
    static double x_n;
    if (n == 2) x_n = (2 * 2) * (2 * 2);
    x_n = x_n * 4;
    return x_n;
}

double SqrtEqFunc(const double n) {
    double res = n * n;
    return res;
}


double GamLimFunc(const double n) {
    double res;
    for (int k = 1; k <= n; ++k) {
        res += (comb(k, n) * ((k % 2 == 0) ? 1: -1) / k) * log(faq(k));
    }
    return res;
}

ret_type_t Mult(const double eps, callback MultFunc, double n) {
    double PrevAns = MultFunc(n);
    n += 1;
    double CurAns = PrevAns * MultFunc(n);
    while (fabs(PrevAns - CurAns) > eps) {
        PrevAns = CurAns;
        n += 1;
        CurAns *= MultFunc(n);
    }
    printf("%fl\n", CurAns);
    return SUCCSESS;
}

ret_type_t Lim(const double eps, callback LimFunc) {
    double n = 1;
    double PrevAns = LimFunc(n);
    n += 1;
    double CurAns = LimFunc(n);
    while (fabs(PrevAns - CurAns) > eps) {
        PrevAns = CurAns;
        n += 1;
        CurAns = LimFunc(n);
    }
    printf("%fl\n", CurAns);
    return SUCCSESS;
}

ret_type_t Row(const double eps, callback RowFunc, double n) {
    double PrevAns = RowFunc(n);
    n += 1;
    double CurAns = PrevAns + RowFunc(n);
    while (fabs(PrevAns - CurAns) > eps) {
        PrevAns = CurAns;
        n += 1;
        CurAns += RowFunc(n);
    }
    printf("%fl\n", CurAns);
    return SUCCSESS;
}

ret_type_t Equation(const double eps, callback FuncEquation, const double EqRes) {
    double LBorder = 1;
    double RBorder = 100;
    double m = 0;
    double CurAns = 0;
    do {
        m = (LBorder + RBorder) / 2;
        CurAns = FuncEquation(m);
        if ((CurAns - EqRes) < eps) {
            LBorder = m;
        } else {
            RBorder = m;
        }
    } while (fabs(CurAns - EqRes) > eps);
    printf("%fl\n", m);
    return SUCCSESS;
}

ret_type_t EquationForPi(const double eps, callback FuncEquation) {
    double PrevAns = 0;
    double CurAns = (1e4 + 1)/ 2;
    while (fabs(PrevAns - CurAns) > eps) {
        PrevAns = CurAns;
        CurAns = FuncEquation(CurAns);
    }
    printf("%fl\n", CurAns);
    return SUCCSESS;
}

int main(int argc, char* argv[]) {
	if (argc != 2) return ERROR;

    if (is_number(argv[1])) 
    {
        printf("Input Error");
        return ERROR;
    }
    double eps = strtod(argv[1], NULL);
    printf("%fl\n", eps);


    Lim(eps, ELimFunc);
    Row(eps, ERowFunc, 0);
    Equation(eps, EEqFunc, 1);

    return 0;
}