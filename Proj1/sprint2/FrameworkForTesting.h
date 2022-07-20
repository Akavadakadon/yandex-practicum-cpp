#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

void FrameworkForTesting();

void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line,
    const string& hint);

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
    const string& func, unsigned line, const string& hint);

template <typename TestFunction>
void RunTestImpl(TestFunction test, string testName, string fileName, string funcName, unsigned lineNum);

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)
#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)
#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

#define RUN_TEST(func) RunTestImpl(func, #func, __FILE__, __FUNCTION__, __LINE__) 
