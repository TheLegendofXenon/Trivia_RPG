#include "Enemies.h"

Enemies::Enemies(int avatar_lvl) {
	// All Declarations
	string temp_name, eat_string, item_string;
	int HP, critical_HP, attack, defense, grip, exp, items_allowed, 
		rand_item, item_int, rand_hold;
	ifstream enemies_file, item_file;
	// Seeds the random number generator with a value based on time().
	int seconds = time(nullptr);
	srand(seconds);

	// Determines which enemy is read-in from the "Enemies.txt" file
	// based on avatar level
	enemies_file.open("Enemies.txt");
	if (enemies_file) {
		for (int i = 0; i < avatar_lvl && !enemies_file.eof(); ++i) {
			getline(enemies_file, temp_name, '\t');
			enemies_file >> HP;
			enemies_file >> critical_HP;
			enemies_file >> attack;
			enemies_file >> defense;
			enemies_file >> grip;
			enemies_file >> exp;
		}
	}

	// Erases the newline characters in some of the enemies' names.
	if (temp_name.find('\n') != -1)
		temp_name.erase(0, 1);
	name = temp_name;
	stats.push_back(HP); // stats[0] is the MAX HP
	stats.push_back(stats[0]); // stats[1] is the current HP
	stats.push_back(critical_HP); // stats[2] is the critical HP
	stats.push_back(attack); // stats[3] is the attack stat
	stats.push_back(defense); // stats[4] is the defense stat
	stats.push_back(grip); // stats[5] is the grip stat
	EXP = exp;
	
	// Get an item from the "Items.txt" file.
	item_file.open("Items.txt");
	if (item_file) {
		for (int i = 0; i < 17; i++) {
			getline(item_file, eat_string, '\n');
		}

		switch (avatar_lvl) {
			case 1:
			case 2:
				items_allowed = 1; break;
			case 3:
			case 4:
				items_allowed = 2; break;
			case 5:
			case 6:
				items_allowed = 3; break;
			case 7:
			case 8:
			case 9:
				items_allowed = 4;
		}
		

		rand_item = rand() % items_allowed + 1;
		rand_hold = rand() % 2 + 1;

		for (int r = 0; r < rand_item && !enemies_file.eof(); ++r) {
			getline(item_file, item_string, '\t');
			item_file >> item_int;
			if (item_string.find('\n') != -1)
				item_string.erase(item_string.begin());
			item = {item_string, item_int}; // Drop is the healing item.
		}
	}

	enemies_file.close();
	item_file.close();
}

// This function chooses a random set of actions the enemy can perform.
// Use item occurs always when current HP <= critical HP.
void Enemies::random_choice(vector<int>& a_stats, int& temp_defense, string a_name) {
	int rand_choice = rand() % 2 + 1;
	
	// Use item occurs at critical HP and when item hasn't been used.
	// Skips other actions
	if (stats[1] <= stats[2] && item.second) {
		use_item();
		return;
	}

	// Random actions from enemy
	switch (rand_choice) {
		case 1:
			rps(a_stats, temp_defense, a_name); break;
		case 2:
			attack(a_stats[1], temp_defense, a_name); 
	}
}

// Chooses a random Avatar stat to affect. Plays rock-paper-scissors with the user.
// Will continue until a tie doesn’t happen. Who ever wins gets a stat increase or decrease in favor.
void Enemies::rps(vector<int>& a_stats, int& temp_defense, string a_name) {
	// All Declarations
	string choice = "";
	int rand_stat, rand_rps_int, valid = 0, winner = 0, who_won = 0;
	char e_rps = ' ', rps_choice;
	
	// Play Rock-Paper-Scissors until user or enemy wins.
	cout << "The enemy challenged you in Rock-Paper-Scissors for stat effects!" << endl;
	while (!winner) {
		int rand_stat = rand() % 3 + 1; // 1 = Attack. 2 = Defense. 3 = Evasion
		int rand_rps_int = rand() % 3 + 1; // 1 = Rock. 2 = Paper. 3 = Scissors

		valid = 0;
		while (!valid) { // Short validation
			cout << "Rock-Paper-Scissors Choice (R/P/S): ";
			cin >> rps_choice; rps_choice = toupper(rps_choice);
			cin.ignore(1000, '\n');  valid = 1;
			switch (rps_choice) {
				case 'R':
					choice = "Rock"; break;
				case 'P':
					choice = "Paper"; break;
				case 'S':
					choice = "Scissors"; break;
				default:
					cout << "That isn't a valid character..." << endl << endl;
					valid = 0;
			}
		}

		// Print out user and enemy's choices.
		cout << a_name << " chose " << choice << ". " << get_name() << " chose ";
		switch (rand_rps_int) {
			case 1:
				e_rps = 'R';
				cout << "Rock." << endl; break;
			case 2:
				e_rps = 'P';
				cout << "Paper." << endl; break;
			case 3:
				e_rps = 'S';
				cout << "Scissors." << endl;
		}
			
		// Decides the winner
		if (e_rps != rps_choice) { 
			winner = 1;
			if ((rps_choice == 'S' && e_rps == 'P') ||
				(rps_choice == 'P' && e_rps == 'R') ||
				(rps_choice == 'R' && e_rps == 'S'))
				who_won = 1; // User wins
			else
				who_won = 2; // Enemy wins
			switch (rand_stat) {
				case 1:
					if (who_won == 1) {
						cout << a_name << "'s Attack rose!!!" << endl << endl;
						a_stats[4] = double(a_stats[4]) * 1.33;
					}
					else {
						cout << a_name << "'s Attack fell..." << endl << endl;
						a_stats[4] = double(a_stats[4]) * 0.66;
					}
					break;
				case 2:
					if (who_won == 1) {
						cout << a_name << "'s Defense rose!!!" << endl << endl;
						temp_defense = double(temp_defense) * 1.33;
					}
					else {
						cout << a_name << "'s Defense fell..." << endl << endl;
						temp_defense = double(temp_defense) * 0.66;
					}
					break;
				case 3:
					if (who_won == 1) {
						cout << a_name << "'s Evasion rose!!!" << endl << endl;
						a_stats[6] = double(a_stats[6]) * 1.33;
					}
					else {
						cout << a_name << "'s Evasion fell..." << endl << endl;
						a_stats[6] = double(a_stats[6]) * 0.66;
					}
			} 
		}
		else // Happens if a tie occurs
			cout << "It's a tie... Redo coming up." << endl << endl;
	}
}
