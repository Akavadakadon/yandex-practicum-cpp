#pragma once

#include <sstream>
#include "../sprint2/SearchServer.h"


void Rating1()
{

    SearchServer ss(1);
    string stop_words_joined("и в на"); //readFromConsole::ReadLine();
    int coint = 3;//readFromConsole::ReadLineWithNumber();

    vector<string> docs = {
"пушистый кот пушистый хвост",
"белый кот и модный ошейник",
"ухоженный пёс выразительные глаза" };
    vector<string> ratingsStr = {
"2 8 -3",
"3 7 2 7",
"4 5 -12 2 1" };
    vector<int> ratingsInt;
    stringstream sstream;

    map<string, set<int>> word_to_documents;
    const int document_count = docs.size();
    vector<string>::iterator iter = docs.begin();


    ss.SetStopWords(stop_words_joined);
    for (int document_id = 0; document_id < document_count; ++document_id, iter++) {
        ratingsInt.clear();
        sstream << ratingsStr[document_id];
        string temp;
        while (getline(sstream, temp, ' '))
            ratingsInt.push_back(stoi(temp));
        ratingsInt.erase(ratingsInt.begin());
        sstream.clear();
        ss.AddDocument(document_id, *iter, ratingsInt);
    }
    string query("пушистый ухоженный кот -ошейник -ошейникa"); //readFromConsole::ReadLine();

    int pos = 0;
    size_t found = 1;
    vector<string> stops;
    while (found != string::npos)
    {
        found = query.find('-', pos);
        if (found != string::npos)
        {
            int endStopWord = query.find(" ", found) - 1;
            endStopWord = endStopWord < 0 ? query.size() + 1 : endStopWord;
            string stopWord = query.substr(found + 1, endStopWord - found);
            //stops.push_back(stopWord); // No stop words for this task
            pos = found + 1;
        }
    }
    pair <string, vector<string>> queryO = { query.substr(0, query.find('-')),stops };

    for (auto [document_id, relevance, status, rating] : ss.FindTopDocuments(queryO)) {
        cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << ", rating = "s << (int)rating << " }"s << endl;
    }
}

void Overflow()
{
    int64_t min = numeric_limits<int64_t>::min();
    uint64_t max = numeric_limits<int64_t>::max();
    cout << min << endl;
    cout << max << endl;
    cout << min + max<< endl;
    cout << min*2 << endl;
    cout << max*2 << endl;
}

void PrintDocuments(vector<Document> documents, size_t skip_start, size_t skip_finish)
{
    auto adsdasdas = documents.size() - (skip_start + skip_finish) <= 0;
    if (skip_start >= documents.size() || skip_finish >= documents.size() || documents.size() - (skip_start + skip_finish) <= 0)
        return;
    sort(documents.begin(), documents.end(), [](Document x, Document y) {
        return x.rating > y.rating;
        });

    auto iterStart = documents.begin() + skip_start;
    for (int i = 0; i < documents.size() - (skip_start + skip_finish); i++)
    {
        cout << "{ document_id = "s <<  (*(iterStart+i)).document_id << ", rating = "s << (*(iterStart+i)).rating << " }"s << endl;
    }

}

void PrintDocument(Document document)
{
    cout << "{ "
        << "document_id = " << document.document_id << ", "
        << "relevance = " << document.relevance << ", "
        << "rating = " << document.rating
        << " }" << endl;
}

void enumTypes()
{
    SearchServer search_server;
    search_server.SetStopWords("и в на"s);

    search_server.AddDocument(0, "белый кот и модный ошейник"s, DocumentStatus::ACTUAL, { 8, -3 });
    search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
    search_server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL, { 5, -12, 2, 1 });
    search_server.AddDocument(3, "ухоженный скворец евгений"s, DocumentStatus::BANNED, { 9 });

    cout << "ACTUAL:"s << endl;
    for (const Document& document : search_server.FindTopDocuments_s("пушистый ухоженный кот"s, DocumentStatus::ACTUAL)) {
        PrintDocument(document);
    }

    cout << "BANNED:"s << endl;
    for (const Document& document : search_server.FindTopDocuments_s("пушистый ухоженный кот"s, DocumentStatus::BANNED)) {
        PrintDocument(document);
    }

}
void enumTypes2()
{
    SearchServer search_server;
    search_server.SetStopWords("и в на"s);

    search_server.AddDocument(0, "белый кот и модный ошейник"s, DocumentStatus::ACTUAL, { 8, -3 });
    search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
    search_server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL, { 5, -12, 2, 1 });
    search_server.AddDocument(3, "ухоженный скворец евгений"s, DocumentStatus::BANNED, { 9 });

    cout << "ACTUAL:"s << endl;
    for (const Document& document : search_server.FindTopDocuments_s("пушистый ухоженный кот"s)) {
        PrintDocument(document);
    }

    cout << "BANNED:"s << endl;
    for (const Document& document : search_server.FindTopDocuments_s("пушистый ухоженный кот"s, DocumentStatus::BANNED)) {
        PrintDocument(document);
    }

}

void NumericTypes07()
{
    cout << "\n\n NumericTypes07 \n";
    cout << "\n\n Rating1 \n";
	Rating1();
    cout << "\n\n Overflow \n";
    //Overflow();
    cout << "\n\n PrintDocuments \n";
    PrintDocuments(
        {
            {.document_id = 100, .rating = 5},
            {.document_id = 101, .rating = 7},
            {.document_id = 102, .rating = -4},
            {.document_id = 103, .rating = 9},
            {.document_id = 104, .rating = 1}
        },
        4,
        0
    );
    cout << "\n\n enumTypes \n";
    enumTypes();
    enumTypes2();
}


