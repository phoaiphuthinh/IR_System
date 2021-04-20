#include "helper.h"

using std::ifstream;
vector<string> words = vector<string>();

int getdir(vector<string>& files) {
	string dir = string("\News");
	DIR* dp;
	struct dirent* dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		//cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		files.push_back(string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}

int getdir_update(vector<string>& files) {
	string dir = string("\import");
	DIR* dp;
	struct dirent* dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		//cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		files.push_back(string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}

void createFilesList() {
	string dir = string("//News");
	vector<string> files = vector<string>();

	getdir(files);

	ofstream fout;
	fout.open("News//listOfFile.txt");

	for (int i = 2; i < (int)files.size(); i++) {
		fout << files[i] << endl;
	}
	fout.close();
}

string fileNametoPath(string& filename) {
	return "News//" + filename;
}

vector <string> importStopWord() {
	vector <string> StopWords;
	ifstream fin;
	fin.open("data//stopwords.txt");
	if (fin.is_open())
	{
		while (!fin.eof())
		{
			string dataline;
			getline(fin, dataline);
			StopWords.push_back(dataline);
		}
	}
	fin.close();
	return StopWords;
}

vector <string> importHistory() {
	vector <string> history;
	ifstream fin;
	fin.open("data//history.txt");
	if (fin.is_open())
	{
		while (!fin.eof())
		{
			string dataline;
			getline(fin, dataline);
			history.push_back(dataline);
		}
	}
	fin.close();
	return history;
}

/*
string default_words()
{
	if (words != "") return words;
	ifstream fin;
	fin.open("data/stopwords.txt");
	if (fin.is_open())
	{
		while (!fin.eof())
		{
			string dataline;
			getline(fin, dataline);
			words += dataline + " ";
		}
		//cout << words;
	}
	fin.close();
	return words;
}

std::set<string> uqwords(const string &str)
{
	std::istringstream source{ str };
	return { std::istream_iterator<string>{source},
			std::istream_iterator<string>{} };
}

stopwords::stopwords()
{
	string inWords = default_words();
	uwords = uqwords(inWords);
}

stopwords::stopwords(const string inWords)
{
	uwords = uqwords(inWords);
}

void stopwords::remove_stopwords(string & str)
{
	size_t pos = string::npos;
	for (auto it = uwords.begin(); it != uwords.end(); it++)
	{
		if (str.length() >= it->length() + 1)
		{
			while (((pos = str.find(" " + *it + " ")) != std::string::npos) && (str.length() >= it->length() + 1))
			{
				str.erase(pos, it->length() + 1);
			}

			if (str.find(*it + " ") == 0)
			{
				str.erase(0, it->length());
			}
			if (str.find(" " + *it) == (str.length() - it->length() - 1))
			{
				str.erase(str.length() - it->length() - 1, it->length());
			}
		}
	}
}

*/

int id(const char& c) {
	if ('a' <= c && c <= 'z') return int(c - 'a');
	if ('0' <= c && c <= '9') return 26 + int(c - '0');
	if (c == 39) return 36; // '
	if (c == '$') return 37;
	if (c == 'A') return 38;
	if (c == 'N') return 39;
	if (c == 'D') return 40;
	if (c == 'O') return 41;
	if (c == 'R') return 42;
	return 43; // :
}

vector<string> splitWord(const string& s, int flag) {
	vector<string> tokens;
	string token;
	std::istringstream tokenStream(s);
	//while (std::getline(tokenStream, token, delimiter))
	while (tokenStream >> token)
	{
		if (flag == 2) 
			removeSpectialCharData(token, false);
		else if (flag == 1)
			removeSpectialCharQuery(token);
		else removeSpectialCharData(token);
		if (token != "") tokens.push_back(token);
	}
	return tokens;
}

vector<std::string> splitWord_DeleteStopwords(Trie& stopwords, const std::string& s) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (tokenStream >> token)
	{
		removeSpectialCharData(token);
		if (!isStopword(stopwords, token))
			tokens.push_back(token);
	}
	return tokens;
}

bool isStopword(Trie& stopword, const string& key) {
	//	if (std::find(stopword.begin(), stopword.end(), key) != stopword.end()) {
	return stopword.find(key);
}

void removeSpectialCharQuery(string& x) {
	if (x == "AND") return;
	if (x == "OR") return;
	if (x == "intitle:") return;
	if (x == "filetype:") return;
	if (x == "*") return;
	if (x != "" && x[0] == '$') {
		bool check = 1;
		for (int i = 1; i < (int)x.size(); ++i)
			if (!('0' <= x[i] && x[i] <= '9')) {
				check = 0;
				break;
			}
		if (check) return;
	}
	if (x !=  "" && (x[0] == '+' || x[0] == '-' || x[0] == '\"')) {
		char c = x[0];
		removeSpectialCharData(x);
		x = c + x;
		return;
	}
	if (x != "" && x.back() == '\"') {
		char c = x.back();
		removeSpectialCharData(x);
		x = x + c;
		return;
	}
	removeSpectialCharData(x);
}

void removeSpectialCharData(string& x, bool flag) {
	for (int i = 0; i < (int)x.size(); ++i) {
		if (!((x[i] >= 'a' && x[i] <= 'z') || (x[i] >= 'A' && x[i] <= 'Z') || x[i] == 39 ||  x[i] == '$' || (x[i] >= '0' && x[i] <= '9'))) {
			x.erase(i--, 1);
		}
		else {
			if (flag && x[i] >= 'A' && x[i] <= 'Z') x[i] = int(x[i]) - (int('Z') - int('z'));
		}
	}
}

int BS(const std::vector<int>& a, const int& x) {
	int l = 0, r = a.size() - 1, m = 0;
	while (l <= r) {
		m = (l + r) / 2;
		if (a[m] == x) return m;
		if (a[m] < x) l = m + 1;
		else r = m - 1;
	}
	return -1;
}

int BS(const vector<std::pair<int, vector<int>>>& a, const int& x) {
	int l = 0, r = a.size() - 1, m = 0;
	while (l <= r) {
		m = (l + r) / 2;
		if (a[m].first == x) return m;
		if (a[m].first < x) l = m + 1;
		else r = m - 1;
	}
	return -1;
}

bool copyFile(const char *SRC, const char* DEST)
{
	std::ifstream src(SRC, std::ios::binary);
	std::ofstream dest(DEST, std::ios::binary);
	dest << src.rdbuf();
	return src && dest;
}

string removeDot(string s)
{
	for (int i = 0; i < (int)s.length(); ++i) {
		if (s[i] == '.') s[i] = ':';
	}
	return s;
}

void ClearConsoleInputBuffer() {
	// If you happen to have any trouble clearing already cleared buffer, uncomment the section below.
	/* keybd_event('S', 0, 0, 0);
	keybd_event('S', 0,KEYEVENTF_KEYUP, 0);
	keybd_event(VK_BACK, 0, 0, 0);
	keybd_event(VK_BACK, 0,KEYEVENTF_KEYUP, 0); */
	PINPUT_RECORD ClearingVar1 = new INPUT_RECORD[256];
	DWORD ClearingVar2;
	ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), ClearingVar1, 256, &ClearingVar2);
	delete[] ClearingVar1;
}

void gotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void textcolor(int x) {
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}

bool getKey(int x) {
	return (GetAsyncKeyState(x) & 0x8000);
}

void cursor(bool flag) {
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = flag;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);

}