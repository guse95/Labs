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

double faq(const double n) {
    if (!n) {
        return 1;
    }
    double res = 1;
    for (int i = 2; i - n <= 0; ++i) {
        res *= i;
    }
    return res;
}

double ELimFunc(const double n) {
    double res = pow(1 + 1 / n, n);
    return res;
}

double ERowFunc(const double n) {
    double res = 1 / faq(n);
    return res;
}

double EEqFunc(const double n) {
    double res = log(n);
    return res;
}


double PiLimFunc(const double n) {
    double res = ((pow(2, 4 * n) /pow(faq(2 * n) , 2)) * pow(faq(n), 4)) / n;
    printf("%fl\n", res);
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


//double lnLimFunc(const double n) {
//    double res = n * (pow(2, 1/n) - 1);
//    return res;
//}
//
//double lnRowFunc(const double n) {
//    double res = ;
//    return res;
//}
//
//double lnEqFunc(const double n) {
//    double res = n * (pow(2, 1/n) - 1);
//    return res;
//}

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
    double LBorder = 2;
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
//        printf("%fl\n", m);
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
}

int main(int argc, char* argv[]) {
	if (argc != 2) return ERROR;

    if (is_number(argv[1])) 
    {
        printf("Input Error");
        return ERROR;
    }
    double eps = strtod(argv[1], NULL);
    printf("%f\n", eps);

//    callback e[] = { &eByLim, &eByRow, &eByEquation};
//    callback pi[] = { &piByLim, &piByRow, &piByEquation};


    Lim(eps, PiLimFunc);
    Row(eps, PiRowFunc, 1);
    Equation(eps, PiEqFunc, -1);

    return 0;
}