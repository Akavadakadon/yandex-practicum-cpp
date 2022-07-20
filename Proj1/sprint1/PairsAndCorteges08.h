#pragma once
#include "TaskTracker.h"
#include "../sprint2/SearchServer.h"

namespace ImprovingComparisonNamespace
{
    struct Document {
        int id;
        double relevance;
        int rating;
    };
    vector<ImprovingComparisonNamespace::Document> SortDocuments(vector<ImprovingComparisonNamespace::Document> matched_documents)
    {
        sort(matched_documents.begin(), matched_documents.end(),
            [](const Document& lhs, const Document& rhs) {
                if (lhs.rating != rhs.rating)
                    return lhs.rating > rhs.rating;
                return lhs.relevance > rhs.relevance;
            });
        return matched_documents;
    }
    void ImprovingComparison()
    {
        vector<ImprovingComparisonNamespace::Document> documents = {
            {100, 0.5, 4},
            {101, 1.2, 4},
            {102, 0.3, 5}
        };
        vector<ImprovingComparisonNamespace::Document> res = SortDocuments(documents);
        for (const ImprovingComparisonNamespace::Document& document : res) {
            cout << document.id << ' '
                << document.relevance << ' '
                << document.rating << endl;
        }
    }
}

namespace PairsInComparisonNamespace
{
    enum DocumentStatus
    {
        ACTUAL, EXPIRED, DELETED
    };
    struct Document {
        int id;
        DocumentStatus status;
        double relevance;
        int rating;
    };
    vector<Document> SortDocuments(vector<Document> matched_documents)
    {
        sort(matched_documents.begin(), matched_documents.end(),
            [](const Document& lhs, const Document& rhs) {
                return pair(lhs.rating, lhs.relevance) > pair(rhs.rating, rhs.relevance) && lhs.status <= rhs.status;
            });
        return matched_documents;
    }
    void ImprovingComparison()
    {
        vector<Document> documents = {
        {100, DocumentStatus::ACTUAL,  0.5, 4},
        {101, DocumentStatus::EXPIRED, 0.5, 4},
        {102, DocumentStatus::ACTUAL,  1.2, 4},
        {103, DocumentStatus::DELETED, 1.2, 4},
        {104, DocumentStatus::ACTUAL,  0.3, 5},
        };
        vector<Document> res = SortDocuments(documents);
        for (const Document& document : res) {
            cout << document.id << ' '
                << static_cast<int>(document.status) << ' '
                << document.relevance << ' '
                << document.rating << endl;
        }
    }
}

namespace TuplesInComparisonNamespace
{
    enum DocumentStatus
    {
        ACTUAL, EXPIRED, DELETED
    };
    struct Document {
        int id;
        DocumentStatus status;
        double relevance;
        int rating;

        auto MakeKey() const
        {
            return tuple(status * -1, rating, relevance);
        }
    };
    vector<Document> SortDocuments(vector<Document> matched_documents)
    {
        sort(matched_documents.begin(), matched_documents.end(),
            [](const Document& lhs, const Document& rhs) {
                return lhs.MakeKey() > rhs.MakeKey();
            });
        return matched_documents;
    }
    void ImprovingComparison()
    {
        vector<Document> documents = {
        {100, DocumentStatus::ACTUAL,  0.5, 4},
        {101, DocumentStatus::EXPIRED, 0.5, 4},
        {102, DocumentStatus::ACTUAL,  1.2, 4},
        {103, DocumentStatus::DELETED, 1.2, 4},
        {104, DocumentStatus::ACTUAL,  0.3, 5},
        };
        vector<Document> res = SortDocuments(documents);
        for (const Document& document : res) {
            cout << document.id << ' '
                << static_cast<int>(document.status) << ' '
                << document.relevance << ' '
                << document.rating << endl;
        }
    }
}

namespace ReturnMultipleValues
{
    void PrintMatchDocumentResult(int document_id, const vector<string>& words, DocumentStatus status) {
        cout << "{ "s
            << "document_id = "s << document_id << ", "s
            << "status = "s << static_cast<int>(status) << ", "s
            << "words ="s;
        for (const string& word : words) {
            cout << ' ' << word;
        }
        cout << "}"s << endl;
    }
    void ReturnMultipleValues()
    {
        SearchServer search_server(1);
        search_server.SetStopWords("и в на"s);

        search_server.AddDocument(0, "белый кот и модный ошейник"s, DocumentStatus::ACTUAL, { 8, -3 });
        search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL, { 5, -12, 2, 1 });
        search_server.AddDocument(3, "ухоженный скворец евгений"s, DocumentStatus::BANNED, { 9 });

        const int document_count = search_server.GetDocumentCount();
        for (int document_id = 0; document_id < document_count; ++document_id) {
            const auto [words, status] = search_server.MatchDocument("пушистый кот"s, document_id);
            PrintMatchDocumentResult(document_id, words, status);
        }
    }
}

void RealNumbers()
{
    SearchServer search_server;
    search_server.SetStopWords("и в на"s);

    search_server.AddDocument(0, "белый кот и модный ошейник"s, DocumentStatus::ACTUAL, { 8, -3 });
    search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
    search_server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL, { 5, -12, 2, 1 });

    for (const Document& document : search_server.FindTopDocuments("ухоженный кот"s)) {
        PrintDocument(document);
    }
}

void PairsAndCorteges08()
{
    setlocale(LC_ALL, "Russian");

	cout << "\n\n PairsAndCorteges08 \n";
    cout << "\n\n ImprovingComparison \n";
    ImprovingComparisonNamespace::ImprovingComparison();
    cout << "\n\n PairsInComparison \n";
    PairsInComparisonNamespace::ImprovingComparison();
    cout << "\n\n TuplesInComparison \n";
    TuplesInComparisonNamespace::ImprovingComparison();
    cout << "\n\n ReturnMultipleValues \n";
    ReturnMultipleValues::ReturnMultipleValues();
    cout << "\n\n RealNumbers \n";
    RealNumbers();
    cout << "\n\n TasksTracker \n";
    TaskTracker();
}

