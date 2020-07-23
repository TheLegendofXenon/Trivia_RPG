#ifndef AVATAR_H
#define AVATAR_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "Enemies.h"
#include "Boss.h"

using std::string;
using std::vector;
using std::map;
using std::invalid_argument;
using std::exception;
using std::ofstream;
using std::cout;
using std::cin;
using std::endl;

class Avatar {
	string name; // Name of the avatar
	vector<int> stats; // 7 values will be used
	int temp_defense; // Will be used for defend() function.
	int level; // Current level of the avatar
	vector<pair<string, int>> equipment; // 3 pairs will be used
	vector<pair<string, int>> items; // 5 pairs will be used
public:
	Avatar(string = "Xenon"); 

	// Avatar's member functions
	string get_name(); // DONE.
	int get_level(); // DONE.
	void action(Enemy&, int&); // DONE.
	void attack(Enemy&); // QA
	void questions(Enemy&); // QA.
	void use_item(); 
	int flee(int); // QA.
	void defend(); // QA.
	void menu(Avatar&, Boss& r);
	void explore(Avatar&);
	int die(); // DONE.
	void level_up(Enemy&); // QA
	void equip(string); // QA
	int print_items(); // DONE.
	void drop_item(); // DONE.
	void pick_up(string, int); // QA
	int valid_choice(char, char, char); // DONE.
	friend void fight(Avatar&, Enemy&); // Defined in Enemy.cpp
};
#endif
