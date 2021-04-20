#include "screen.h"
#include "query.h"

screen::screen() {
	// full screen
	system("mode con COLS=700");
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
	/*for (int i = 0; i < 256; i++) {
		textcolor(i);
		cout << i << "\n";
	}
	system("pause");*/
}


void screen::drawSearch() {
	Sleep(delay);
	system("cls");
	//TEXT COLOR CARO
	system("COLOR E");


	gotoXY(23, 1);
	std::cout << "------------------------------------------------------------------------------------";
	gotoXY(23, 3);
	std::cout << "     HHHH        HHHH     KKKK      KKKK     TTTTTTTTTTTTTTTTT     DDDDDDDDD         \n";
	gotoXY(23, 4);
	std::cout << "     HHHH        HHHH     KKKK    KKKK       TTTTTTTTTTTTTTTTT     DDDD  DDDDD                 \n";
	gotoXY(23, 5);
	std::cout << "     HHHH        HHHH     KKKK   KKKK              TTTT            DDDD    DDDD  \n";
	gotoXY(23, 6);
	std::cout << "     HHHH        HHHH     KKKK KKKK                TTTT            DDDD      DDDD\n";
	gotoXY(23, 7);
	std::cout << "     HHHHHHHHHHHHHHHH     KKKKKKKKK                TTTT            DDDD       DDDD        \n";
	gotoXY(23, 8);
	std::cout << "     HHHHHHHHHHHHHHHH     KKKK KKKK                TTTT            DDDD       DDDD\n";
	gotoXY(23, 9);
	std::cout << "     HHHH        HHHH     KKKK  KKKK               TTTT            DDDD      DDDD   \n";
	gotoXY(23, 10);
	std::cout << "     HHHH        HHHH     KKKK    KKKK             TTTT            DDDD     DDDD                 \n";
	gotoXY(23, 11);
	std::cout << "     HHHH        HHHH     KKKK     KKKK            TTTT            DDDD  DDDDD         \n";
	gotoXY(23, 12);
	std::cout << "     HHHH        HHHH     KKKK      KKKK           TTTT            DDDDDDDDD                \n";
	gotoXY(23, 14);
	std::cout << "------------------------------------------------------------------------------------";
}

void screen::searchScreen() {
	while (1) {
		drawSearch();

		_query a;
		if (!a.input()) break;

		vector<std::pair<int, vector<int>>> result = a.search(db);
		showResult(result, db);

		//string s;
		//vector<string> suggest = suggestion(s, db);
	}
}

screen::~screen()
{

}

//void screen::showResult(std::vector<int> index, std::vector<std::vector<int>>& b, Database& db)
/* void screen::showResult(std::vector<int>& index, Database& db) {
	//a[i]: tên file i
	//b[i][j]: j là thứ in đậm trong file i
		system("cls");

		for (int i = 0; i < index.size(); ++i)
		{
			std::cout << "----------------------------------------------------------------------------" << endl;
			db.News[index[i]].printScreen();
			std::cout << "----------------------------------------------------------------------------" << endl;
		}
		system("pause");
} */

void screen::drawResult(vector<std::pair<int, vector<int>>>& data) {
	Sleep(delay);
	system("cls");
	textcolor(15);

	if (data.size() == 0) {
		return;
	}

	gotoXY(10, 0); std::cout << "----------------------------------------------------------------------------" << endl;
	for (int i = 0; i < min(5,data.size()); ++i)
	{
		//gotoXY(10, i * 8 + 1); std::cout << "----------------------------------------------------------------------------" << endl;
		db.News[data[i].first].printScreen(10, i * 7+1, data[i].second);
		gotoXY(10, i * 7 + 7); std::cout << "----------------------------------------------------------------------------" << endl;
	}
	//gotoXY(10, (data.size() - 1) * 7 + 7); std::cout << "----------------------------------------------------------------------------" << endl;
}

void screen::showResult(vector<std::pair<int, vector<int>>>& data, Database& db){
	//resizeConsole(1200, 1200);
	int first, last, u, v;
	first = 0; last = min(5,data.size()); u = 0, v = 0;
	cursor(false);
	while (1) {
		drawResult(data);

		if (data.size() == 0) {
			cout << "Sorry nothing can be found\n\n";
			while (!(getKey(VK_ESCAPE) || getKey(VK_RETURN))) {}
			return;
		}

		int choose = ChoosingMenu(u, v, first, last);

		if (choose == -1) return;
		db.News[data[choose].first].printScreenFull();
		while (!(getKey(VK_ESCAPE) || getKey(VK_RETURN))) {}
	}
}

int idY(int v) {
	return v * 7 + 2;
}

int screen::ChoosingMenu(const int &x, int &v, const int &firstline, const int &lastline) {
	while (1) {
		gotoXY(x, idY(v));
		cout << "->";

		if (getKey(VK_DOWN) || getKey('s') || getKey('S')) {
			gotoXY(x, idY(v));
			cout << "  ";
			if (v == lastline - 1) v = 0;
			else ++v;
			Sleep(delay);
		}

		if (getKey(VK_UP) || getKey('w') || getKey('W')) {
			gotoXY(x, idY(v));
			cout << "  ";
			if (v == firstline) v = lastline - 1;
			else --v;
			Sleep(delay);
		}

		if (getKey(VK_RETURN)) return v;

		if (getKey(VK_ESCAPE)) return -1;
	}
}

void screen::updateDatabase()
{
	db.updateDb();
}

void screen::menu() {
	drawMenu();
	int cur = 0, ma = 4, u = 28, v = 17, d = 2;
	while (1) {
		if (getKey(VK_DOWN) || getKey('s') || getKey('S')) {
			gotoXY(u, v + cur * d); cout << "  ";
			++cur;
			if (cur == ma) cur = 0;
			Sleep(delay);
		}
		else if (getKey(VK_UP) || getKey('w') || getKey('W')) {
			gotoXY(u, v + cur * d); cout << "  ";
			--cur;
			if (cur == -1) cur = ma-1;
			Sleep(delay);
		}

		else if (getKey(VK_RETURN)) {
			switch (cur) {
			case 0: {
				searchScreen();
				drawMenu();
				break;
			}
			case 1: {
				update();
				drawMenu();
				break;
			}
			case 2: {
				history();
				drawMenu();
				break;
			}
			case 3: {
				return;
			}
			default: {
				break;
			}
			}
		}

		gotoXY(u, v + cur * d); cout << "->";
	}
}
void screen::drawMenu() {
	Sleep(delay);
	cursor(false);
	system("cls");
	gotoXY(30, 15); cout << "Search engine";
	gotoXY(33, 17); cout << "1. Search";
	gotoXY(33, 19); cout << "2. Add data";
	gotoXY(33, 21); cout << "3. View history";
	gotoXY(33, 23); cout << "4. Exit";
}

void screen::update() {
	drawUpdate();
	int cur = 0, ma = 2, u = 24, v = 19, d = 2;
	while (1) {
		if (getKey(VK_DOWN) || getKey('s') || getKey('S')) {
			gotoXY(u, v + cur * d); cout << "  ";
			++cur;
			if (cur == ma) cur = 0;
			Sleep(delay);
		}
		else if (getKey(VK_UP) || getKey('w') || getKey('W')) {
			gotoXY(u, v + cur * d); cout << "  ";
			--cur;
			if (cur == -1) cur = ma - 1;
			Sleep(delay);
		}

		else if (getKey(VK_RETURN)) {
			switch (cur) {
			case 0: {
				db.updateDb();
				return;
				break;
			}
			case 1: {
				return;
			}
			default: {
				break;
			}
			}
		}

		gotoXY(u, v + cur * d); cout << "->";
	}
}

void screen::drawUpdate() {
	Sleep(delay);
	cursor(false);
	system("cls");
	gotoXY(20, 15); cout << "You have to create folder name 'Import' in the same direct of code and put data file in there";
	gotoXY(25, 17); cout << "Do you want to import: ";
	gotoXY(27, 19); cout << "1. Yes";
	gotoXY(27, 21); cout << "2. No";
}

void screen::history() {
	int cur = 0, ma = (db.history.size() + 4) / 5, page = 0;
	drawHistory(page);
	while (1) {
		if (getKey(VK_RIGHT) || getKey('D') || getKey('d')) {
			++page;
			if (page >= ma) page = ma - 1;
			else drawHistory(page);
			Sleep(delay);
		}
		else if (getKey(VK_LEFT) || getKey('a') || getKey('A')) {
			--page;
			if (page < 0) page = 0;
			else drawHistory(page);
			Sleep(delay);
		}
		else if (getKey(VK_RETURN) || getKey(VK_ESCAPE)) {
			return;
		}
	}
}

void screen::drawHistory(int page) {
	Sleep(delay);
	cursor(false);
	system("cls");
	int u = 24, v = 15, d = 2;
	for (int i = 0; i < 5; ++i) {
		gotoXY(u, v + d * i); cout << page * 5 + i + 1 << ". " << db.history[page * 5 + i];
	}
	gotoXY(u + 2, v + d * 5); cout << "-> Exit";
	gotoXY(u + 15, v + d * 5); cout << "Page " << page << "/" << (db.history.size() + 4) / 5;
}