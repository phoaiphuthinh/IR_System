#include "database.h"
#include <iostream>
#include "helper.h"
#include "Trie.h"
#include "query.h"
#include "operator.h"
#include <fstream>
#include "query.h"
#include <algorithm>
#include "screen.h"
#include "microtimer.h"

int main() {
	screen a;
	a.menu();
}

/*int main() {
	microtimer microtime;
	microtimer::size_type start, end;
	
	start = microtime();

	Database db;

	end = microtime();
}*/


















void hamilton(int i, int n, int u, int cost, int& best, bool* free, int** arr) {
	if (i == n) {
		if (u == 1 && best > cost)
			best = cost;
		return;
	}
	for (int v = 0; v < n; ++v)
		if (arr[u][v] > 0 && free[v]) {
			free[v] = false;
			hamilton(i + 1, n, v, cost + arr[u][v], best, free, arr);
			free[v] = true;
		}
}

















