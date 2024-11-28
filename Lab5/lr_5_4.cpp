#include <iostream>
#include <cmath>

class complex {

public:

    complex() : real(0), imag(0) {}

    complex(const double r, const double i) : real(0), imag(0) {
        real = r;
        imag = i;
    }

    complex(const complex &old) {
        real = old.real;
        imag = old.imag;
    }

    ~complex() = default;

    complex operator+ (complex& a) const {
        double r, i;
        r = real + a.real;
        i = imag + a.imag;
        return complex{r, i};
    }

    complex operator- (complex& a) const {
        double r, i;
        r = real - a.real;
        i = imag - a.imag;
        return complex{r, i};
    }

    complex operator* (complex& a) const {
        double r, i;
        r = real * a.real - imag * a.imag;
        i = imag * a.real + a.imag * real;
        return complex{r, i};
    }

    complex operator/ (complex& a) const {
        double r, i, divider;
        divider = a.real * a.real + a.imag * a.imag;
        r = (real * a.real + imag * a.imag) / divider;
        i = (imag * a.real - a.imag * real) / divider;
        return complex{r, i};
    }

    complex comp_sum(complex& a) {
        double r, i;
        r = real + a.real;
        i = imag + a.imag;
        return complex{r, i};
    }

    complex comp_dif (complex& a) const {
        double r, i;
        r = real - a.real;
        i = imag - a.imag;
        return complex{r, i};
    }

    complex comp_mult (complex& a) const {
        double r, i;
        r = real * a.real - imag * a.imag;
        i = imag * a.real + a.imag * real;
        return complex{r, i};
    }

    complex comp_div (complex& a) const {
        double r, i, divider;
        divider = a.real * a.real + a.imag * a.imag;
        r = (real * a.real + imag * a.imag) / divider;
        i = (imag * a.real - a.imag * real) / divider;
        return complex{r, i};
    }

    double module() const {
        double len = sqrt(real * real + imag * imag);
        return len;
    }

    double argument() const {
        if (real == 0) {
            if (imag == 0) {
                std::cout << "The argument for the null vector is not defined.\n";
                return -1;
            }
            return ((imag < 0) ? -1 : 1) * asin(1);
        }
        double arg = atan(imag / real);
        return arg;
    }


    friend std::ostream& operator << (std::ostream &out, const complex& a);

private:
    double real;
    double imag;
};

std::ostream& operator << (std::ostream &out, const complex& a) {
    out << a.real << " + " << a.imag << "i";
    return out;
}

int main() {
    complex a(1.2, 8.4);
    complex b(5.131, 123.5);

    std::cout << a.comp_div(b) << '\n';
    std::cout << a.comp_dif(b) << '\n';
    std::cout << a.comp_sum(b) << '\n';
    std::cout << a.comp_mult(b) << '\n';
}