#pragma once
#include "SearchServer.h"
#include "FrameworkForTesting.h"

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <execution>
#include <cmath>

namespace TestingSearchServerFramework {
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

	bool TestExcludeStopWordsFromAddedDocumentContent() {
		const int doc_id = 42;
		const string content = "cat in the city"s;
		const vector<int> ratings = { 1, 2, 3 };
		// Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
		// находит нужный документ
		{
			SearchServer server;
			server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
			const auto found_docs = server.FindTopDocuments("in"s);
			ASSERT(found_docs.size() == 1);
			const Document& doc0 = found_docs[0];
			ASSERT(doc0.document_id == doc_id);
		}

		{
			SearchServer server;
			server.SetStopWords("in the"s);
			server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
			ASSERT(server.FindTopDocuments("in"s).empty());
		}
		return true;
	}

	bool TestFindAllDocumentsWithCertainStatus()
	{
		SearchServer server;
		string content = "cat "s;
		const vector<int> ratings = { 1, 2, 3 };
		for (int i = 0; i < 14; i++)
		{
			content += to_string(i) + " ";
			server.AddDocument(i, content, static_cast<DocumentStatus>(i % 4), ratings);
		}
		ASSERT(server.FindTopDocuments_s("cat", DocumentStatus::ACTUAL).size() == 4);
		ASSERT(server.FindTopDocuments_s("cat", DocumentStatus::IRRELEVANT).size() == 4);
		ASSERT(server.FindTopDocuments_s("cat", DocumentStatus::BANNED).size() == 3);
		ASSERT(server.FindTopDocuments_s("cat", DocumentStatus::REMOVED).size() == 3);
		return true;
	}

	bool TestComputingRelevance()
	{
		SearchServer server;
		vector<Document> documents = server.FindTopDocuments("fat dog");
		sort(documents.begin(), documents.end(), [](Document x, Document y) {return x.document_id < y.document_id; });
		ASSERT(abs(documents[0].relevance - 0) < 0.0001);
		ASSERT(abs(documents[1].relevance - 0) < 0.0001);
		ASSERT(abs(documents[2].relevance - 0) < 0.0001);
		ASSERT(abs(documents[3].relevance - 0) < 0.0001);
		ASSERT(abs(documents[4].relevance - 0.89587973461402748) < 0.0001);
		return true;
	}

	bool TestComputingRating()
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
			ratings.push_back(4 + i);
		}
		auto res = server.FindTopDocuments("cat");
		sort(res.begin(), res.end(), [](Document x, Document y) {return x.document_id < y.document_id; });
		for (Document doc : res)
			ASSERT(doc.rating == all_ratings[doc.document_id]);
		return true;
	}

	bool TestIfMinusWordsIncluded()
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
					res++; });
			return res != 0; });
		ASSERT(counter == 0);
		return true;
	}

	bool TestMatch()
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
		pair <string, vector<string>> queryO = { query.substr(0, query.find('-') - 1),stops };

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

			if (MinusFlag != 0)
				ASSERT(foundMatches.size() == 0);
			else
			{
				sort(matches.begin(), matches.end());
				sort(foundMatches.begin(), foundMatches.end());
				ASSERT(foundMatches == matches);
			}

		}
		return true;
	}

	bool TestSortingRelevance()
	{
		SearchServer ss(1);

		string stop_words_joined("и в на"); //readFromConsole::ReadLine();
		vector<string> docs = { "белый кот и модный ошейник",
"пушистый кот пушистый хвост",
"ухоженный пёс выразительные глаза" };
		string query = "пушистый ухоженный кот";

		const int document_count = docs.size();
		vector<string>::iterator iter = docs.begin();
		ss.SetStopWords(stop_words_joined);
		for (int document_id = 0; document_id < document_count; ++document_id, iter++) {
			ss.AddDocument(document_id, *iter);
		}

		vector<Document> res =  ss.FindTopDocuments(query);
		Document prev;
		for (int i=0;i<res.size();i++)
		{
			if (prev.document_id == -1)
				prev = res[i];
			else
				ASSERT(prev.relevance >= res[i].relevance);
		}

		return true;
	}
	
	bool TestAddDocument()
	{
		{
			SearchServer ss(1);
			string doc = "asd 123 lod nuf asd";
			int doc_id = 1123;
			ss.AddDocument(doc_id, doc);
			auto res = ss.FindTopDocuments(doc);
			ASSERT(find_if(res.begin(), res.end(), [doc_id](Document x) {return doc_id == x.document_id; }) != res.end());
		}
		{
			SearchServer ss(1);
			string doc = "";
			int doc_id = 1123;
			ss.AddDocument(doc_id, doc);
			auto res = ss.FindTopDocuments(doc);
			ASSERT(find_if(res.begin(), res.end(), [doc_id](Document x) {return doc_id == x.document_id; }) != res.end());
		}
		{
			SearchServer ss(1);
			string doc ;
			int doc_id = 1123;
			ss.AddDocument(doc_id, doc);
			auto res = ss.FindTopDocuments(doc);
			ASSERT(find_if(res.begin(), res.end(), [doc_id](Document x) {return doc_id == x.document_id; }) != res.end());
		}
		return true;
	}
	
	bool TestStopWords()
	{
		SearchServer ss(1);

		string stop_words_joined("белый кот"); //readFromConsole::ReadLine();
		vector<string> docs = { "белый кот и модный ошейник",
"пушистый кот пушистый хвост",
"пушистый ухоженный пёс выразительные глаза" };
		string query = "кот";

		const int document_count = docs.size();
		vector<string>::iterator iter = docs.begin();
		ss.SetStopWords(stop_words_joined);
		for (int document_id = 0; document_id < document_count; ++document_id, iter++) {
			ss.AddDocument(document_id, *iter);
		}

		vector<Document> res = ss.FindTopDocuments(query);
		ASSERT(res.size() == 0);
		
		query = "пушистый кот";
		res = ss.FindTopDocuments(query);
		int idOfFoundDoc = 1;
		ASSERT(find_if(res.begin(), res.end(), [idOfFoundDoc](Document x) {return idOfFoundDoc == x.document_id; }) != res.end());
		idOfFoundDoc = 2;
		ASSERT(find_if(res.begin(), res.end(), [idOfFoundDoc](Document x) {return idOfFoundDoc == x.document_id; }) != res.end());

		return true;
	}
	
	bool TestUserPredicate()
	{
		SearchServer ss(1);

		string stop_words_joined("белый кот"); //readFromConsole::ReadLine();
		vector<string> docs = { "пушистый белый кот и модный ошейник",
"пушистый кот пушистый хвост",
"пушистый ухоженный пёс выразительные глаза" };
		string query = "пушистый";
		vector<DocumentStatus> statuses({ DocumentStatus::ACTUAL,DocumentStatus::REMOVED, DocumentStatus::IRRELEVANT });

		const int document_count = docs.size();
		vector<string>::iterator iter = docs.begin();
		ss.SetStopWords(stop_words_joined);
		for (int document_id = 0; document_id < document_count; ++document_id, iter++) {
			ss.AddDocument(document_id, *iter, statuses[document_id], { 1,2,3 });
		}

		vector<Document> res = ss.FindTopDocuments_s(query);
		int idOfFoundDoc = 0;
		ASSERT(find_if(res.begin(), res.end(), [idOfFoundDoc](Document x) {return idOfFoundDoc == x.document_id; }) != res.end());
		res = ss.FindTopDocuments_s(query, DocumentStatus::REMOVED);
		idOfFoundDoc = 1;
		ASSERT(find_if(res.begin(), res.end(), [idOfFoundDoc](Document x) {return idOfFoundDoc == x.document_id; }) != res.end());
		res = ss.FindTopDocuments_s(query, DocumentStatus::IRRELEVANT);
		idOfFoundDoc = 2;
		ASSERT(find_if(res.begin(), res.end(), [idOfFoundDoc](Document x) {return idOfFoundDoc == x.document_id; }) != res.end());
		res = ss.FindTopDocuments_s(query, DocumentStatus::NOSTATUS);
		ASSERT(res.size() == 0);
		return true;
	}

	bool SearchServerTest()
	{
		RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
		//	Добавление документов.Добавленный документ должен находиться по поисковому запросу, который содержит слова из документа.
		RUN_TEST(TestAddDocument);
		//	Поддержка стоп - слов.Стоп - слова исключаются из текста документов.
		RUN_TEST(TestStopWords);
		//	Поддержка минус - слов.Документы, содержащие минус - слова поискового запроса, не должны включаться в результаты поиска.
		RUN_TEST(TestIfMinusWordsIncluded);
		//	Матчинг документов.При матчинге документа по поисковому запросу должны быть возвращены все слова из поискового запроса, присутствующие в документе.Если есть соответствие хотя бы по одному минус - слову, должен возвращаться пустой список слов.
		RUN_TEST(TestMatch);
		//	Сортировка найденных документов по релевантности.Возвращаемые при поиске документов результаты должны быть отсортированы в порядке убывания релевантности.
		RUN_TEST(TestSortingRelevance);
		//	Вычисление рейтинга документов.Рейтинг добавленного документа равен среднему арифметическому оценок документа.
		RUN_TEST(TestComputingRating);
		//	Фильтрация результатов поиска с использованием предиката, задаваемого пользователем.
		RUN_TEST(TestUserPredicate);
		//	Поиск документов, имеющих заданный статус.
		RUN_TEST(TestFindAllDocumentsWithCertainStatus);
		//	Корректное вычисление релевантности найденных документов
		RUN_TEST(TestComputingRelevance);
		return true;
	}
}