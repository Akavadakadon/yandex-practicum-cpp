#include "SearchServer.h"

void SearchServer::AddDocument(int document_id, const string& document)
{
    for (const string& word : SplitIntoWordsNoStop(document)) {
        word_to_documents_[word].insert(document_id);
    }
}

int SearchServer::GetStopWordsSize()
{
    return stop_words_.size();
}

vector<string> SearchServer::SplitIntoWordsNoStop(const string& text) const
{
    vector<string> words;
    for (const string& word : SplitIntoWords(text)) {
        if (stop_words_.count(word) == 0) {
            words.push_back(word);
        }
    }
    return words;
}

vector<string> SearchServer::SplitIntoWords(const string& text) const
{
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

set<string> SearchServer::SetStopWords(const string& text)
{
    stop_words_ = set<string>();
    for (const string& word : SplitIntoWords(text)) {
        stop_words_.insert(word);
    }
    return stop_words_;
}

SearchServer SearchServer::CreateSearchServer()
{
    SearchServer search_server;

    const string stop_words_joined("a the on cat");
    search_server.SetStopWords(stop_words_joined);

    vector<string> docs = { "a fat cat sat on a mat and ate a fat rat",
"a fat cat",
"fat rat",
"a fat cat sat",
"a fat cat rat",
"a fat dog" };

    map<string, set<int>> word_to_documents;
    const int document_count = docs.size();
    vector<string>::iterator iter = docs.begin();
    for (int document_id = 0; document_id < document_count; ++document_id, iter++) {
        search_server.AddDocument(document_id, *iter);
    }

    return search_server;
}

vector<Document> SearchServer::FindAllDocuments(const string& query) const
{
    const vector<string> query_words = SplitIntoWordsNoStop(query);
    map<int, int> document_to_relevance;
    for (const string& word : query_words) {
        if (word_to_documents_.count(word) == 0) {
            continue;
        }
        for (const int document_id : word_to_documents_.at(word)) {
            ++document_to_relevance[document_id];
        }
    }

    vector<Document> found_documents;
    for (auto [document_id, relevance] : document_to_relevance) {
        found_documents.push_back({ document_id, relevance });
    }
    return found_documents;
}

vector<Document> SearchServer::FindTopDocuments(const string& query) const
{
    vector<Document> all_documents;
    all_documents = FindAllDocuments(query);

    sort(all_documents.begin(), all_documents.end(), [](Document x, Document y) {return x.relevance > y.relevance; });
    vector<Document> top_documents(all_documents.begin(), all_documents.begin() + MAX_RESULT_DOCUMENT_COUNT);
    return top_documents;
}

SearchServer::SearchServer()
{
    const string stop_words_joined("a the on cat");
    SetStopWords(stop_words_joined);

    vector<string> docs = { "a fat cat sat on a mat and ate a fat rat",
"a fat cat",
"fat rat",
"a fat cat sat",
"a fat cat rat",
"a fat dog" };
    map<string, set<int>> word_to_documents;
    const int document_count = docs.size();
    vector<string>::iterator iter = docs.begin();
    for (int document_id = 0; document_id < document_count; ++document_id, iter++) {
        AddDocument(document_id, *iter);
    }
}

