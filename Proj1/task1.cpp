#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <set>

#include "task1_relevantDocs.h"

using namespace std;

int PrintWorryCount(const vector<bool>& is_nervous) {
    int worry_count = 0;
    for (bool status : is_nervous) {
        if (status) {
            ++worry_count;
        }
    }
    //cout << worry_count << endl;
    return worry_count;
}

int PrintWorryCount_countIf(const vector<bool>& is_nervous) {
    int worry_count = 0;
    worry_count = count_if(is_nervous.begin(), is_nervous.end(), [](bool x) {return x; });
    //cout << worry_count << endl;
    return worry_count;
}

int count()
{
     vector<bool> testVector = { true, false,false,false };
    int res1 = PrintWorryCount(testVector);
    int res2 = PrintWorryCount_countIf(testVector);
    cout << "PrintWorryCount" << " :\t" << res1 << " to " << res2 << endl;
	return 0;
}

int CountStartsWithA(const vector<string>& xs) {
    return count_if(xs.begin(), xs.end(), [](string x) {return (x[0] == 'a' || x[0] == 'A'); });
}

int count2()
{
    cout << CountStartsWithA({ "And"s, "another"s, "one"s, "gone"s, "another"s, "one"s "bites"s, "the"s, "dust"s }) << endl;
    cout << CountStartsWithA({ }) << endl;
    return 0;
}

auto SertPeopleByAge(vector<pair<int, string>> vec)
{
    sort(vec.begin(), vec.end(), [](pair<int, string>x, pair<int, string> y) {return x.first > y.first; });
    for (auto pp : vec)
        cout << pp.second << endl;
}

auto SertPeopleByAge(vector<pair<int, string>>* vec)
{
    sort(vec->begin(), vec->end(), [](pair<int, string>x, pair<int, string> y) {return x.first > y.first; });
}

int sort()
{
    vector<pair<int, string>> vec = { {13,"A"}, {15,"Aaa"}, {15,"Aaa"}, {14,"Aa"} };
    SertPeopleByAge(&vec);
    for (auto pp : vec)
        cout << pp.second << endl;

    return 0;
}

double CalcMedian(vector<int> vec)
{
    if (vec.size() == 0)
        return -1;
    sort(vec.begin(), vec.end());
    int sum = vec.size() / 2;
    if (vec.size() % 2 == 0)
        return (double)(vec[sum] + vec[sum - 1]) / 2;
    else
        return vec[sum];
}

int sort2()
{
    cout << CalcMedian({ 1, 2, 3, 4 }) << endl;
    cout << CalcMedian({ 1, 2, 3 }) << endl;
    cout << CalcMedian({ 1, 2, 1000 }) << endl;
    cout << CalcMedian({ 1 }) << endl;
    return 0;
}

double Average(const vector<double>& xs) {
    double res = 0.0;
    if (xs.size())
        res = accumulate(xs.begin(), xs.end(), res)/ xs.size();
    return res;
}

int numeric_accumulate()
{
    vector<double> values = { 1,2,3,4,5 };

    cout << Average(values) << endl;
    return 0;
}

string AddStopWords(const string& query, const set<string>& stop_words) {
    string res ("");
    res = query + accumulate(stop_words.begin(), stop_words.end(), (string)"", [](string x, string y) {return x + " -" + y; });
    return res;
}

int numeric_accumulate2()
{
    string query("some tasty oranges");
    set<string> stop_words({ "of", "in"});
    cout << AddStopWords(query, stop_words) << endl;
    return 0;
}


int task1() {
    count();
    count2();

    sort();
    sort2();

    numeric_accumulate();
    numeric_accumulate2();

    FindRelevantDocs();

    return 0;
}
