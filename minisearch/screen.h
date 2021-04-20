#ifndef  _SCREEN_H_
#define _SCREEN_H_

#include "database.h"
#include <iostream>
#include "helper.h"
#include "Trie.h"
#include "query.h"
#include "operator.h"
#include <fstream>
#include <conio.h>

class screen {
private:
	Database db;
public:
	screen();
	~screen();
	
	void menu();
	void drawMenu();

	void update();
	void drawUpdate();

	void history();
	void drawHistory(int page);

	void drawSearch();
	void searchScreen();
	
	void drawResult(vector<std::pair<int, vector<int>>>& data);
	void showResult(vector<std::pair<int, vector<int>>>& data, Database& db);
	
	int ChoosingMenu(const int &x, int &v, const int &firstline, const int &lastline); 
	void updateDatabase();
};

#endif  
