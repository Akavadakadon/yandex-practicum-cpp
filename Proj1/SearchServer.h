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

namespace sprint3 {
#ifndef MAX_RESULT_DOCUMENT_COUNT
#define MAX_RESULT_DOCUMENT_COUNT 5
#endif

    struct Document {
        int id;
        double relevance;
        int rating;
    };
    enum class DocumentStatus {
        ACTUAL,
        IRRELEVANT,
        BANNED,
        REMOVED,
    };
    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };
    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    class SearchServer {
    public:
        void SetStopWords(const string& text);
        void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings);
        template <typename DocumentPredicate> vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const;
        vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const;
        vector<Document> FindTopDocuments(const string& raw_query) const;
        int GetDocumentCount() const;
        tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const;

        explicit SearchServer() = default;
        explicit SearchServer(const string stop_words);
        template <typename StringCollection> 
        explicit SearchServer(const StringCollection& stop_words_container)
        {
            for (string str : stop_words_container)
                stop_words_.insert(str);
        }
    private:
        set<string> stop_words_;
        map<int, DocumentData> documents_;
        map<string, map<int, double>> word_to_document_freqs_;

        vector<string> SplitIntoWords(const string& text) const;
        bool IsStopWord(const string& word) const;
        vector<string> SplitIntoWordsNoStop(const string& text) const;
        QueryWord ParseQueryWord(string text) const;
        Query ParseQuery(const string& text) const;
        double ComputeWordInverseDocumentFreq(const string& word) const;
        template <typename DocumentPredicate> vector<Document> FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const;

        static int ComputeAverageRating(const vector<int>& ratings);
    };

}

