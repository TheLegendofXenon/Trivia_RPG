#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>

using std::string;
using std::vector;
using std::pair;
using std::ifstream;
using std::cout;
using std::cin;
using std::endl;

class Enemy {
protected: // Allows access for subclasses
	string name;
	vector<int> stats; // 6 values will be used.
	pair<string, int> item;
	static int EXP;
public:
	// Enemy's member functions	
	string get_name(); // DONE.
	pair<string, int> get_item(); // DONE.
	static int get_EXP(); // DONE.
	virtual void random_choice(vector<int>&, int&, string) = 0; // Pure virtual function
	void attack(int&, int, string); // QA.
	void use_item(); // QA.
	int die(); // DONE.
	int& operator[](int index); // DONE.
};

#endif
