#include "task3.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>


using namespace std;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

void Comporator2()
{
    int count = 4;// ReadLineWithNumber();
    string line = "quantity AMOUNT Selection Search Aaa AAAA aAA";// ReadLine();

    vector<string> words;
    string word;
    for (const char c : line) {
        if (c == ' ') {
            words.push_back(word);
            word = "";
        }
        else {
            word += c;
        }
    }
    words.push_back(word);

    sort(words.begin(), words.end(), [](string x, string y) {
        int i = 0;
        while (i < x.length() || i < y.length())
        {
            if (tolower(x[i]) == tolower(y[i]))
                i++;
            else
                return tolower(x[i]) < tolower(y[i]);
        }
        return x.length() < y.length();
        });

    for_each(words.begin(), words.end(), [](string x) { cout << x << endl; });
}

void ByValue()
{
    int count = 3;// ReadLineWithNumber();
    vector<string> lines;
    //for (int i = 0; i < 3; i++)
    //    lines.push_back(ReadLine());
    lines.push_back("apple juice");
    lines.push_back("bionic beaver beaver");
    lines.push_back("funny little beaver");
    string word = "beaver";// ReadLine();

    int counter = 0, pos = 0;
    for (int i =0;i<lines.size();i++)
    {
        size_t found = lines[i].find(word, pos);
        if (found != string::npos)
        {
            counter++;
            pos = found + 1;
            i--;
        }
        else
            pos = 0;
    }
    cout << endl << "found occurrences: " << counter << endl;

}

void DogShelter()
{
    map<string, int> shelter{
    {"landseer"s, 1},
    {"otterhound"s, 2},
    {"pekingese"s, 2},
    {"pointer"s, 3}
    };
    const map<string, int> max_amount{
        {"landseer"s, 2},
        {"otterhound"s, 3},
        {"pekingese"s, 4},
        {"pointer"s, 7}
    };
    const vector<string> new_dogs{
        "landseer"s,
        "otterhound"s,
        "otterhound"s,
        "otterhound"s,
        "pointer"s
    };
    int counter = 0;
    //for (pair<string, int> dog : max_amount)
    counter += count_if(new_dogs.begin(), new_dogs.end(), [max_amount, &shelter](const string x) {
        int a = shelter.at(x) ;
        int b = max_amount.at(x);
        bool res = a + 1 <= b;
        if (res)
        {
            shelter.at(x)++;
            return true;
        }
        return false;
        });
    cout << endl << "admitted dogs: " << counter << endl;
}

void Task3()
{
	cout << "\n\t\tThird chapter\n" << endl;
    Comporator2();
    ByValue();
    DogShelter();
}
