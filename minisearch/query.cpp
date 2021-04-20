#include "query.h"
#include "helper.h"
#include "operator.h"
#include "database.h"
#include <iostream>
#include <conio.h>

_query::_query() {
	tree = new Trie();
	operation = 0;
	history = importHistory();
	s.clear();
	text.clear();
}

_query::~_query() {
	s.clear();
	text.clear();
	tree->deallo();
	history.clear();
}

bool _query::input() {
	while (1) {
		ClearConsoleInputBuffer();

		int u = 25, v = 16, a = 38, b = 20, noSug = 0, cur = 0;
		char c;
		bool ok;
		textcolor(15);
		gotoXY(u, v); cout << "Input query: ";
		u = 38; v = 16;

		vector<string> sug;

		while (1) {
			if (getKey(VK_UP)) {
				if (cur > 0) {
					gotoXY(a - 5, b + cur - 1);
					cout << "  ";
				}
				--cur;
				if (cur == -1) cur = noSug;
				Sleep(delay);
				ClearConsoleInputBuffer();
			}
			else if (getKey(VK_DOWN)) {
				if (cur > 0) {
					gotoXY(a - 5, b + cur - 1);
					cout << "  ";
				}
				++cur;
				if (cur >= noSug + 1) cur = 0;
				Sleep(delay);
				ClearConsoleInputBuffer();
			}
			else if ((getKey(VK_RIGHT) || getKey(VK_LEFT)) && (cur > 0)) {
				u = 38; v = 16;
				for (int i = 0; i < max(s.size(), sug[cur - 1].size()); ++i) {
					gotoXY(u, v);
					if (i < sug[cur - 1].size()) cout << sug[cur - 1][i];
					else cout << " ";

					++u;
					if (u == 100) {
						u = 38; ++v;
					}
				}

				s = sug[cur - 1];
				u = s.size() % 62 + 38;
				v = s.size() / 62 + 16;

				ClearConsoleInputBuffer();
			}

			else if (_kbhit()) {
				c = _getch();

				if (32 <= c && c <= 126) {
					cur = 0;
					gotoXY(u, v);
					cout << c;
					s += c;
					++u;
					if (u == 100) {
						u = 38; ++v;
					}
				}
				else if (c == VK_BACK) {
					if (u == 38 && v == 16) continue;

					s.pop_back();
					--u;
					if (u == 37) {
						u = 99; --v;
					}

					gotoXY(u, v);
					cout << " ";
				}
				else if (c == VK_RETURN) {
					if (cur > 0) s = sug[cur - 1];
					break;
				}
				else if (c == VK_ESCAPE) return 0;

				cursor(false);

				if (s == "") sug.clear();
				else sug = suggestion(s, history);
				noSug = sug.size();
				for (int i = 0; i < 5; i++) {
					gotoXY(a, b + i);
					if (i < (int)sug.size()) cout << sug[i];
					for (int j = 0; j < 150; ++j)
						cout << " ";
				}
			}

			if (cur == 0) {
				cursor(true);
				gotoXY(u, v);
			}
			else {
				gotoXY(a - 5, b + cur - 1);
				cursor(false);
				cout << "->";
			}
		}

		addHistory(s);
		text = splitWord(s, true);
		if (text.size() == 0) continue;
		tree->add(text, 0);
		return 1;
	}
}

int _query::ChoosingMenu1(int x, int v, int firstline, int lastline, int u1, int v1)
{
	char a;
	int t = x;
	while (true)
	{
		if (_kbhit())
		{
			char c = _getch();
			if (c == 'P' || c == 'S' || c == 's') //down
			{
				gotoXY(x, v);
				cout << (' ');
				if (v >= lastline)
				{
					a = ' ';
					x = u1;
					v = v1;
				}
				else if ((x == u1) && (v == v1))
				{
					a = 175;
					x = t;
					v = 20;
				}
				else
				{
					a = 175;
					x = t;
					v = v + 1;
				}
				gotoXY(x, v);
				cout << a;
			}
			if (c == 'w' || c == 'W' || c == 'H') //up
			{
				gotoXY(x, v);
				cout << (' ');
				if (v <= firstline)
				{
					a = ' ';
					x = u1;
					v = v1;
				}
				else if ((x == u1) && (v == v1))
				{
					a = 175;
					x = t;
					v = 25;
				}
				else
				{
					a = 175;
					x = t;
					v = v - 1;
				}
				gotoXY(x, v);
				cout << a;
			}
			if (c == 'M')
			{
				gotoXY(x, v);
				cout << ' ';
				return v + 30;
			}
			if (c == 13) // 13 la ma ascii cua enter
			{
				return v;
			}
		}
	}
}

int _query::getOperation() {
	if (tree->find("AND")) return operation = 1;
	if (tree->find("OR")) return operation = 2;
	if (tree->find("intitle:")) return operation = 4;
	if (tree->find("filetype:")) return operation = 6;
	if (tree->find("$")) return operation = 7;
	for (int i = 0; i < (int)text.size(); ++i)
		switch (text[i][0])
		{
		case '+': return operation = 5;
			break;
		case '-': return operation = 3;
			break;
		case '\"': return operation = 9;
			break;
		case '*': return operation = 10;
			break;
		default:
			break;
		}
	return operation = 0;
}

vector<std::pair<int, vector<int>>> _query::search(Database& db) {
	db.addHistory(s);

	getOperation();
	vector<std::pair<int, vector<int>>> result;
	switch (operation) {
	case 0: {
		result = defSearch(text, db);
		break;
	}case 1: {
		vector<string> s1, s2;
		int m;
		for (int i = 0; i < (int)text.size(); i++)
			if (text[i] == "AND") {
				m = i;
				break;
			}
		for (int i = 0; i < m; ++i)
			s1.push_back(text[i]);
		for (int i = m + 1; i < (int)text.size(); ++i)
			s2.push_back(text[i]);
		result = ANDSearch(s1, s2, db);
		break;
	}
	case 2: {
		vector<string> s1, s2;
		int m;
		for (int i = 0; i < (int)text.size(); i++)
			if (text[i] == "OR") {
				m = i;
				break;
			}
		for (int i = 0; i < m; ++i)
			s1.push_back(text[i]);
		for (int i = m + 1; i < (int)text.size(); ++i)
			s2.push_back(text[i]);
		result = ORSearch(s1, s2, db);
		break;
	}
	case 3: {
		vector<string> s, ex;
		for (int i = 0; i < (int)text.size(); ++i)
			if (text[i][0] != '-')
				s.push_back(text[i]);
			else {
				text[i].erase(text[i].begin());
				ex.push_back(text[i]);
			}
		result = exSearch(s, ex, db);
		break;
	}
	case 4: {
		result = titleSearch(text, db);
		break;
	}

	case 5: {
		vector<string> s, in;
		for (int i = 0; i < (int)text.size(); ++i)
			if (text[i][0] == '+') {
				text[i].erase(text[i].begin());
				in.push_back(text[i]);
			}
			else
				s.push_back(text[i]);
		result = inSearch(s, in, db);
		break;
	}
	case 6: {
		result = typeSearch(text[1], db);
		break;
	}
	case 7: {
		result = searchPrice(text, db);
		break;
	}

	case 9: {
		if (text[0][0] == '\"')
			text[0].erase(text[0].begin());
		if (text.back().back() == '\"')
			text.back().erase(text.back().end() - 1);
		result = exactSearch(text, db);
		break;
	}
	case 10: {
		result = unkSearch(text, db);
		break;
	}
	default: break;
	}

	return result;
}

void _query::exportHistory() {
	ofstream fout;
	fout.open("data//history.txt");
	for (int i = 0; i < (int)history.size(); i++)
	{
		fout << history[i] << endl;
	}
	fout.close();
}

void _query::addHistory(string&s) {
	if (std::find(history.begin(), history.end(), s) == history.end()) {
		s.resize(70, '\0');
		history.insert(history.begin(), s);
		this->exportHistory();
	}
}