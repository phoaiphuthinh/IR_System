#ifndef _HELPER_H
#define _HELPER_H

#include "dirent.h"
#include "Trie.h"

#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <regex>

using std::string;
using std::vector;
using std::ofstream;
using std::cout;
using std::endl;
using std::ostream;

const int delay = 200;

// GET LIST OF FILES NAME IN FOLDER
int getdir(vector<string> &files);
int getdir_update(vector<string>& files);

// WRITE LIST OF FILES TO TXT FILE
void createFilesList();
bool copyFile(const char *SRC, const char* DEST);

string fileNametoPath(string& filename);
int id(const char &c);
vector<string> splitWord(const string& s, int flag = 0);
vector<string> splitWord_DeleteStopwords(Trie &stopwords, const string& s);
void removeSpectialCharData(string &x, bool flag = true);
void removeSpectialCharQuery(string &x);
vector<string> importStopWord();
bool isStopword(Trie &stopword, const string &key);
int BS(const std::vector<int>& a,const int &x);
int BS(const vector<std::pair<int, vector<int>>>& a, const int& x);
vector <string> importHistory();
string removeDot(string s);
void ClearConsoleInputBuffer();
void resizeConsole(int width, int height);
void gotoXY(int x, int y);
void textcolor(int x);
bool getKey(int x);
void cursor(bool flag);
#endif