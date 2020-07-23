#include "Enemy.h"
#include "Avatar.h"

int Enemy::EXP; // Static Data Member

// This function return's the enemy's name.
string Enemy::get_name() {
	return name;
}

// This function returns a drop from the enemy.
pair<string, int> Enemy::get_item() {
	return item;
}

// This function returns the EXP gained from the enemy.
int Enemy::get_EXP() {
	return EXP;
}

// This function performs a normal enemy attack.
void Enemy::attack(int& a_HP, int temp_defense, string a_name) {
	int rand_attack, rand_defense, temp_attack, temp_def, actual_attack;
	
	rand_attack = rand() % 10 + 41; // 40 to 50
	rand_defense = rand() % 10 + 21; // 20 to 30

	// Temporarily give random attack range to reduce enemy HP.
	temp_attack = stats[3] * double(rand_attack) / 100;
	temp_def = temp_defense * double(rand_defense) / 100;
	actual_attack = temp_attack - temp_def; 
	if (actual_attack < 0)
		actual_attack = 1;
	a_HP -= actual_attack;
	cout << name << " did " << actual_attack << " HP damage on " 
		<< a_name << "..." << endl << endl;
}

// This function uses an enemy's item if current HP <= critical HP.
void Enemy::use_item() {
	// Adds healing item HP to current HP. Sets current HP to max HP if overflow.
	if (item.second + stats[1] > stats[0])
		stats[1] = stats[0];
	else
		stats[1] += item.second;
	
	string item_name = item.first.substr(0, item.first.find('('));
	cout << name << " used a " << item_name << " and recovered " << item.second
		<< " HP..." << endl << endl;
	// Set item to a used pair.
	item = pair<string, int>{"(", 0};
}

// Returns 1 if current HP <= 0. Returns 0 otherwise.
int Enemy::die() {
	if (stats[1] <= 0) {
		return 1;
	}
	return 0;
}

// This [] operator function allows easier access of int values in the stats vector.
int& Enemy::operator[](int index) {
	return stats[index];
}

// This function allows a fight between the user and an enemy.
void fight(Avatar& a, Enemy& e) {
	// Make all checks = 0 and print apperance.
	int a_death_check = 0, e_death_check = 0, flee_check = 0, fight_done = 0;
	cout << e.get_name() << " has appeared." << endl << endl;

	// Holds reset values for user stats
	int real_attack = a.stats[4];
	int real_defense = a.stats[5];
	int real_evasion = a.stats[6];

	// The fighting
	while (!fight_done) {
		cout << a.get_name() << "'s HP: (" << a.stats[1] << " / " << a.stats[0] << ')';
		cout << "\t\t\t\t\t\t" << e.get_name() << "'s HP: (" << e[1] << " / " << e[0] << ')' << endl;

		// Makes defend option last only one turn.
		a.temp_defense = a.stats[5];

		// User's turn
		a.action(e, flee_check);
		if (a.die() || e.die() || flee_check) {
			if (e.die()) {
				a.stats[4] = real_attack;
				a.stats[5] = real_defense;
				a.stats[6] = real_evasion;
				cout << e.get_name() << " has been defeated!" << endl;
				a.level_up(e);
				if (e.get_item().second != 0) {
					cout << e.get_item().first.substr(0, e.get_item().first.find('('))
						<< " has been dropped." << endl << endl;
					a.pick_up(e.get_item().first, e.get_item().second);
				}
			}
			break;
		}

		// Enemy's turn
		e.random_choice(a.stats, a.temp_defense, a.get_name());
		if (a.die() || e.die()) {
			if (e.die()) {
				a.stats[4] = real_attack;
				a.stats[5] = real_defense;
				a.stats[6] = real_evasion;
				cout << e.get_name() << " has been defeated!" << endl;
				a.level_up(e);
				if (e.get_item().second != 0) {
					cout << e.get_item().first.substr(0, e.get_item().first.find('('))
						<< " has been dropped." << endl << endl;
					a.pick_up(e.get_item().first, e.get_item().second);
				}
			}
			break;
		}
	}
}