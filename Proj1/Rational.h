#pragma once
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
private:
    void Normalize();
    int numerator_ = 0;
    int denominator_ = 1;
};

