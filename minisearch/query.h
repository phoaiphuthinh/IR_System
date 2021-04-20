#ifndef _QUERY_H
#define _QUERY_H

#include <string>
#include "Trie.h"
#include "database.h"
using std::string;

struct _query {
public:
	_query();
	~_query();
	int getOperation();
	vector<std::pair<int, vector<int>>> search(Database &db);
	bool input();
	int ChoosingMenu1(int x, int v, int firstline, int lastline,int u1,int v1);
	//void importStopword();
	//void importSynonym();
private:
	//Trie* stopword;
	//Trie* synonym;
	Trie* tree;
	string s; 
	vector<string> text;
	int operation;
	vector<string> history;
	void addHistory(string&s);
	void exportHistory();
};

#endif // !_QUERY-H