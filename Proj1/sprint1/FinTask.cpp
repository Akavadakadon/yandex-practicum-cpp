#include "FinTask.h"
#include "../sprint2/SearchServer.h"

using namespace std;
namespace readFromConsole
{

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
}

void FinTask()
{
    SearchServer ss(1);
    string stop_words_joined("� � ��"); //readFromConsole::ReadLine();
    int coint = 3;//readFromConsole::ReadLineWithNumber();

    vector<string> docs = { 
"�������� ��� �������� �����",
"����� ��� � ������ �������",
"��������� �� ������������� �����" };

    map<string, set<int>> word_to_documents;
    const int document_count = docs.size();
    vector<string>::iterator iter = docs.begin();


    ss.SetStopWords(stop_words_joined);
    for (int document_id = 0; document_id < document_count; ++document_id, iter++) {
        ss.AddDocument(document_id, *iter);
    }
    string query("�������� ��������� ��� -������� -�������a"); //readFromConsole::ReadLine();

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
            //stops.push_back(stopWord);
            pos = found + 1;
        }
    }
    pair <string, vector<string>> queryO = { query.substr(0, query.find('-')),stops };

    for (auto [document_id, relevance, status, rating] : ss.FindTopDocuments(queryO)) {
        cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << " }"s << endl;
    }
}
