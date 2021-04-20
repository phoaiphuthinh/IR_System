#ifndef _TRIE_H
#define _TRIE_H
#include <string>
#include <vector>
#include <queue>
const int numALP = 44;

class Trie {
public:
	Trie(); 
	void add(const std::string& s, int idNew, int location, Trie*& tmp);
	void add(const std::vector<std::string>& file, int idNew);

	bool find(const std::string& s);
	Trie* Find(const std::string& s);
	void deallo();

	std::vector<std::vector<int>> indexWord;
	std::vector<int> indexNew;
private:
	bool end;
	Trie* next[numALP];
};

#endif