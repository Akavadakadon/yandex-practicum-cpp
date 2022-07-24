#pragma once

#include <iostream>
#include "Rational.h"
using namespace std;


void Overloading()
{
	
	cout << "\n\nOverloadingTesting\n" << endl;
	Rational asd, asd1, asd2, asd3;
	//cin >> asd;
	asd1 = asd2 = asd3 = asd;
	asd1 = asd1 + asd1;
	asd2++;
	asd3--;
	cout << asd << endl;
	cout << asd1 << endl;
	cout << asd2 << endl;
	cout << asd3 << endl;
	cout << ++asd2 << endl;
	cout << --asd3 << endl;
	cout << (asd3 == asd3) << endl;
	cout << (asd3 != asd3) << endl;
	cout << (asd3 <= asd3) << endl;
	cout << (asd3 >= asd3) << endl;
	cout << (asd3 > asd3) << endl;
	cout << (asd3 < asd3) << endl;


};

