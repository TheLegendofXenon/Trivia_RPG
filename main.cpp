/*
*	Author: T. Vo
*	Description: You are a character who is playing through an entire RPG dungeon. 
*	This entire RPG adventure is all text-based (it’s up to the user to fill in the 
*	gaps with their imagination). This dungeon is crawling with various enemies and 
*	one final boss. However, you will gain EXP, collect items, and use OOP and trivia 
*	knowledge to help you along the way. The game will end when you die or you defeat 
*	the final boss.
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "Avatar.h"
#include "Enemies.h"
#include "Boss.h"

using namespace std;

void read_file(const string& text_name);
int valid_choice(char check_char, char chr1, char chr2);

int main()
{
	// All declarations
	Avatar user;
	string name;
	char name_choice = ' ', adventure_choice = ' ';
	int valid = 0, level_check = 1, death_check = 0;

	// Opens "Intro.txt" and reads each line of data.
	read_file("Intro.txt");

	// State default name and ask for alternative.
	cout << "Your default name is 'Xenon'!" << endl;
	while (!valid) {
		cout << "Would you prefer a different name? (Y/N): ";
		cin >> name_choice;
		name_choice = toupper(name_choice); cin.ignore(1000, '\n');
		valid = valid_choice(name_choice, 'Y', 'N');
	}
	if (name_choice == 'Y') {
		cout << "Name: ";
		cin >> name; 
		cin.ignore(1000, '\n');
		user = Avatar(name); // New name;
	}
	else
		user = Avatar(); // Default name;
	Boss ragnarok = Boss();

	// Basic Tutorial
	cout << endl << "Alright " << user.get_name() << ", use your skills and knowledge to " 
		<< "reach Level 10 ASAP!" << endl;
	read_file("Tutorial.txt");

	// User can use Menu or Explore until they die or hit level 10.
	// level_check = user.get_level(); // Test out boss fight.
	valid = 0;
	while (level_check < 10 && death_check == 0) {
		while (!valid) {
			cout << "Look in MENU or go EXPLORE? (M/E): ";
			cin >> adventure_choice;
			adventure_choice = toupper(adventure_choice); cin.ignore(1000, '\n');
			valid = valid_choice(adventure_choice, 'M', 'E'); 
		}
		if (adventure_choice == 'M')
			user.menu(user, ragnarok);
		else
			user.explore(user);
		
		valid = 0;
		level_check = user.get_level();
		death_check = user.die();
		if (death_check)
			cout << "You Died..." << endl << endl;
	}

	// Fight Ragnarok when level = 10. You can either die or win to read
	// in the "End_Credits.txt" file.
	if (level_check == 10) { // Edit algorithm
		// Last time in menu for user. Also creates boss and engages fight.
		cout << "You feel something wicked coming. "
			<< "Please take a look at your menu..." << endl;
		user.menu(user, ragnarok);

		// Situations where user or boss dies
		death_check = user.die();
		if (death_check)
			cout << "You Died..." << endl << endl;
		if(ragnarok.die())
			read_file("End_Credits.txt");
	}

	return 0;
}

// This function reads in any text file regularly.
void read_file(const string& text_name) {
	ifstream file(text_name);
	if (file) {
		string text_line;
		while (getline(file, text_line))
			cout << text_line << endl;
	}
	file.close();
}

// This function checks to see whether an entered character matches either of 
// two required characters.
int valid_choice(char check_char, char chr1, char chr2) {
	try {
		if (check_char == chr1 || check_char == chr2)
			return 1;
		else {
			cin.clear();
			throw invalid_argument("That isn't a valid character. Please try again... ");
		}
	}
	catch (exception &c) {
		cout << c.what() << endl << endl;
		return 0;
	}
}



