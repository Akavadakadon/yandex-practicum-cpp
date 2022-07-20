#include <iostream>
#include "task2_relevantDocs.h"
#include "../sprint2/SearchServer.h"

using namespace std;

void task5()
{
    SearchServer server;
    const SearchServer asd = server.CreateSearchServer();

    const string query("funny fat cat on a mat with rat");
    for (auto [document_id, relevance, status, rating] : asd.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << " }"s << endl;
    }

}

void Proj2()
{
    cout << "\n\t\tSecond chapter\n" << endl;
    taskTwo::FindRelevantDocs();
    cout << "\nSerch engine works as the class\n" << endl;
    task5();
}
