#pragma once
#include <iostream>
using namespace std;

class Rational {
public:
    int Numerator() const;

    int Denominator() const;

    void SetNumerator(int numerator);

    void SetDenominator(int denominator);

    Rational() = default;

    Rational(int numerator, int denominator);

    Rational(int numerator);

    Rational& operator++();
    Rational operator++(int);
    Rational& operator--();
    Rational operator--(int);

    Rational& operator+= (Rational right);
    Rational& operator-= (Rational right);
    Rational& operator/= (Rational right);
    Rational& operator*= (Rational right);

    Rational operator+ ( Rational right);
    Rational operator-( Rational right);
    Rational operator* (Rational right);
    Rational operator/(Rational right);

    bool operator== (Rational right);
    bool operator!=(Rational right);
    bool operator> (Rational right);
    bool operator<(Rational right);
    bool operator>= (Rational right);
    bool operator<=(Rational right);

private:
    void Normalize();
    int numerator_ = 0;
    int denominator_ = 1;
};

istream& operator>> (istream& in, Rational& rat);
ostream& operator<<(ostream& output, const Rational& rat);
