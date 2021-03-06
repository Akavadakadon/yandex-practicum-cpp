#include "task2_relevantDocs.h"

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <execution>

using namespace std;

namespace taskTwo
{
#define MAX_RESULT_DOCUMENT_COUNT 5

    struct Document
    {
        int document_id;
        int relevance;
    };

    bool HasDocumentLessId(const Document& lhs, const Document& rhs) {
        return lhs.document_id < rhs.document_id;
    }

    bool HasDocumentMoreRelevance(const Document& lhs, const Document& rhs) {
        return lhs.relevance > rhs.relevance;
    }

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

    vector<string> SplitIntoWords(const string& text) {
        vector<string> words;
        string word;
        for (const char c : text) {
            if (c == ' ') {
                words.push_back(word);
                word = "";
            }
            else {
                word += c;
            }
        }
        words.push_back(word);

        return words;
    }

    set<string> ParseStopWords(const string& text) {
        set<string> stop_words;
        for (const string& word : SplitIntoWords(text)) {
            stop_words.insert(word);
        }
        return stop_words;
    }

    vector<string> SplitIntoWordsNoStop(const string& text, const set<string>& stop_words) {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (stop_words.count(word) == 0) {
                words.push_back(word);
            }
        }
        return words;
    }

    void AddDocument(map<string, set<int>>& word_to_documents,
        const set<string>& stop_words,
        int document_id,
        const string& document) {
        for (const string& word : SplitIntoWordsNoStop(document, stop_words)) {
            word_to_documents[word].insert(document_id);
        }
    }

    // For each document returns its id and relevance
    vector<Document> FindAllDocuments(
        const map<string, set<int>>& word_to_documents,
        const set<string>& stop_words,
        const string& query) {
        const vector<string> query_words = SplitIntoWordsNoStop(query, stop_words);
        map<int, int> document_to_relevance;
        for (const string& word : query_words) {
            if (word_to_documents.count(word) == 0) {
                continue;
            }
            for (const int document_id : word_to_documents.at(word)) {
                ++document_to_relevance[document_id];
            }
        }

        vector<Document> found_documents;
        for (auto [document_id, relevance] : document_to_relevance) {
            found_documents.push_back({ document_id, relevance });
        }
        return found_documents;
    }

    vector<Document> FindTopDocuments(
        const map<string, set<int>>& word_to_documents,
        const set<string>& stop_words,
        const string& query)
    {
        vector<Document> all_documents;
        all_documents = FindAllDocuments(word_to_documents, stop_words, query);

        sort(all_documents.begin(), all_documents.end(), [](Document x, Document y) {return x.relevance > y.relevance; });
        vector<Document> top_documents(all_documents.begin(), all_documents.begin() + MAX_RESULT_DOCUMENT_COUNT);
        return top_documents;
    }



    int FindRelevantDocs() {
        const string stop_words_joined("a the on cat");
        const set<string> stop_words = ParseStopWords(stop_words_joined);

        vector<string> docs = { "a fat cat sat on a mat and ate a fat rat",
    "a fat cat",
    "fat rat",
    "a fat cat sat",
    "a fat cat rat",
    "a fat dog" };

        // Read documents
        map<string, set<int>> word_to_documents;
        const int document_count = docs.size();
        vector<string>::iterator iter = docs.begin();
        for (int document_id = 0; document_id < document_count; ++document_id, iter++) {
            AddDocument(word_to_documents, stop_words, document_id, *iter);
        }

        const string query("funny fat cat on a mat with rat");
        for (auto [document_id, relevance] : FindTopDocuments(word_to_documents, stop_words, query)) {
            cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << " }"s << endl;
        }
        return 1;
    }
}
