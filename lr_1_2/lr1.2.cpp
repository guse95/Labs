#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

enum ret_type_t {
    SUCCSESS,
    ERROR_NO_VALUE,
    ERROR_ZERO_VAL,
    ERROR_NEGATIVE_VALUE,
    ERROR_NOT_NUMBER,
    ERROR_TOO_LONG_STR,
    ERROR_EPS_OUT_OF_RANGE,
    ERROR_WRONG_NUMB_OF_ARGS
};

typedef double(*callback)(double);

ret_type_t is_number(const char* s) {
    int flag = 0;
    if (*s == '\0') return ERROR_NO_VALUE;
    while (*s == ' ') s++;
    int len = 0;
    while (isdigit(*s)) {
        if (*s != '0') flag = 1;
        len++;
        s++;
    }
    if (*s == '.') s++;
    while (isdigit(*s)) {
        if (*s != '0') flag = 1;
        s++;
        len++;
    }
    if (!flag) {
        return ERROR_ZERO_VAL;
    }
    if (len > 10) {
        return ERROR_TOO_LONG_STR;
    }
    if (*s == '\0') return SUCCSESS;
    return ERROR_NOT_NUMBER;
}

void HandlingError(int code) {
    switch (code) {
        case ERROR_NEGATIVE_VALUE:
            printf("Negative value was entered.\n");
            break;
        case ERROR_NO_VALUE:
            printf("No number was entered.\n");
            break;
        case ERROR_NOT_NUMBER:
            printf("Value isn`t a number in the right number system.\n");
            break;
        case ERROR_TOO_LONG_STR:
            printf("Too long string was entered.\n");
            break;
        case ERROR_EPS_OUT_OF_RANGE:
            printf("Epsilon out of range.\n");
            break;
        case ERROR_ZERO_VAL:
            printf("Epsilon is zero.\n");
            break;
        default:
            break;
    }
}

double faq(double n) {
    double res = 0;
    if (n == 0) {
        return 1.0;
    } else {
        res = n * faq(n - 1);
        return res;
    }
}

double comb(double up, double down) {
    double res = faq(down) / (faq(up) * faq(down - up));
    //printf("%lf\n", res);
    return res;
}

int is_prime(const int n) {
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
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

double PiLimFunc(const double n) {
    double res = pow((pow(2, n) * faq(n)),4) / (n * pow(faq(2 * n), 2));
//    printf("%d   %lf\n", n, res);
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

double lnEqFunc(const double n) {
    double res = exp(n);
    return res;
}

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

double GamLimFunc (const double n) {
    double res = 0.0;
    for (int k = 1; k <= n; k++) {
        double up = (k % 2 == 0) ? 1 : -1;
        res += (comb(k, n) * (up / k) * log(faq(k)));
    }
    //printf("%.20lf %lf\n", res, m);
    return res;
}

double GamRowFunc(const double n) {
    double res = 0;
    res = 1.0 / pow(floor(sqrt(n)), 2) - (1 / n);
    return res;
}

double GamEqFunc(const double n) {
    double res;
    res = exp(-n);
    return res;
}

double Mult(const double eps, callback MultFunc, const double n) {
    double x = n;
    double PrevAns = MultFunc(x);
    x += 1;
    double CurAns = PrevAns * MultFunc(x);
    while (fabs(PrevAns - CurAns) > eps) {
        PrevAns = CurAns;
        x += 1;
        CurAns *= MultFunc(x);
        //printf("%fl        %fl\n", PrevAns, CurAns);
    }
    return CurAns;
}

double Lim(const double eps, callback LimFunc) {
    double n = 1;
    double PrevAns;
    double CurAns;
    do {
        PrevAns = LimFunc(n);
        n += 1;
        CurAns = LimFunc(n);
    } while (fabs(PrevAns - CurAns) > eps);
    return CurAns;
}

double Row(const double eps, callback RowFunc, const double n) {
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
    return summ;

//    int m = n;
//    double summ = RowFunc(m);
//    double cur_delta;
//
//    do {
//        m += 1;
//        cur_delta = RowFunc(m);
//        summ = summ + cur_delta;
//
//    } while (fabs(cur_delta) > eps);
//    return summ;
}

double Equation(const double eps, callback FuncEq, const double EqRes,
                    double LBorder, double RBorder) {
    double ans = 0;
    double CurAns = 0;
    if (FuncEq(RBorder) - FuncEq(LBorder) > 0) {
        do {
            ans = (LBorder + RBorder) / 2;
            CurAns = FuncEq(ans);
            if ((CurAns - EqRes) < eps) {
                LBorder = ans;
            } else {
                RBorder = ans;
            }
        } while (fabs(CurAns - EqRes) > eps);
    } else {
        do {
            ans = (LBorder + RBorder) / 2;
            CurAns = FuncEq(ans);
            if ((CurAns - EqRes) > eps) {
                LBorder = ans;
            } else {
                RBorder = ans;
            }
        } while (fabs(CurAns - EqRes) > eps);
    }
    return ans;
}

double ResFuncForGam(const double n) {
    double res = 1;
    for (double p = 2; p <= n; p = p + 1) {
        if (is_prime((int)p)) {
            res = res * (1 - 1.0 / p);
        }
    }
    res = res * log(n);
//    printf("%lf\n", res);
    return res;
}

double GamEqRes(const double eps) {
    double res = Lim(eps, ResFuncForGam);
    return res;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
        printf("Wrong number of arguments.");
        return ERROR_WRONG_NUMB_OF_ARGS;
    }

    if (ret_type_t code = is_number(argv[1]))
    {
        HandlingError(code);
        return code;
    }
    double eps = atof(argv[1]);

    if (eps > 0.2 || eps < 0.00001) {
        HandlingError(ERROR_EPS_OUT_OF_RANGE);
        return ERROR_EPS_OUT_OF_RANGE;
    }
    double res_lim = Lim(eps, ELimFunc);
    double res_row = Row(eps, ERowFunc, 0);
    double res_eq = Equation(eps, EEqFunc, 1, 1, 100);
    printf("|%10lf|%10lf|%10lf|\n", res_lim, res_row, res_eq);

    res_lim = Lim(eps * 100, PiLimFunc);
    res_row = Row(eps, PiRowFunc, 1);
    res_eq = Equation(eps, PiEqFunc, 0, 2, 4);
    printf("|%10lf|%10lf|%10lf|\n", res_lim, res_row, res_eq);

    res_lim = Lim(eps, lnLimFunc);
    res_row = Row(eps, lnRowFunc, 1);
    res_eq = Equation(eps, lnEqFunc, 2, 0, 100);
    printf("|%10lf|%10lf|%10lf|\n", res_lim, res_row, res_eq);

    res_lim = Lim(eps, SqrtLimFunc);
    res_row = Mult(eps, SqrtMultFunc, 2);
    res_eq = Equation(eps, SqrtEqFunc, 2, 1, 100);
    printf("|%10lf|%10lf|%10lf|\n", res_lim, res_row, res_eq);

    res_lim = Lim(eps * 100, GamLimFunc);
    res_row = acos(-1) * acos(-1) / (-6.0) + Row(eps, GamRowFunc, 2);
    res_eq = Equation(eps, GamEqFunc, GamEqRes(eps), 0, 100);
    printf("|%10lf|%10lf|%10lf|\n", res_lim, res_row, res_eq);
    return SUCCSESS;
}