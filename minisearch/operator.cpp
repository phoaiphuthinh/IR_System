#include "operator.h"
#include <queue>

struct cmp {
	bool operator()(std::pair<int, vector<int>*>& const A, std::pair<int, vector<int>*>& const B) {
		return A.first > B.first;
	}

	bool operator()(std::pair<std::pair<int, vector<int>>, int>& const A, std::pair<std::pair<int, vector<int>>, int>& const B) {
		return A.second > B.second;
	}
};

vector<std::pair<int, vector<int>>> defSearch(vector<string>& s, Database& data) {
	std::vector<Trie*> q; q.clear();
	for (int i = 0; i < (int)s.size(); ++i) {
		q.push_back(data.content->Find(s[i]));
		q.push_back(data.title->Find(s[i]));
	}

	vector<std::pair<int, vector<int>*>> v;

	for (int i = 0; i < (int)q.size(); ++i)
		for (int j = 0; q[i] != NULL && j < (int)q[i]->indexNew.size(); ++j)
			v.push_back(std::make_pair(q[i]->indexNew[j], &(q[i]->indexWord[j])));

	sort(v.begin(), v.end(), cmp());

	vector <std::pair< std::pair<int, vector<int>>, int>> result;

	for (int i = 0; i < (int)v.size(); ++i) {
		if (i == 0 || v[i].first != v[i - 1].first) result.push_back(make_pair(make_pair(v[i].first, vector<int>{}), 0));
		result[result.size() - 1].first.second.insert(result[result.size() - 1].first.second.end(), v[i].second->begin(), v[i].second->end());
		++result[result.size() - 1].second;
	}

	sort(result.begin(), result.end(), cmp());

	vector<std::pair<int, vector<int>>> final;

	for (int i = 0; i < (int)result.size(); ++i)
		final.push_back(result[i].first);

	return final;
}


vector<pair<int, vector<int>>> exSearch(vector<string>& s, vector<string>& ex, Database& data) {
	vector<std::pair<int, vector<int>>> f = defSearch(s, data);
	vector<std::pair<int, vector<int>>> res;
	set<int> ss;
	for (auto j : ex) {
		Trie* tmp = data.content->Find(j);
		if (tmp)
		for (auto i : tmp->indexNew)
			ss.insert(i);
		tmp = data.title->Find(j);
		if (tmp)
		for (auto i : tmp->indexNew)
			ss.insert(i);
	}
	for (int i = 0; i < (int)f.size(); i++)
		if (ss.find(f[i].first) == ss.end())
			res.push_back(f[i]);
	return res;
}

vector<pair<int, vector<int>>> inSearch(vector<string>& s, vector<string>& in, Database& data) {
	s.insert(s.end(), in.begin(), in.end());
	vector<std::pair<int, vector<int>>> f = defSearch(s, data);
	vector<std::pair<int, vector<int>>> res;
	set<int> ss;
	for (auto j : in) {
		Trie* tmp = data.content->Find(j);
		if (tmp)
		for (auto i : tmp->indexNew)
			ss.insert(i);
		tmp = data.title->Find(j);
		if (tmp)
		for (auto i : tmp->indexNew)
			ss.insert(i);
	}
	for (auto it : f)
		if (ss.find(it.first) != ss.end())
			res.push_back(it);
	return res;
}

vector<pair<int, vector<int>>> ANDSearch(vector<string>& s1, vector<string>& s2, Database& data) {
	vector<std::pair<int, vector<int>>> f1 = defSearch(s1, data);
	vector<std::pair<int, vector<int>>> f2 = defSearch(s2, data);
	s1.insert(s1.end(), s2.begin(), s2.end());
	vector<std::pair<int, vector<int>>> f = defSearch(s1, data);
	vector<std::pair<int, vector<int>>> res, ans;
	set<int> ss;
	for (auto it : f1)
		ss.insert(it.first);
	for (auto it : f2)
		if (ss.find(it.first) != ss.end())
			res.push_back(it);
	ss.clear();
	for (auto it : res)
		ss.insert(it.first);
	for (auto it : f)
		if (ss.find(it.first) != ss.end())
			ans.push_back(it);
	return ans;
}

vector<pair<int, vector<int>>> ORSearch(vector<string>& s1, vector<string>& s2, Database& data) {
	vector<std::pair<int, vector<int>>> f1 = defSearch(s1, data);
	vector<std::pair<int, vector<int>>> f2 = defSearch(s2, data);
	s1.insert(s1.end(), s2.begin(), s2.end());
	vector<std::pair<int, vector<int>>> f = defSearch(s1, data);
	vector<std::pair<int, vector<int>>> res;
	set<int> ss;
	for (auto it : f) {
		ss.insert(it.first);
		res.push_back(it);
	}
	for (auto it : f1)
		if (ss.find(it.first) == ss.end()) {
			ss.insert(it.first);
			res.push_back(it);
		}
	for (auto it : f2)
		if (ss.find(it.first) == ss.end())
			res.push_back(it);
	return res;
}

vector<pair<int, vector<int>>> exactSearch(vector<string>& s, Database& data) {
	vector<std::pair<int, vector<int>>> f = defSearch(s, data);
	vector<std::pair<int, vector<int>>> res;
	map<int, vector< pair<int, int> > > content, title;
	for (int j = 0; j < (int)s.size(); j++) {
		Trie* tmp = data.content->Find(s[j]);
		if (tmp)
		for (int i = 0; i < (int)tmp->indexNew.size(); i++)
			for (auto g : tmp->indexWord[i])
				content[tmp->indexNew[i]].push_back({ g , j });
		tmp = data.title->Find(s[j]);
		if (tmp)
		for (int i = 0; i < (int)tmp->indexNew.size(); i++)
			for (auto g : tmp->indexWord[i])
				title[tmp->indexNew[i]].push_back({ g,j });
	}
	for (auto it : f) {
		int g = it.first;
		sort(content[g].begin(), content[g].end());
		sort(title[g].begin(), title[g].end());
		int p = 1;
		bool ok = (p == s.size() && content[g].size() + title[g].size() > 0);
		for (int j = 1; j < (int)title[g].size(); j++)
			if (title[g][j].first == title[g][j - 1].first + 1 && title[g][j].second == title[g][j - 1].second + 1)
				ok |= (++p == s.size());
			else
				p = 1;
		p = 1;
		for (int j = 1; j < (int)content[g].size(); j++)
			if (content[g][j].first == content[g][j - 1].first + 1 && content[g][j].second == content[g][j - 1].second + 1)
				ok |= (++p == s.size());
			else
				p = 1;
		if (ok)
			res.push_back(it);
	}
	return res;
}


vector<pair<int, vector<int>>> unkSearch(vector<string>& s, Database& data) {
	vector<std::pair<int, vector<int>>> f = defSearch(s, data);
	vector<std::pair<int, vector<int>>> res;
	map<int, vector< pair<int, int> > > content, title;
	for (int j = 0; j < (int)s.size(); j++) 
		if (s[j] != "*") {
			Trie* tmp = data.content->Find(s[j]);
			if (tmp)
			for (int i = 0; i < (int)tmp->indexNew.size(); i++)
				for (auto g : tmp->indexWord[i])
					content[tmp->indexNew[i]].push_back({ g , j });
			tmp = data.title->Find(s[j]);
			if (tmp)
			for (int i = 0; i < (int)tmp->indexNew.size(); i++)
				for (auto g : tmp->indexWord[i])
					title[tmp->indexNew[i]].push_back({ g , j });
		}
	for (auto it : f) {
		int g = it.first;
		sort(title[g].begin(), title[g].end());
		sort(content[g].begin(), content[g].end());
		int p = 1;
		bool ok = (p == s.size() && title[g].size() + content[g].size() > 0);
		for (int j = 1; j < (int)title[g].size(); j++)
			if (title[g][j].first == title[g][j - 1].first + 1 && title[g][j].second == title[g][j - 1].second + 1)
				ok |= (++p == s.size());
			else if (title[g][j].first == title[g][j - 1].first + 2
				&& title[g][j].second == title[g][j - 1].second + 2 && s[title[g][j - 1].second + 1] == "*") {
				p += 2;
				ok |= (p == s.size());
			} else 
				p = 1;
		p = 1;
		for (int j = 1; j < (int)content[g].size(); j++)
			if (content[g][j].first == content[g][j - 1].first + 1 && content[g][j].second == content[g][j - 1].second + 1)
				ok |= (++p == s.size());
			else if (content[g][j].first == content[g][j - 1].first + 2
				&& content[g][j].second == content[g][j - 1].second + 2 && s[content[g][j - 1].second + 1] == "*") {
				p += 2;
				ok |= (p == s.size());
			}
			else
				p = 1;
		if (ok)
			res.push_back(it);
	}
	return res;
}

vector<std::pair<int, vector<int>>> typeSearch(string& s, Database& db)
{
	vector<std::pair<int, vector<int>>> final;

	if (s == "txt")
	{

		for (int i = 0; i < 100; ++i)
			final.push_back(std::make_pair(i, vector<int>{0}));
	}
	return final;

}

vector<string> suggestion(const string& s, vector<string>& history) {
	std::vector<std::string> first;
	std::vector<std::string> second;
	for (int i = 0; i < (int)history.size(); i++)
	{
		if (history[i].find(s) == 0) first.push_back(history[i]);
		if (s.length() > 1 && history[i].find(s) > 0 && history[i].find(s) < history[i].length()) second.push_back(history[i]);
		if (first.size() > 5) break;
	}
	if (first.size() < 5 && second.size() > 0)
	{
		for (int i = 0; i < (int)second.size(); i++)
		{
			first.push_back(second[i]);
			if (first.size() > 5) break;
		}
	}
	return first;
}

vector<std::pair<int, vector<int>>> titleSearch(vector<string>& s, Database& data) {
	std::vector<Trie*> q; q.clear();
	for (int i = 1; i < (int)s.size(); ++i) {
		q.push_back(data.title->Find(s[i]));
	}
	vector<std::pair<int, vector<int>*>> v;
	for (int i = 0; i < (int)q.size(); ++i)
		for (int j = 0; q[i] != NULL && j < (int)q[i]->indexNew.size(); ++j)
			v.push_back(std::make_pair(q[i]->indexNew[j], &(q[i]->indexWord[j])));
	sort(v.begin(), v.end(), cmp());

	vector <std::pair< std::pair<int, vector<int>>, int>> result;

	for (int i = 0; i < (int)v.size(); ++i) {
		if (i == 0 || v[i].first != v[i - 1].first) result.push_back(make_pair(make_pair(v[i].first, vector<int>{}), 0));
		result[result.size() - 1].first.second.insert(result[result.size() - 1].first.second.end(), v[i].second->begin(), v[i].second->end());
		++result[result.size() - 1].second;
	}

	sort(result.begin(), result.end(), cmp());

	vector<std::pair<int, vector<int>>> final;

	for (int i = 0; i < (int)result.size(); ++i)
		final.push_back(result[i].first);

	return final;
}

vector<std::pair<int, vector<int>>> searchPrice(vector<string>& s, Database& db) {
	vector<string> tmp = { "$" };
	vector<std::pair<int, vector<int>>> v1 = defSearch(s, db);
	vector<std::pair<int, vector<int>>> v2 = defSearch(tmp, db);
	vector<std::pair<int, vector<int>>> result;

	sort(v1.begin(), v1.end());
	for (int i = 0; i < (int)v2.size(); ++i) {
		int m = BS(v1, v2[i].first);
		if (m != -1) result.push_back(v2[i]);
	}

	for (int i = 0; i < (int)v2.size(); ++i) {
		result.push_back(v2[i]);
	}

	return result;
}