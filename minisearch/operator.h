#ifndef _OPERATOR_H_
#define _OPERATOR_H_

#include "database.h"
#include "Trie.h"
#include <set>
#include <map>
#include <algorithm>

using std::vector;
using std::pair;
using std::set;
using std::map;
using std::sort;

//Search by default, no operator

vector<pair<int, vector<int>>> defSearch(vector<string>& s, Database& data);

//Operator -
vector<pair<int, vector<int>>> exSearch(vector<string>& s, vector<string>& ex, Database& data);

//Operator +
vector<pair<int, vector<int>>> inSearch(vector<string>& s, vector<string>& in, Database& data);

//Operator AND
vector<pair<int, vector<int>>> ANDSearch(vector<string>& s1, vector<string>& s2, Database& data);

//Operator OR
vector<pair<int, vector<int>>> ORSearch(vector<string>& s1, vector<string>& s2, Database& data);

//Search exact keywords
vector<pair<int, vector<int>>> exactSearch(vector<string>& s, Database& data);

//Search unknown keywords
vector<pair<int, vector<int>>> unkSearch(vector<string>& s, Database& data);

//title search
vector<pair<int, vector<int>>> titleSearch(vector<string>& s, Database& data);

//filetype search 
vector<pair<int, vector<int>>> typeSearch(string& s, Database& db);
//suggestion
vector<string> suggestion(const string& s, vector<string>& history);

//price search
vector<std::pair<int, vector<int>>> searchPrice(vector<string>& s, Database& db);

#endif _OPERATOR_H_