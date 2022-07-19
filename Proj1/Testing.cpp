#include "Testing.h"
#include <iostream>
#include <cassert>
#include "ReadFromConsole.h"
#include "Synonyms.h"
#include "Buses.h"
#include "SearchServer.h"

using namespace std;

vector<string> SplitIntoWords(const string& text) 
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
	// Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
	// находит нужный документ
	{
		SearchServer server;
		server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
		const auto found_docs = server.FindTopDocuments("in"s);
		assert(found_docs.size() == 1);
		const Document& doc0 = found_docs[0];
		assert(doc0.document_id == doc_id);
	}

	{
		SearchServer server;
		server.SetStopWords("in the"s);
		server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
		assert(server.FindTopDocuments("in"s).empty());
	}
}

void TestFindAllDocumentsWithCertainStatus()
{
	SearchServer server;
	string content = "cat "s;
	const vector<int> ratings = { 1, 2, 3 };
	for (int i = 0; i < 14; i++)
	{
		content += to_string(i) + " ";
		server.AddDocument(i, content, static_cast<DocumentStatus>(i%4), ratings);
	}
	assert(server.FindTopDocuments_s("cat", DocumentStatus::ACTUAL).size() == 4);
	assert(server.FindTopDocuments_s("cat", DocumentStatus::IRRELEVANT).size() == 4);
	assert(server.FindTopDocuments_s("cat", DocumentStatus::BANNED).size() == 3);
	assert(server.FindTopDocuments_s("cat", DocumentStatus::REMOVED).size() == 3);
}

void TestComputingRelevance()
{
	SearchServer server;
	vector<Document> documents = server.FindTopDocuments("fat dog");
	sort(documents.begin(), documents.end(), [](Document x, Document y) {return x.document_id < y.document_id; });
	assert(abs(documents[0].relevance - 0) < 0.0001);
	assert(abs(documents[1].relevance - 0) < 0.0001);
	assert(abs(documents[2].relevance - 0) < 0.0001);
	assert(abs(documents[3].relevance - 0) < 0.0001);
	assert(abs(documents[4].relevance - 0.89587973461402748) < 0.0001);
}

void TestComputingRating()
{
	SearchServer server;
	string content = "cat "s;
	vector<int> ratings = { 1, 2, 3 };
	map<int, int> all_ratings;
	for (int i = 0; i < 14; i++)
	{
		content += to_string(i) + " ";
		server.AddDocument(i, content, ratings);
		int rate = reduce(ratings.begin(), ratings.end()) / ratings.size();
		all_ratings.insert({ i,rate });
		ratings.push_back(4+i);
	}
	auto res = server.FindTopDocuments("cat");
	sort(res.begin(), res.end(), [](Document x, Document y) {return x.document_id < y.document_id; });
	for (Document doc : res)
		assert(doc.rating == all_ratings[doc.document_id]);
}

void TestIfMinusWordsIncluded()
{
	SearchServer server(1);
	const string stop_words_joined("a the on cat");
	server.SetStopWords(stop_words_joined);

	vector<string> docs = { "a fat cat sat on a mat and ate a fat rat",
"a fat cat",
"fat rat",
"a fat cat sat",
"a fat cat rat",
"a fat dog" };
	const int document_count = docs.size();
	vector<string>::iterator iter = docs.begin();
	for (int document_id = 0; document_id < document_count; ++document_id, iter++) {
		server.AddDocument(document_id, *iter);
	}
	string query("fat dog -rat -asd");
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
			stops.push_back(stopWord);
			pos = found + 1;
		}
	}
	pair <string, vector<string>> queryO = { query.substr(0, query.find('-')),stops };

	vector<Document> res = server.FindTopDocuments(queryO);
	auto counter = count_if(res.begin(), res.end(), [docs, stops](Document x) {
		int res = 0;
		for_each(stops.begin(), stops.end(), [&res, docStr = docs[x.document_id]](string str) {
			if (docStr.find(str) != -1)
				res++;});
		return res != 0;});
	assert(counter == 0);
}

void TestMatch()
{
	SearchServer server;
	vector<string> docs = { "a fat cat sat on a mat and ate a fat rat",
"a fat cat",
"fat rat",
"a fat cat sat",
"a fat cat rat",
"a fat dog" };
	string query("fat dog -rat -asd");
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
			stops.push_back(stopWord);
			pos = found + 1;
		}
	}
	pair <string, vector<string>> queryO = { query.substr(0, query.find('-')-1),stops };

	for (int i = 0; i < docs.size(); i++)
	{
		int MinusFlag = 0;
		for_each(stops.begin(), stops.end(), [&MinusFlag, docStr = docs[i]](string str) {
			if (docStr.find(str) != -1)
				MinusFlag++; });

		vector<string> foundMatches;
		DocumentStatus status;
		tie(foundMatches, status) = server.MatchDocument(queryO, i);

		vector<string> queryWords = SplitIntoWords(queryO.first);
		vector<string> matches;
		for (string word : queryWords)
		{
			if (docs[i].find(word) != -1)
				matches.push_back(word);
		}

		if(MinusFlag!=0)
			assert(foundMatches.size() == 0);
		else
		{
			sort(matches.begin(), matches.end());
			sort(foundMatches.begin(), foundMatches.end());
			assert(foundMatches == matches);
		}

	}
}

void SearchServerTest()
{
	TestExcludeStopWordsFromAddedDocumentContent();
	//	Добавление документов.Добавленный документ должен находиться по поисковому запросу, который содержит слова из документа.
	
	//	Поддержка стоп - слов.Стоп - слова исключаются из текста документов.
	
	//	Поддержка минус - слов.Документы, содержащие минус - слова поискового запроса, не должны включаться в результаты поиска.
	TestIfMinusWordsIncluded();
	//	Матчинг документов.При матчинге документа по поисковому запросу должны быть возвращены все слова из поискового запроса, присутствующие в документе.Если есть соответствие хотя бы по одному минус - слову, должен возвращаться пустой список слов.
	TestMatch();
	//	Сортировка найденных документов по релевантности.Возвращаемые при поиске документов результаты должны быть отсортированы в порядке убывания релевантности.
	TestComputingRelevance();
	//	Вычисление рейтинга документов.Рейтинг добавленного документа равен среднему арифметическому оценок документа.
	TestComputingRating();
	//	Фильтрация результатов поиска с использованием предиката, задаваемого пользователем.
	 
	//	Поиск документов, имеющих заданный статус.
	TestFindAllDocumentsWithCertainStatus();
	//	Корректное вычисление релевантности найденных документов
}


void Testing()
{
	cout << "\n\n Testing \n\n";

	cout << "\n\n Testing_IsPalindrome \n\n";
	Testing_IsPalindrome();
	cout << "\n\n SynonymsTask \n\n";
	SynonymsTask();
	cout << "\n\n BusesTask \n\n";
	BusesTask();
	cout << "\n\n SearchServerTest \n\n";
	SearchServerTest();
}


