#include "database.h"
#include "helper.h"
#include "microtimer.h"
#include <algorithm>
#include<stdio.h>


using std::ifstream;

New::New(string& NAME) {
	name = NAME;
}

void New::deallo() {
	name.clear();
}

void New::print() {
	cout << name << "\n";
	/*ifstream infile("News//" + name);
	if (!infile.good())return;
	string s;
	getline(infile, s);
	cout << s << "\n\n";
	infile.close();*/
}

void New::printScreen(int x, int y, vector<int> pos) {
	ifstream infile("News//" + name);
	if (!infile.good())return;
	string s, sa;
	getline(infile, s);
	vector<string> p = splitWord(s, 2);
	std::sort(pos.begin(), pos.end());
	textcolor(8);
	gotoXY(x, y); cout << New::name << "\n";
	textcolor(11);
	gotoXY(x, y + 1);
	//cout << s << "\n\n";
	int cnt = 0;
	int j = 0;
	for (int i = 0; i < p.size(); i++) {
		string ss = p[i];
		removeSpectialCharData(ss);
		if (ss == "")
			continue;
		if (j < pos.size() && pos[j] == i) {
			textcolor(14);
			j++;
		}
		else
			textcolor(11);
		cout << p[i] << " ";
		cnt += p[i].size();
		if (cnt > 100) {
			textcolor(11);
			cout << "...";
			break;
		}
	}
	while (j < pos.size() && pos[j] < p.size())
		j++;
	textcolor(15); cout << "\n";
	int dem = 0, max = 0, ind = pos.size(), m = 0;
	for (int i = j; i < pos.size(); i++) {
		int k = std::lower_bound(pos.begin(), pos.end(), pos[i] + 100) - pos.begin();
		if (max < k - i + 1) {
			max = k - i + 1;
			ind = i;
		}
	}
	for (int i = j; i < pos.size(); i++)
		pos[i] -= p.size();
	p.clear();
	while (!infile.eof()) {
		infile >> s;
		sa = s;
		removeSpectialCharData(sa);
		if (sa != "")
			p.push_back(s);
	}
	int index = (ind < pos.size() ? min(pos[ind] - 1, p.size() - 100) : 100);
	gotoXY(x, y + 2);
	cnt = 0;
	cout << "... ";
	while (dem < 100 && ind < pos.size()) {
		if (pos[ind] == dem + index)
			textcolor(14), ind++;
		else
			textcolor(15);
		cout << p[dem + index] << " ";
		textcolor(15);
		dem++; cnt += p[dem + index].size();
		if (cnt > 100) {
			m++;
			if (m == 4)
				break;
			gotoXY(x, y + 2 + m);
			cnt = 0;
		}
	}
	while (m < 4 && dem + index < p.size()) {
		cout << p[dem + index] << " ";
		dem++; cnt += p[dem + index].size();
		textcolor(15);
		if (cnt > 100) {
			m++;
			if (m == 4)
				break;
			gotoXY(x, y + 2 + m);
			cnt = 0;
		}
	}
	cout << " ...";
	/*int m = 0;
	while (!infile.eof() && dem < 100 && j < pos.size()) {
		infile >> s;
		string ss = s;
		removeSpectialCharData(ss);
		if (ss == "")
			continue;
		if (pos[j] == dem + index + p.size())
			textcolor(14), j++;
		else
			textcolor(15);

		cout << s << " ";
		dem++; cnt += s.size();
		textcolor(15);
		if (cnt > 100) {
			m++;
			gotoXY(x, y + 2 + m);
			cnt = 0;
			if (m == 4)
				break;
		}
	}
	while (m < 4 && !infile.eof()) {
		infile >> s;
		cout << s << " ";
		dem++; cnt += s.size();
		textcolor(15);
		if (cnt > 100) {
			m++;
			gotoXY(x, y + 2 + m);
			cnt = 0;
			if (m == 4)
				break;
		}
	}
	/*while (!infile.eof())
	{
		getline(infile, s);
		dem++;
		int dem1 = s.length();
		if (dem < 4)
		{
			if (dem1 < 100) {
				gotoXY(x, y + 2 + dem); cout << s << "..." << endl;
			}
			else
			{
				string s1 = s.substr(0, 100);
				gotoXY(x, y + 2 + dem); cout << s1 << "..." << endl;
			}
		}

	}*/
	infile.close();
}

void New::printScreenFull() {
	Sleep(delay);
	system("cls");
	ifstream infile("News//" + name);

	if (!infile.good())return;

	string s;
	getline(infile, s);

	cout << "   " << New::name << "\n";
	cout << "   " << s << "\n\n";

	while (!infile.eof())
	{
		getline(infile, s);
		xuongdong(s);
	}
	infile.close();
}
void New::xuongdong(string s)
{
	int dem = 1;
	for (int j = 0; j < s.length(); j++)
	{
		if (s[j] == ' ') dem = dem + 1;
		if (dem == 20)
		{
			cout << "   " << s.substr(0, j) << endl;
			s = s.substr(j + 1, s.length());
			xuongdong(s);
		}
		if ((dem < 20) && (j == s.length() - 1)) cout << "   " << s << endl << endl;
	}
}


Database::Database() {
	//vector<string> tmp = importStopWord();
	//stopwords = new Trie;
	//stopwords->add(tmp);
	content = new Trie();
	title = new Trie();
	fileList = new Trie();
	history = importHistory();
	Import();
}

void Database::deallo() {
	for (int i = 0; i < (int)News.size(); ++i)
		News[i].deallo();
	News.clear();
	history.clear();
	fileList->deallo();
}

void Database::Import() {
	ifstream fin;
	vector<string> files;
	getdir(files);
	string Content;
	int i, j; Trie* tmp;
	for (i = 2; i < (int)files.size(); i++) {

		fin.open(fileNametoPath(files[i]));
		if (fin.is_open()) {
			News.push_back(New(files[i]));

			getline(fin, Content);
			title->add(splitWord(Content), News.size() - 1);

			j = splitWord(Content).size();
			while (fin >> Content) {
				removeSpectialCharData(Content);
				if (Content != "") content->add(Content, News.size() - 1, j++, tmp);
			}
		}
		fin.close();
	}
}

void Database::exportHistory() {
	ofstream fout;
	fout.open("data//history.txt");
	for (int i = 0; i < (int)history.size(); i++)
	{
		fout << history[i] << endl;
	}
	fout.close();
}

void Database::addHistory(const string&s) {
	if (std::find(history.begin(), history.end(), s) == history.end()) {
		history.insert(history.begin(), s);
		this->exportHistory();
	}
}

void  Database::updateDb()
{
	ifstream fin;
	vector<string> files;
	getdir_update(files);
	string Content = "";
	int i, j; Trie* tmp;
	for (i = 2; i < (int)files.size(); i++) {
		fin.open("import//"+files[i]);
		if (fin.is_open()) {
			News.push_back(New(files[i]));
			getline(fin, Content);
			title->add(splitWord(Content), News.size() - 1);
			j = splitWord(Content).size();
			while (fin >> Content) {
				removeSpectialCharData(Content);
				if (Content != "") content->add(Content, News.size() - 1, j++, tmp);
			}
		}
		fin.close();
		string s = "import/" + files[i];
		string d = "news/" + files[i];
		const char * SRC = s.c_str();
		const char * DEST = d.c_str();
		bool res1 = copyFile(SRC, DEST), res2 = remove(SRC);
	}
}
