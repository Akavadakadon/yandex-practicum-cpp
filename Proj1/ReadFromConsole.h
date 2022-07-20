#pragma once
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class ReadFromConsole
{
public: 
	static string ReadLine() {
	    string s;
	    getline(cin, s);
	    return s;
	}
	
	static int ReadLineWithNumber() {
	    int result;
	    cin >> result;
	    ReadLine();
	    return result;
	}

	static streambuf* AddToCin(string str, istream& stream)
	{
		streambuf* orig = stream.rdbuf();
		istringstream* input = new istringstream(str);
		stream.rdbuf(input->rdbuf());
		return orig;
	}
};

