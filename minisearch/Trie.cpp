#include "Trie.h"
#include "helper.h"
#include <queue>

Trie::Trie() {
	indexNew.clear();
	indexWord.clear();
	end = false;
	for (int i = 0; i < numALP; ++i)
		next[i] = nullptr;
}

void Trie::deallo() {
	for (int i = 0; i < numALP; ++i)
		if (next[i]) {
			next[i]->deallo();
			delete next[i];
		}
}

void Trie::add(const std::string& s, int idNew, int location, Trie*& tmp) {
	tmp = this;
	int i = 0;
	while (i < (int)s.length() && tmp->next[id(s[i])]) tmp = tmp->next[id(s[i++])];
	while (i < (int)s.length()) {
		tmp->next[id(s[i])] = new Trie();
		tmp = tmp->next[id(s[i++])];
	}
	
	tmp->end = 1;
	if (tmp->indexNew.empty() || tmp->indexNew[tmp->indexNew.size()-1] != idNew) {
		tmp->indexNew.push_back(idNew);
		tmp->indexWord.push_back(vector<int>{});
		tmp->indexWord[tmp->indexWord.size() - 1].push_back(location);
	}	
	else tmp->indexWord[tmp->indexWord.size()-1].push_back(location);
}

void Trie::add(const std::vector<std::string>& file, int idNew) {
	Trie* tmp;
	for (int i = 0; i < (int)file.size(); ++i)
		add(file[i], idNew, i, tmp);
}

bool Trie::find(const std::string& s) {
	Trie* tmp = this;
	int i = 0;
	while (i < (int) s.length() && tmp->next[id(s[i])]) 
		tmp = tmp->next[id(s[i++])];
	return i == s.length() && tmp->end;
}

Trie* Trie::Find(const std::string& s) {
	Trie* tmp = this;
	int i = 0;
	while (i < (int) s.length() && tmp->next[id(s[i])]) 
		tmp = tmp->next[id(s[i++])];
	if (i == s.length() && tmp->end) return tmp;
	return NULL;
} 