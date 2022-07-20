#include "FrameworkForTesting.h"
#include "Synonyms.h"
#include "SSTesting.h"


using namespace std;

void Assert(bool value, const string& hint) {
    if (value)
        return;
    cerr << "Assertion failed." << endl;
    if (hint.length() != 0)
        cerr << "Hint: " << hint << endl;
    abort();
}
void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line,
    const string& hint) {
    if (!value) {
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)
#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

template <typename T>
ostream& operator <<(ostream& os, vector<T> container)
{
    for (T val : container)
        os << val << " ";
    return os;
}
template <typename T>
ostream& operator <<(ostream& os, set<T> container)
{
    for (T val : container)
        os << val << " ";
    return os;
}
template <typename T, typename Y>
ostream& operator <<(ostream& os, map<T,Y> container)
{
    for (pair<T,Y> val : container)
        os << val.first << " " << val.second << " ";
    return os;
}

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
    const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cerr << boolalpha;
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cerr << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)
#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))



void MyAssert()
{
    const string greeting = "Hello"s;
    Assert(!greeting.empty(), "Greeting must be non-empty"s);
    cerr << "This line will not be printed"s << endl;
}

void AssertMacro()
{
    string hello = "hello"s;
    ASSERT(!hello.empty());
    ASSERT_HINT(2 + 2 != 5, "This will fail"s);
}

vector<int> TakeEvens(const vector<int>& numbers) {
    vector<int> evens;
    for (int x : numbers) {
        if (x % 2 == 0) {
            evens.push_back(x);
        }
    }
    return evens;
}
map<string, int> TakeAdults(const map<string, int>& people) {
    map<string, int> adults;
    for (const auto& [name, age] : people) {
        if (age >= 18) {
            adults[name] = age;
        }
    }
    return adults;
}
bool IsPrime(int n) {
    if (n < 2) {
        return false;
    }
    int i = 2;
    while (i * i <= n) {
        if (n % i == 0) {
            return false;
        }
        ++i;
    }
    return true;
}
set<int> TakePrimes(const set<int>& numbers) {
    set<int> primes;
    for (int number : numbers) {
        if (IsPrime(number)) {
            primes.insert(number);
        }
    }
    return primes;
}
void FrameworkForContainers() {
    {
        const set<int> numbers = { -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        const set<int> expected_primes = { 2, 3, 5, 7, 11, 13 };
        ASSERT_EQUAL(TakePrimes(numbers), expected_primes);
    }

    {
        const map<string, int> people = { {"Ivan"s, 19}, {"Sergey"s, 16}, {"Alexey"s, 18} };
        const map<string, int> expected_adults = { {"Alexey"s, 18}, {"Ivan"s, 19} };
        ASSERT_EQUAL(TakeAdults(people), expected_adults);
    }

    {
        const vector<int> numbers = { 3, 2, 1, 0, 3, 6 };
        const vector<int> expected_evens = { 2, 0, 6 };
        ASSERT_EQUAL(TakeEvens(numbers), expected_evens);
    }
}

bool TestAddingSynonymsIncreasesTheirCountFramework() {
    Synonyms synonyms;
    ASSERT(synonyms.GetSynonymCount("music"s) == 0);
    ASSERT(synonyms.GetSynonymCount("melody"s) == 0);

    synonyms.Add("music"s, "melody"s);
    ASSERT(synonyms.GetSynonymCount("music"s) == 1);
    ASSERT(synonyms.GetSynonymCount("melody"s) == 1);

    synonyms.Add("music"s, "tune"s);
    ASSERT(synonyms.GetSynonymCount("music"s) == 2);
    ASSERT(synonyms.GetSynonymCount("tune"s) == 1);
    ASSERT(synonyms.GetSynonymCount("melody"s) == 1);
    return true;
}
bool TestAreSynonymsFramework() {
    Synonyms synonyms;
    ASSERT(synonyms.AreSynonyms("music"s, "melody"s) == 0);

    synonyms.Add("music"s, "melody"s);
    ASSERT(synonyms.AreSynonyms("music"s, "melody"s) == 1);

    synonyms.Add("music"s, "tune"s);
    ASSERT(synonyms.AreSynonyms("music"s, "tune"s) == 1);
    ASSERT(synonyms.AreSynonyms("tune"s, "melody"s) == 0);
    ASSERT(synonyms.AreSynonyms("melody"s, "tune"s) == 0);
    return true;
}
void TestSynonymsFramework()
{
    TestAddingSynonymsIncreasesTheirCountFramework();
    cerr << "TestAddingSynonymsIncreasesTheirCountFramework" << " OK"s << endl;
    TestAreSynonymsFramework();
    cerr << "TestAreSynonymsFramework" << " OK"s << endl;
}

template <typename TestFunction>
void RunTestImpl(TestFunction test, string testName, string fileName, string funcName, unsigned lineNum)
{
    bool testResultBoll = test();
    if (testResultBoll)
        cerr << testName << " OK"s << endl;
}
#define RUN_TEST(func) RunTestImpl(func, #func, __FILE__, __FUNCTION__, __LINE__) 
void TestSynonymsFrameworkRUN_TEST()
{
    RUN_TEST(TestAddingSynonymsIncreasesTheirCountFramework);
    RUN_TEST(TestAreSynonymsFramework);
}

void FrameworkForTesting()
{
    cout << "\n\n FrameworkForTesting \n\n";
    cout << "\n\n MyAssert \n\n";
    MyAssert();
    cout << "\n\n AssertMacro \n\n";
    AssertMacro();
    cout << "\n\n FrameworkForContainers \n\n";
    FrameworkForContainers();
    cout << "\n\n TestSynonymsFramework \n\n";
    TestSynonymsFramework();
    cout << "\n\n TestSynonymsFrameworkRUN_TEST \n\n";
    TestSynonymsFrameworkRUN_TEST();
    cout << "\n\n SearchServerTest \n\n";
    RUN_TEST(TestingSearchServerFramework::SearchServerTest);
}
