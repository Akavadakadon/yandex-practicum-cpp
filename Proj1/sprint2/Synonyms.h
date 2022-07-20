#pragma once
#include <string>
#include <map>
#include <set>

int SynonymsTask();

class Synonyms {
public:
    void Add(const std::string& first_word, const std::string& second_word);

    size_t GetSynonymCount(const std::string& word) const;

    bool AreSynonyms(const std::string& first_word, const std::string& second_word) const;

private:
    std::map<std::string, std::set<std::string>> synonyms_;
};
