#include <iostream>
#include <cmath>

class complex {

public:

    complex() : real(0), imag(0) {}

    complex(double r, double i) : real(0), imag(0) {
        real = r;
        imag = i;
    }

    complex(complex &old) {
        real = old.real;
        imag = old.imag;
    }

    ~complex() {
        real = 0;
        imag = 0;
    }

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

    double module() const {
        double len = sqrt(real * real + imag * imag);
        return len;
    }

    double argument() const {
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
    complex a(2, 3);
    complex b(5, 1);
    std::cout << a.module();
}