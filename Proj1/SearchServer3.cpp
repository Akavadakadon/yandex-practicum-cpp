#include "SearchServer3.h"


namespace sprint3 {
	SearchServer::SearchServer(const string stop_words)
	{
		for (string str : SplitIntoWords(stop_words))
			stop_words_.insert(str);
	}

	void SearchServer::SetStopWords(const string& text) {
		for (const string& word : SplitIntoWords(text)) {
			stop_words_.insert(word);
		}
	}

	vector<string> SearchServer::SplitIntoWords(const string& text) const
	{
		vector<string> words;
		string word;
		for (const char c : text) {
			if (c == ' ') {
				if (word != "")
					words.push_back(word);
				word = "";
			}
			else {
				word += c;
			}
		}
		if (word != "")
			words.push_back(word);

		return words;
	}

	bool SearchServer::IsSymbol(const char& ch)const
	{
		return (ch >= 0 && ch <= 31);
	}

	bool SearchServer::AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
		if (documents_.find(document_id) != documents_.end())
			return false;
		if (document_id < 0)
			return false;
		for(char ch: document)
			if (IsSymbol(ch))
				return false;
		const vector<string> words = SplitIntoWordsNoStop(document);
		const double inv_word_count = 1.0 / words.size();
		for (const string& word : words) {
			word_to_document_freqs_[word][document_id] += inv_word_count;
		}
		documents_.emplace(document_id,
			DocumentData{
				ComputeAverageRating(ratings),
				status
			});
		return true;
	}

	bool SearchServer::CheckStopWords(const set<string> minus_words)const
	{
		for (string str : minus_words)
			if (str == "")
				return true;
			else
				for (char ch : str)
					if (IsSymbol(ch) || ch == '-')
						return true;
		return false;
	}

	template <typename DocumentPredicate>
	bool SearchServer::FindTopDocuments(const string& raw_query, vector<Document>& documents, DocumentPredicate document_predicate) const {
		const Query query = ParseQuery(raw_query);
		if (CheckStopWords(query.minus_words))
			return false;
		auto matched_documents = FindAllDocuments(query, document_predicate);

		sort(matched_documents.begin(), matched_documents.end(),
			[](const Document& lhs, const Document& rhs) {
				if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
					return lhs.rating > rhs.rating;
				}
				else {
					return lhs.relevance > rhs.relevance;
				}
			});
		if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
			matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
		}
		documents = matched_documents;
		return true;
	}
	bool SearchServer::FindTopDocuments(const string& raw_query, vector<Document>& documents, DocumentStatus status) const {
		return FindTopDocuments(raw_query, documents,[status](int document_id, DocumentStatus document_status, int rating) { return document_status == status; });
	}
	bool SearchServer::FindTopDocuments(const string& raw_query , vector<Document>& documents) const {
		return FindTopDocuments(raw_query, documents, DocumentStatus::ACTUAL);
	}

	int SearchServer::GetDocumentCount() const {
		return documents_.size();
	}

	Query SearchServer::ParseQuery(const string& text) const {
		Query query;
		for (const string& word : SplitIntoWords(text)) {
			const QueryWord query_word = ParseQueryWord(word);
			if (!query_word.is_stop) {
				if (query_word.is_minus) {
					query.minus_words.insert(query_word.data);
				}
				else {
					query.plus_words.insert(query_word.data);
				}
			}
		}
		return query;
	}

	tuple<vector<string>, DocumentStatus> SearchServer::MatchDocument(const string& raw_query, int document_id) const {
		Query query = ParseQuery(raw_query);
		vector<string> matched_words;
		for (const string& word : query.plus_words) {
			if (word_to_document_freqs_.count(word) == 0) {
				continue;
			}
			if (word_to_document_freqs_.at(word).count(document_id)) {
				matched_words.push_back(word);
			}
		}
		for (const string& word : query.minus_words) {
			if (word_to_document_freqs_.count(word) == 0) {
				continue;
			}
			if (word_to_document_freqs_.at(word).count(document_id)) {
				matched_words.clear();
				break;
			}
		}
		return { matched_words, documents_.at(document_id).status };
	}

	vector<string> SearchServer::SplitIntoWordsNoStop(const string& text) const {
		vector<string> words;
		for (const string& word : SplitIntoWords(text)) {
			if (!IsStopWord(word)) {
				words.push_back(word);
			}
		}
		return words;
	}

	int SearchServer::ComputeAverageRating(const vector<int>& ratings) {
		if (ratings.empty()) {
			return 0;
		}
		int rating_sum = 0;
		for (const int rating : ratings) {
			rating_sum += rating;
		}
		return rating_sum / static_cast<int>(ratings.size());
	}

	QueryWord SearchServer::ParseQueryWord(string text) const {
		bool is_minus = false;
		// Word shouldn't be empty
		if (text[0] == '-') {
			is_minus = true;
			text = text.substr(1);
		}
		return {
			text,
			is_minus,
			IsStopWord(text)
		};
	}

	bool SearchServer::IsStopWord(const string& word) const
	{
		return stop_words_.count(word) > 0;
	}

	double SearchServer::ComputeWordInverseDocumentFreq(const string& word) const {
		return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
	}
	template <typename DocumentPredicate>
	vector<Document> SearchServer::FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const 
	{
		map<int, double> document_to_relevance;
		for (const string& word : query.plus_words) {
			if (word_to_document_freqs_.count(word) == 0) {
				continue;
			}
			const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
			for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
				const auto& document_data = documents_.at(document_id);
				if (document_predicate(document_id, document_data.status, document_data.rating)) {
					document_to_relevance[document_id] += term_freq * inverse_document_freq;
				}
			}
		}

		for (const string& word : query.minus_words) {
			if (word_to_document_freqs_.count(word) == 0) {
				continue;
			}
			for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
				document_to_relevance.erase(document_id);
			}
		}
		vector<Document> matched_documents;
		for (const auto [document_id, relevance] : document_to_relevance) {
			matched_documents.push_back({
				document_id,
				relevance,
				documents_.at(document_id).rating
				});
		}
		return matched_documents;
	}
	void PrintDocument(const Document& document) {
		cout << "{ "s
			<< "document_id = "s << document.id << ", "s
			<< "relevance = "s << document.relevance << ", "s
			<< "rating = "s << document.rating
			<< " }"s << endl;
	}
};

