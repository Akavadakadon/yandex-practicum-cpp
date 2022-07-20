#pragma once
#include "Rational.h"

Rational Add(Rational r1, Rational r2) {
    int numerator = r1.Numerator() * r2.Denominator() + r2.Numerator() * r1.Denominator();
    int denominator = r1.Denominator() * r2.Denominator();

    // ������ � ���������� ����� � �������� ���������� � ������������
    return Rational{ numerator, denominator };
}

void ObjectLifeCycle()
{
	Rational r(6,2);
}
