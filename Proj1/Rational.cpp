#include "Rational.h"

int Rational::Numerator() const {
    return numerator_;
}

int Rational::Denominator() const {
    return denominator_;
}

void Rational::SetNumerator(int numerator) {
    numerator_ = numerator;
}

void Rational::SetDenominator(int denominator) {
    if (denominator != 0) {
        denominator_ = denominator;
    }
}

int NOD(int a, int b)
{
    for (int i = a; i > 0; i--) {
        if (a % i == 0 && b % i == 0) {
            return i;
            break;
        }
    }
}

void Rational::Normalize() {
    if (denominator_ < 0) {
        numerator_ = -numerator_;
        denominator_ = -denominator_;
    }
    const int divisor = NOD(numerator_, denominator_);
    numerator_ /= divisor;
    denominator_ /= divisor;
}

Rational::Rational(int numerator, int denominator)
{
    int nod = NOD(numerator, denominator);
    numerator_ = numerator / nod;
    denominator_ = denominator / nod;
}

Rational::Rational(int numerator) : numerator_(numerator), denominator_(1)
{

}
