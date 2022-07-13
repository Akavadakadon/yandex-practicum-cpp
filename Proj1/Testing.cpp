#include "Testing.h"
#include <iostream>
#include <cassert>
#include "ReadFromConsole.h"
#include "Synonyms.h"

using namespace std;

string IsPalindrome(string str)
{
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	if (str.length() == 0)
		return "not a palindrome";
	//remove_if(str.begin(), str.end(), isspace);
	int len = str.length() -1 ;
	char tmp,* str2 = new char[str.length() + 1];
	strcpy_s(str2, sizeof(char)* str.length() + 1, str.c_str());
	for (int i = 0; i < (str.length()) / 2; i++)
	{
		tmp = str2[len - i];
		str2[len - i] = str2[i];
		str2[i] = tmp;
	}
	string str1223(str2);
	return str== str1223?"palindrome":"not a palindrome";
}
bool Testing_IsPalindrome()
{
	int res = 0;
	assert(IsPalindrome("radar") == "palindrome");
	assert(IsPalindrome("never odd or even") == "palindrome");
	assert(IsPalindrome("apple") == "not a palindrome");
	assert(IsPalindrome("kick") == "not a palindrome");

	assert(IsPalindrome("a") == "palindrome");
	assert(IsPalindrome("            ") == "not a palindrome");

	cout << "Test IsPalindrome() is OK"s << endl;
	return res == 0;
}

void Testing()
{
	//string asd = ReadFromConsole::ReadLine();
	Testing_IsPalindrome();
	SynonymsTask();
	cout << "\n\nTesting\n\n";
}


