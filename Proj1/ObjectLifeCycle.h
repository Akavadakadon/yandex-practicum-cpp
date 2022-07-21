#pragma once
#include "Rational.h"
#include "SearchServer.h"

Rational Add(Rational r1, Rational r2) {
    int numerator = r1.Numerator() * r2.Denominator() + r2.Numerator() * r1.Denominator();
    int denominator = r1.Denominator() * r2.Denominator();

    // ������ � ���������� ����� � �������� ���������� � ������������
    return Rational{ numerator, denominator };
}

void ObjectLifeCycle()
{
    cout << "\n\n ObjectLifeCycle \n\n";
    cout << "\n\n Rational \n\n";
	Rational r(6,2);
    cout << "\n\n Initialization SearchServer \n\n";
    set<string> stop_words_set = { "�"s, "�"s, "��"s };
    auto asdsa = { "�"s, "�"s, "��"s };
    vector<string> stop_words_vector = { "�"s, "�"s, "��"s, ""s, "�"s };
    sprint3::SearchServer search_server2(stop_words_vector);
    sprint3::SearchServer search_server3("  �  � ��   "s);
    sprint3::SearchServer ss;

}
