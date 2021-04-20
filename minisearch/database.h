#ifndef _DATABASE_H
#define _DATABASE_H
#include <string>
#include <vector>
#include "dirent.h"
#include "helper.h"
using std::vector;
using std::string;

class New {
private:
	string name; //id of news
public:
	New(string& NAME);
	void print();
	void printScreen(int x, int y, vector<int> pos);
	void deallo();
	void printScreenFull();
	void xuongdong(string s);

};

class Database {
private:
	//Trie* stopwords;
	int numOfNew;
public:
	vector<New> News;
	vector<string> history;
	Trie* fileList;
	Trie* content,* title;
	Database();
	void deallo();
	void Import();
	void updateDb();
	void addHistory(const string&s);
	void exportHistory();
};
#endif