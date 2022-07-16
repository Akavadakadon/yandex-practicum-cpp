#pragma once

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

void PairsAndCorteges08()
{
	cout << "\n\n PairsAndCorteges08 \n";
    cout << "\n\n ImprovingComparison \n";
    ImprovingComparisonNamespace::ImprovingComparison();
    cout << "\n\n PairsInComparison \n";
    PairsInComparisonNamespace::ImprovingComparison();
    cout << "\n\n TuplesInComparison \n";
    TuplesInComparisonNamespace::ImprovingComparison();
}

