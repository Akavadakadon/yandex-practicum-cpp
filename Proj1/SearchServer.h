#pragma once
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <execution>

using namespace std;

#ifndef Document
struct Document
{
    int document_id;
    int relevance;
};
#endif

#ifndef MAX_RESULT_DOCUMENT_COUNT
#define MAX_RESULT_DOCUMENT_COUNT 5
#endif

class SearchServer {
private:
    map<string, set<int>> word_to_documents_;
    set<string> stop_words_;

    vector<string> SplitIntoWords(const string& text);
    vector<string> SplitIntoWordsNoStop(const string& text);
    vector<Document> FindAllDocuments(const string& query);

public:
    SearchServer();
    // Должна быть конструктором
    SearchServer CreateSearchServer();

    int GetStopWordsSize();
    void AddDocument(int document_id, const string& document);
    set<string> SetStopWords(const string& text);    
    vector<Document> FindTopDocuments(const string& query);
};

