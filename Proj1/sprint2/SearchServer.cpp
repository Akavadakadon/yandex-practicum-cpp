#include "../sprint2/SearchServer.h"


    void SearchServer::AddDocument(int document_id, const string& document)
    {
        docs_counter++;
        for (const string& word : SplitIntoWordsNoStop(document)) {
            word_to_documents_[word].insert(document_id);
        }
    }

    int SearchServer::ComputeAverageRating(const vector<int>& ratings)
    {
        if (ratings.empty()) {
            return 0;
        }
        double rating = std::accumulate(ratings.begin(), ratings.end(), 0);
        rating /= ratings.size();
        return rating;
    }

    void SearchServer::AddDocument(int document_id, const string& document, vector<int> ratings)
    {
        docs_counter++;
        for (const string& word : SplitIntoWordsNoStop(document)) {
            word_to_documents_[word].insert(document_id);
        }
        this->ratings[document_id] = ComputeAverageRating(ratings);
    }

    void SearchServer::AddDocument(int document_id, const string& document, DocumentStatus status, vector<int> ratings)
    {
        this->statuses[document_id] = status;
        AddDocument(document_id, document, ratings);
    }

    int SearchServer::GetStopWordsSize()
    {
        return stop_words_.size();
    }
    int SearchServer::GetDocumentCount()
    {
        return docs_counter;
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

    set<string> SearchServer::AddStopWords(const string& text)
    {
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

    tuple<vector<string>, DocumentStatus> SearchServer::MatchDocument(const string& raw_query, int document_id) const
    {
        const vector<string> query_words = SplitIntoWordsNoStop(raw_query);
        vector<string> asd;
        for (auto word : query_words)
        {
            auto a = word_to_documents_.at(word).count(document_id);
            auto asd1 = word_to_documents_.at(word);
            if (word_to_documents_.at(word).count(document_id) <= 0)
                continue;
            else
                asd.push_back(word);
        }
        return tuple(asd, statuses.at(document_id));
    }

    tuple<vector<string>, DocumentStatus> SearchServer::MatchDocument(const pair <string, vector<string>> query, int document_id) const
    {
        const vector<string> query_words = SplitIntoWordsNoStop(query.first);
        vector<string> matchingWords;

        set<int> minusDocks;
        for (auto word : query.second)
        {
            if (word_to_documents_.find(word) == word_to_documents_.end())
                continue;
            if (word_to_documents_.at(word).find(document_id) != word_to_documents_.at(word).end())
                return tuple(vector<string>(), DocumentStatus::NOSTATUS);
        }

        for (auto word : query_words)
        {
            if (word_to_documents_.find(word) == word_to_documents_.end())
                continue;
            auto a = word_to_documents_.at(word).count(document_id);
            if (word_to_documents_.at(word).count(document_id) <= 0)
                continue;
            else
                matchingWords.push_back(word);
        }
        DocumentStatus status = statuses.find(document_id) == statuses.end() ? DocumentStatus::NOSTATUS : statuses.at(document_id);
        return tuple(matchingWords, status);
    }

    vector<Document> SearchServer::FindAllDocuments(const string& query, vector<string> stopWords) const
    {
        const vector<string> query_words = SplitIntoWordsNoStop(query);
        map<int, double> document_to_relevance;
        map<string, double> IDF;
        for (const string& word : query_words) {
            if (word_to_documents_.count(word) == 0) {
                continue;
            }
            int count = word_to_documents_.find(word)->second.size();
            IDF[word] = (log((double)docs_counter / count));
        }

        for (auto doc : word_to_documents_)
        {
            if (query.find(doc.first) == -1)
                continue;
            for (int document_id : doc.second)
            {
                int doc_size = 0;
                for (auto doc : word_to_documents_)
                    doc_size += doc.second.count(document_id);
                double res = (double)1 / doc_size * IDF[doc.first];
                document_to_relevance[document_id] += res;
            }
        }

        vector<int> failed;
        for (auto doc : word_to_documents_)
        {
            if (find(stopWords.begin(), stopWords.end(), doc.first) == stopWords.end())
                continue;
            for (int a : doc.second)
                failed.push_back(a);
        }

        vector<Document> found_documents;
        for (auto [document_id, relevance] : document_to_relevance) {
            if (find(failed.begin(), failed.end(), document_id) == failed.end())
                ;
            found_documents.push_back({
            .document_id = document_id,
            .relevance = relevance,
            .rating = ratings.size() != 0 ? ratings.at(document_id) : NULL ,
            .status = statuses.size() != 0 ? statuses.at(document_id) : DocumentStatus::NOSTATUS
                });
        }
        return found_documents;
    }

    vector<Document> SearchServer::FindTopDocuments(const string& query) const
    {
        vector<Document> all_documents;
        all_documents = FindAllDocuments(query);

        sort(all_documents.begin(), all_documents.end(), [](Document x, Document y) {
            //return x.relevance > y.relevance; 
            double diff = abs(x.relevance - y.relevance);
            double EPSILON = 1e-6;
            if (diff < EPSILON)
                return x.rating > y.rating;
            return x.relevance > y.relevance;
            });
        vector<Document> top_documents(all_documents.begin(), all_documents.begin() + (MAX_RESULT_DOCUMENT_COUNT <= all_documents.size() ? MAX_RESULT_DOCUMENT_COUNT : all_documents.size()));
        return top_documents;
    }

    vector<Document> SearchServer::FindTopDocuments_s(const string& query, DocumentStatus status) const
    {
        vector<Document> all_documents;
        all_documents = FindAllDocuments(query);

        sort(all_documents.begin(), all_documents.end(), [](Document x, Document y) {return x.relevance > y.relevance; });
        vector<Document> top_documents(all_documents.begin(), all_documents.begin() + (MAX_RESULT_DOCUMENT_COUNT <= all_documents.size() ? MAX_RESULT_DOCUMENT_COUNT : all_documents.size()));
        vector<Document> filteredTopDocuments;
        copy_if(top_documents.begin(), top_documents.end(), back_inserter(filteredTopDocuments), [status](Document x) {return x.status == status; });
        return filteredTopDocuments;
    }

    vector<Document> SearchServer::FindTopDocuments(const pair <string, vector<string>> query) const
    {
        vector<Document> all_documents;
        all_documents = FindAllDocuments(query.first, query.second);

        sort(all_documents.begin(), all_documents.end(), [](Document x, Document y) {return x.relevance > y.relevance; });
        vector<Document> top_documents(all_documents.begin(), all_documents.begin() + (MAX_RESULT_DOCUMENT_COUNT <= all_documents.size() ? MAX_RESULT_DOCUMENT_COUNT : all_documents.size()));
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
    SearchServer::SearchServer(int)
    {

    }

