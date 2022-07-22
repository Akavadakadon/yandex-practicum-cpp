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
    if (nod == 0)
        return;
    numerator_ = numerator / nod;
    denominator_ = denominator / nod;
}

Rational::Rational(int numerator) : numerator_(numerator), denominator_(1)
{

}

istream& operator>> (istream& in, Rational& rat) {
    int numerator, denominator;
    if (in >> numerator >> denominator)
        rat = Rational(numerator, denominator);
    return in;
}
ostream& operator<<(ostream& output, const Rational& rat) {
    cout << "numerator " << rat.Numerator() << " denominator " << rat.Denominator();
    return output;
}

Rational Rational::operator+ ( Rational right)
{
    int numerator, denominator;
    numerator = this->Numerator() * right.Denominator() + right.Numerator() * this->Denominator();
    denominator = right.Denominator() * this->Denominator();
    Rational newR = Rational(numerator, denominator);
    return newR;
}
Rational Rational::operator-( Rational right)
{
    int numerator, denominator;
    numerator = this->Numerator() * right.Denominator() - right.Numerator() * this->Denominator();
    denominator = right.Denominator() * this->Denominator();
    Rational newR = Rational(numerator, denominator);
    return newR;
}
Rational Rational::operator/(Rational right)
{
    int numerator, denominator;
    numerator = this->Numerator() * right.Denominator();
    denominator = right.Numerator() * this->Denominator();
    Rational newR = Rational(numerator, denominator);
    return newR;
}
Rational Rational::operator*(Rational right)
{
    int numerator, denominator;
    numerator = this->Numerator() * right.Numerator();
    denominator = right.Denominator() * this->Denominator();
    Rational newR = Rational(numerator, denominator);
    return newR;
}

Rational& Rational::operator++() {
    numerator_+= denominator_;
    return *this;
}
Rational Rational::operator++(int) {
    Rational temp = *this;
    ++* this;
    return temp;
}   
Rational& Rational::operator--() {
    numerator_-=denominator_;
    return *this;
}
Rational Rational::operator--(int) {
    Rational temp = *this;
    --* this;
    return temp;
}
Rational& Rational::operator+= (Rational right)
{
    Rational asd = *this + right;
    return *this;
}
Rational& Rational::operator-= (Rational right)
{
    Rational asd = *this - right;
    return *this;
}
Rational& Rational::operator*= (Rational right)
{
    Rational asd = *this * right;
    return *this;
}
Rational& Rational::operator/= (Rational right)
{
    Rational asd = *this / right;
    return *this;
}
bool Rational::operator== (Rational right) {
    int righti, lefti;
    righti = this->Numerator() * right.Denominator();
    lefti = this->Denominator() * right.Numerator();
    return righti == lefti;
}
bool Rational::operator!=(Rational right) {
    int righti, lefti;
    righti = this->Numerator() * right.Denominator();
    lefti = this->Denominator() * right.Numerator();
    return righti != lefti;
}
bool Rational::operator> (Rational right) {
    int righti, lefti;
    righti = this->Numerator() * right.Denominator();
    lefti = this->Denominator() * right.Numerator();
    return righti > lefti;
}
bool Rational::operator<(Rational right) {
    int righti, lefti;
    righti = this->Numerator() * right.Denominator();
    lefti = this->Denominator() * right.Numerator();
    return righti < lefti;
}
bool Rational::operator>= (Rational right) {
    int righti, lefti;
    righti = this->Numerator() * right.Denominator();
    lefti = this->Denominator() * right.Numerator();
    return righti >= lefti;
}
bool Rational::operator<=(Rational right) {
    int righti, lefti;
    righti = this->Numerator() * right.Denominator();
    lefti = this->Denominator() * right.Numerator();
    return righti <= lefti;
}