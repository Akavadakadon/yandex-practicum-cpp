#include <iostream>
#include "task2_relevantDocs.h"
#include "SearchServer.h"

using namespace std;

void task5()
{
    SearchServer server;
    SearchServer asd = server.CreateSearchServer();

    const string query("funny fat cat on a mat with rat");
    for (auto [document_id, relevance] : asd.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << " }"s << endl;
    }

}

void Proj2()
{
	FindRelevantDocs();
    cout << "\nSerch engine works as the class\n" << endl;

    task5();
}
