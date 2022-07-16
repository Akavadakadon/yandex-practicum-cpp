#pragma once
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <execution>
#include <cmath>

using namespace std;

#ifndef Document
struct Document
{
    int document_id;
    double relevance;
    int rating;
};
#endif

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

#ifndef MAX_RESULT_DOCUMENT_COUNT
#define MAX_RESULT_DOCUMENT_COUNT 5
#endif

namespace TestingSS
{
    class SearchServer {
    private:
        map<string, set<int>> word_to_documents_;
        set<string> stop_words_;
        int docs_counter;

        vector<string> SplitIntoWords(const string& text) const;
        vector<string> SplitIntoWordsNoStop(const string& text) const;
        vector<Document> FindAllDocuments(const string& query, vector<string> stopWords = vector<string>())const;

    public:
        SearchServer(int);
        SearchServer();
        SearchServer CreateSearchServer();

        int GetStopWordsSize();
        void AddDocument(int document_id, const string& document);
        set<string> SetStopWords(const string& text);
        set<string> AddStopWords(const string& text);
        vector<Document> FindTopDocuments(const string& query) const;
        vector<Document> FindTopDocuments(const pair <string, vector<string>> query) const;
    };
}
