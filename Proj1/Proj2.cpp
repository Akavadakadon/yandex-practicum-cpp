#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "task2_relevantDocs.h"

using namespace std;

class SearchServer {
    map<string, set<int>> word_to_documents_;
    set<string> stop_words_;
public:
    int GetStopWordsSize()
    {
        return stop_words_.size();
    }

};

void task5()
{
    SearchServer server;
    cout << server.GetStopWordsSize() << endl;
}

void Proj2()
{
	FindRelevantDocs();

    task5();
}
