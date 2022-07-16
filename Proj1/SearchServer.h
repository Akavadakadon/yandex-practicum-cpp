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


<<<<<<< HEAD
enum DocumentStatus
{
    ACTUAL, IRRELEVANT, BANNED, REMOVED, NOSTATUS
};

=======
>>>>>>> 5b2989887d891673d7231b7eca7f0d431ade1134
#ifndef Document
struct Document
{
    int document_id;
    double relevance;
    double rating;
<<<<<<< HEAD
    DocumentStatus status;
=======
    SearchServer::DocumentStatus status;
>>>>>>> 5b2989887d891673d7231b7eca7f0d431ade1134
};
#endif

#ifndef MAX_RESULT_DOCUMENT_COUNT
#define MAX_RESULT_DOCUMENT_COUNT 5
#endif

class SearchServer {
public:
<<<<<<< HEAD
=======
    enum DocumentStatus
    {
        ACTUAL, IRRELEVANT, BANNED, REMOVED, NOSTATUS
    };
>>>>>>> 5b2989887d891673d7231b7eca7f0d431ade1134

private:
    map<string, set<int>> word_to_documents_;
    set<string> stop_words_;
    map<int, double> ratings;
    map<int, DocumentStatus> statuses;
    int docs_counter;

    static int ComputeAverageRating(const vector<int>& ratings);
    vector<string> SplitIntoWords(const string& text) const;
    vector<string> SplitIntoWordsNoStop(const string& text) const;
    vector<Document> FindAllDocuments(const string& query, vector<string> stopWords = vector<string>())const;

public:
    SearchServer(int);
    SearchServer();
    // Должна быть конструктором
    SearchServer CreateSearchServer();

    int GetStopWordsSize();
    void AddDocument(int document_id, const string& document);
    void AddDocument(int document_id, const string& document, vector<int> ratings);
    void AddDocument(int document_id, const string& document, DocumentStatus status, vector<int> ratings);
    set<string> SetStopWords(const string& text);
    set<string> AddStopWords(const string& text);
    vector<Document> FindTopDocuments(const string& query) const;
    vector<Document> FindTopDocuments_s(const string& query, DocumentStatus status = DocumentStatus::ACTUAL) const;
    vector<Document> FindTopDocuments(const pair <string, vector<string>> query) const;
};

