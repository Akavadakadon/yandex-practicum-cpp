#include "Testing.h"
#include <iostream>
#include <cassert>
#include "ReadFromConsole.h"
#include "Synonyms.h"
#include "Buses.h"
#include "SearchServer.h"

using namespace std;

string IsPalindrome(string str)
{
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	if (str.length() == 0)
		return "not a palindrome";
	//remove_if(str.begin(), str.end(), isspace);
	int len = str.length() -1 ;
	char tmp,* str2 = new char[str.length() + 1];
	strcpy_s(str2, sizeof(char)* str.length() + 1, str.c_str());
	for (int i = 0; i < (str.length()) / 2; i++)
	{
		tmp = str2[len - i];
		str2[len - i] = str2[i];
		str2[i] = tmp;
	}
	string str1223(str2);
	return str== str1223?"palindrome":"not a palindrome";
}
bool Testing_IsPalindrome()
{
	int res = 0;
	assert(IsPalindrome("radar") == "palindrome");
	assert(IsPalindrome("never odd or even") == "palindrome");
	assert(IsPalindrome("apple") == "not a palindrome");
	assert(IsPalindrome("kick") == "not a palindrome");

	assert(IsPalindrome("a") == "palindrome");
	assert(IsPalindrome("            ") == "not a palindrome");

	cout << "Test IsPalindrome() is OK"s << endl;
	return res == 0;
}
void TestExcludeStopWordsFromAddedDocumentContent() {
	const int doc_id = 42;
	const string content = "cat in the city"s;
	const vector<int> ratings = { 1, 2, 3 };
	// —начала убеждаемс€, что поиск слова, не вход€щего в список стоп-слов,
	// находит нужный документ
	{
		TestingSS::SearchServer server;
		server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
		const auto found_docs = server.FindTopDocuments("in"s);
		assert(found_docs.size() == 1);
		const Document& doc0 = found_docs[0];
		assert(doc0.id == doc_id);
	}

	// «атем убеждаемс€, что поиск этого же слова, вход€щего в список стоп-слов,
	// возвращает пустой результат
	{
		TestingSS::SearchServer server;
		server.SetStopWords("in the"s);
		server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
		assert(server.FindTopDocuments("in"s).empty());
	}
}
void SearchServer()
{
	/*SearchServer CreateSearchServer();
	int GetStopWordsSize();
	void AddDocument(int document_id, const string & document);
	set<string> SetStopWords(const string & text);
	set<string> AddStopWords(const string & text);
	vector<Document> FindTopDocuments(const string & query) const;
	vector<Document> FindTopDocuments(const pair <string, vector<string>> query) const;*/


	TestExcludeStopWordsFromAddedDocumentContent();
}


void Testing()
{
	//string asd = ReadFromConsole::ReadLine();
	Testing_IsPalindrome();
	SynonymsTask();
	BusesTask();
	SearchServer();
	cout << "\n\nTesting\n\n";
}


