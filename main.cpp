#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <iterator>
#include "utilities.h"



using namespace std;




vector < pair<string, string>> productions;

/* given a symbol  "symbol"
	returns a set of all symbols
	that can produce it
	*/
set<string> search(string symbol) {

	set<string> x;

	for (int i = 0; i < productions.size(); i++) {
		if (productions[i].second == symbol)
			x.insert(productions[i].first);
	}

	return x;
}
/* generalization of the above
   returns a set of symbols that
   can produce a set of symbols
   */
set<string> search(set<string> ss) {
	set<string> x;
	set <string> ::iterator itr;
	for (itr = ss.begin(); itr != ss.end(); ++itr)
	{
		set<string> y = search(*itr);
		x = merge(x, y);
	}

	return x;
}


/* this is the heart of
   the CYK algorithm
   it takes an argument a matrix m
   and string imput and "fills"
   the solution
   */
void solve(Matrix<set<string>>& m, string input) {
	string symbol;
	set<string> result;
	set<string> s1;
	set<string> s2;
	int c1, c2;

	for (int j = 0; j < m.getCols(); j++) {
		for (int i = 0; i < m.getRows() - j; i++) {
			if (j == 0) {
				symbol = input[i];
				m[i][j] = search(symbol);
			}

			else {
				c1 = j;
				c2 = 1;

				while (c2 <= j) {
					s1 = search(cartesian(m[i][j - c1], m[i + c2][j - c2]));
					m[i][j] = merge(s1, m[i][j]);
					c1--;
					c2++;

				}
			}
		}
	}
}


int main() {
	cout << "input a string\n";
	string input;
	cin >> input;
	/* Assumes that the starting
		symbol is "S"
		*/
	Matrix<set<string>> table(input.length(), input.length());
	std::ifstream file;
	/* read the cfg from file "cfg.txt"
	   if the grammar is CNF*/
	file.open("cfg2.txt");
	int np;
	std::string pr;
	std::string left, right;
	/* read the file and save the productions
		in a vector of pairs. For example
		S->AB is saved as pair("S","AB");
		*/
	while (file >> pr) {
		if (pr.length() > 5) {
			std::cout << "not cnf\n";
			abort();
		}
		int itr = pr.find("->");
		if (itr != 1) {
			std::cout << "more than one symbol on left\n";
			abort();
		}
		left = pr.substr(0, 1);
		right = pr.substr(itr + 2, 2);
		pair<string, string> p(left, right);
		productions.push_back(p);
	}

	solve(table, input);

	file.close();
	/* the startig symbol is assumed
		to be S
		*/
	int j = input.length() - 1;
	if (table[0][j].find("S") != table[0][j].end()) {
		std::cout << "belongs to language\n";
	}
	else
		std::cout << "does not belong to language\n";

	productions;

}
