#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

enum ret_type_t { SUCCSESS, ERROR };

typedef double(*callback)(int);

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
    long long res = 1;
    if (n > 34) {
        printf("mi ymerli\n");
    }
    for (long long i = 2; i <= n; i = i + 1) {
        res *= i;
    }
    return res;
}

double comb(double up, double down) {
    double res = 1;
    if (up < (down - up)) {
        up = down - up;
    }
    for (double i = up + 1; i < down + 1; i = i + 1) {
        res *= i;
    }
    //printf("%lf  ", res);
    for (double i = 2; i < (down - up) + 1; i = i + 1) {
        res /= i;
    }
    //printf("%lf  %lf  %lf\n", down, up, res);
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
    double res = 1.0 / faq((int)n);
//    printf("%lf\n", res);
    return res;
}

double EEqFunc(const double n) {
    double res = log(n);
    return res;
}


double PiLimFunc(const int n) {
    double res = pow((pow(2, n) * (double)faq(n)),4) / (double)(n * pow(faq(2 * n), 2));
    printf("%d   %lf\n", n, res);
    return res;
}

double PiRowFunc(const double n) {
    //double res =  4 * pow(-1, n - 1) / (2 * n - 1);
    double up = 1;
    int power = (int)n;
    if (power % 2 == 0) {
        up = -1;
    }
    double res = up / (2 * n - 1);
//    printf("%lf\n", res);
    return 4 * res;
}

double PiEqFunc(const double n) {
    double res =  sin(n);
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
    double x_n;
    x_n = pow(2, pow(2, -n));
    return x_n;
}

double SqrtEqFunc(const double n) {
    double res = n * n;
    return res;
}


double GamLimFunc(const double n) {
    double res = 0;
    double OneIter;
    for (double k = 1; k <= n; k = k + 1) {
        OneIter = (comb(k, n) / (double)k * log(faq((double)k)));
//        printf("%lf  %lf   %lf\n", n, k, OneIter);
        if ((long long)k % 2 == 0) {
            res = res + OneIter;
        } else {
            res = res - OneIter;
        }
    }
//    printf("%lf\n", res);
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
        //printf("%fl        %fl\n", PrevAns, CurAns);
    }
    printf("%lf\n", CurAns);
    return SUCCSESS;
}

ret_type_t Lim(const double eps, callback LimFunc) {
    double n = 1;
    double PrevAns;
    double CurAns;
    do {
        PrevAns = LimFunc(n);
        n += 1;
        CurAns = LimFunc(n);
    } while (fabs(PrevAns - CurAns) > eps);
    printf("%lf\n", CurAns);
    return SUCCSESS;
}

ret_type_t Row(const double eps, callback RowFunc, double n) {
    // double PrevAns = RowFunc(n);
    // n += 1;
    // double CurAns = PrevAns + RowFunc(n);
    // while (fabs(PrevAns - CurAns) > eps) {
    //     PrevAns = CurAns;
    //     n += 1;
    //     CurAns += RowFunc(n);
    // }
    // printf("%fl\n", CurAns);

    double x = n;
    double prev_delta = RowFunc(x);
    double summ = prev_delta;
    x += 1;
    double cur_delta = RowFunc(x);

    do {
        summ += cur_delta;
        prev_delta = cur_delta;
        x += 1;
        cur_delta = RowFunc(x);

    } while (fabs(fabs(cur_delta) - fabs(prev_delta)) > eps);
    printf("%lf\n", summ);
    return SUCCSESS;
}

ret_type_t Equation(const double eps, callback FuncEq, const double EqRes,
                    double LBorder, double RBorder) {
    double m = 0;
    double CurAns = 0;
    if (FuncEq(RBorder) - FuncEq(LBorder) > 0) {
        do {
            m = (LBorder + RBorder) / 2;
            CurAns = FuncEq(m);
            if ((CurAns - EqRes) < eps) {
                LBorder = m;
            } else {
                RBorder = m;
            }
        } while (fabs(CurAns - EqRes) > eps);
    } else {
        do {
            m = (LBorder + RBorder) / 2;
            CurAns = FuncEq(m);
            if ((CurAns - EqRes) > eps) {
                LBorder = m;
            } else {
                RBorder = m;
            }
        } while (fabs(CurAns - EqRes) > eps);
    }

    printf("%lf\n", m);
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
    printf("%lf\n", eps);


//    Lim(eps, ELimFunc);
//    Row(eps, ERowFunc, 0);
//    Equation(eps, EEqFunc, 1, 1, 100);

    Lim(eps, PiLimFunc);
//    Row(eps, PiRowFunc, 1);
//    Equation(eps, PiEqFunc, 0, 2, 4);
//
//    Lim(eps, lnLimFunc);
//    Row(eps, lnRowFunc, 1);
//    Equation(eps, lnEqFunc, 2, 0, 100);
//
//    Lim(eps, SqrtLimFunc);
//    Mult(eps, SqrtMultFunc, 2);
//    Equation(eps, SqrtEqFunc, 2, 1, 100);
//
//    Lim(eps, GamLimFunc);

    return 0;
}