#include "Avatar.h"

// This constructor takes in a name string and intializes data members.
// It also seeds in the random number generator.
Avatar::Avatar(string n) {
	// All initializations
	name = n; // Name can be chosen by user, or default is 'Xenon'. 
	stats.push_back(25); // stats[0] is the MAX HP
	stats.push_back(stats[0]); // stats[1] is the current HP
	stats.push_back(15); // stats[2] is the EXP cap
	stats.push_back(0); // stats[3] is the current EXP 
	stats.push_back(25); // stats[4] is the attack stat
	stats.push_back(25); // stats[5] is the defense stat
	stats.push_back(25); // stats[6] is the evasion stat
	temp_defense = stats[5]; // The temp defense stat
	level = 1; // Manipulate to test boss

	equipment.push_back(pair<string, int> {"N/A(W)", 0}); // Weapon
	equipment.push_back(pair<string, int> {"N/A(B)", 0}); // Body Armor
	equipment.push_back(pair<string, int> {"N/A(H)", 0}); // Headgear

	// Seeds the random number generator with a value based on time().
	int seconds = time(nullptr); 
	srand(seconds);
}

// This function returns the avatar's name.
string Avatar::get_name() {
	return name;
}

// This function returns the avatar's level.
int Avatar::get_level() {
	return level;
}

// This function allows a user to choose from a set of battle actions.
void Avatar::action(Enemy& e, int& flee_check) {
	// All Declarations
	int valid = 0;
	char action_choice;
	
	// Print out options
	cout << endl << "(A) - Attack " << endl;
	cout << "(Q) - Questions " << endl;
	cout << "(U) - Use Items " << endl;
	cout << "(D) - Defend" << endl;
	cout << "(F) - Flee" << endl << endl;

	// Action validiation loop
	while (!valid) {
		cout << "Choose your next action: ";
		cin >> action_choice;
		action_choice = toupper(action_choice); cin.ignore(1000, '\n');
		valid = 1;
		switch (action_choice) {
			case 'A':
				attack(e); break;
			case 'Q':
				questions(e); break;
			case 'U':
				use_item(); break;
			case 'D':
				defend(); break;
			case 'F':
				flee_check = flee(e[5]); break;
			default:
				cout << "That isn't a valid character..." << endl << endl;
				valid = 0;
		}
	}
}

// This function causes the user to perform a normal attack on the enemy.
void Avatar::attack(Enemy& e) {
	int rand_attack, rand_defense, temp_attack, temp_def, actual_attack; // All Declarations

	rand_attack = rand() % 25 + 26; // 25 to 50
	rand_defense = rand() % 10 + 11; // 1 to 10

	// Temporarily give random attack range to reduce enemy HP.
	temp_attack = stats[4] * double(rand_attack) / 100;
	temp_def = e[4] * double(rand_defense) / 100;
	actual_attack = temp_attack - temp_def;
	if (actual_attack < 0)
		actual_attack = 1;
	e[1] -= actual_attack;
	cout << name << " did " << actual_attack << " HP damage on " 
		<< e.get_name() << '!' << endl;
}

// This function allows you to answer a question to manipulate the enemy's stats.
// If successful, you can decrease a desired stat. However, failure will result
// in the opposite affect.
void Avatar::questions(Enemy& e) {
	// All Declarations
	string question = "", eat_string;
	char who_choice, stat_choice, answer_choice, actual_answer = ' ';
	int valid = 0, rand_question;

	cout << question << endl; // Test

	while (!valid) { // Short validation for stat choice
		cout << "Which enemy stat do you want to affect (Attack, Defense, Grip)? (A, D, G): ";
		cin >> stat_choice; stat_choice = toupper(stat_choice);
		cin.ignore(1000, '\n');  valid = 1;
		switch (stat_choice) { // Work On.
		case 'A': 
		case 'D':
		case 'G':
			break;
		default:
			cout << "That isn't a valid character. Please try again..." << endl << endl; 
			valid = 0;
		}
	}

	// Open "Questions.txt" and determine which question to choose.
	ifstream questions_file;
	questions_file.open("Questions.txt");
	rand_question = rand() % 19 + 1;
	if (questions_file) {
		for (int i = 0; i < rand_question - 1 && !questions_file.eof(); ++i) {
			for (int r = 0; r < 6; ++r)
				getline(questions_file, eat_string, '\n');
		}
	}

	// Take the question and print it. Manipulate to remove correct answer and store it.
	cout << endl;
	for (int q = 0; q < 5; ++q) {
		getline(questions_file, question, '\n');
		if (question.find('^') != -1) {
			actual_answer = question.front();
			question.pop_back();
		}
		cout << question << endl;
	}
	cout << endl;
	questions_file.close();

	// See if the user answers correctly and give the appropriate stat change.
	cout << "Answer Choice: ";
	cin >> answer_choice;
	answer_choice = toupper(answer_choice); cin.ignore(1000, '\n');
	valid = 0;
	while (!valid) {
		switch (answer_choice) {
			case 'A':
			case 'B':
			case 'C':
			case 'D':
				if (answer_choice == actual_answer) {
					cout << "CORRECT!!!  ";
					if (stat_choice == 'A') {
						e[3] = double(e[3]) * .66;
						cout << e.get_name() << "'s attack has lowered!" << endl << endl;
					}
					else if (stat_choice == 'D') {
						e[4] = double(e[4]) * .66;
						cout << e.get_name() << "'s defense has lowered!" << endl << endl;
					}
					else {
						e[5] = double(e[5]) * .66;
						cout << e.get_name() << "'s grip has lowered!" << endl << endl;
					}
				}
				else {
					cout << "WRONG...  " << endl;
					if (stat_choice == 'A') {
						e[3] = double(e[3]) * 1.33;
						cout << e.get_name() << "'s attack has boosted..." << endl << endl;
					}
					else if (stat_choice == 'D') {
						e[4] = double(e[4]) * 1.33;
						cout << e.get_name() << "'s defense has boosted..." << endl << endl;
					}
					else {
						e[5] = double(e[5]) * 1.33;
						cout << e.get_name() << "'s grip has boosted..." << endl << endl;
					}
				}
				valid = 1; break;
			default:
				cout << "That isn't a valid character. Please try again..." << endl << endl;
		}

	}
}

void Avatar::use_item() {
	int has_heal = 0, item_number = 1, item_choice, count = 0; // All Declarations

	// Check if current HP is at max HP.
	if (stats[0] == stats[1]) {
		cout << "You do not need to heal..." << endl << endl;
		return;
	}

	// Finds if there are healing items.
	for (pair<string, int> i : items) {
		if (i.first.find("(HP)") != -1) {
			has_heal = 1;
		}
	}
	if (!has_heal) {
		cout << "No healing items in inventory..." << endl;
		return;
	}

	// Let the user pick the healing item. Behaves like a drop_item function, 
	// except only accepts healing items.
	while (true) { 
		cout << "Which item number should you use? ";
		try {
			if (cin >> item_choice) {
				if (item_choice <= items.size() && item_choice > 0 && 
					items[item_choice - 1].first.find("(HP)") != -1) {
					cin.ignore(1000, '\n'); break;
				}
				else {
					cin.clear(); cin.ignore(1000, '\n');
					throw invalid_argument("That isn't a valid number...");
				}
			}
			else {
				cin.clear(); cin.ignore(1000, '\n');
				throw invalid_argument("That isn't a number...");
			}
		}
		catch (exception& i) {
			cout << i.what() << endl << endl;
		}
	}

	// Use healing item
	if (items[item_choice - 1].second + stats[1] > stats[0])
		stats[1] = stats[0];
	else
		stats[1] += items[item_choice - 1].second;
	items.erase(items.begin() + item_choice - 1); // Removal
}

// This function allows the user to try and escape.
int Avatar::flee(int grip) {
	int evasion = rand() % 25 + 76; // 75 to 100
	int rand_grip = rand() % 25 + 76; // 75 to 100

	// Temporarily give random evasion stat to compare with random enemy grip stat.
	int temp_evasion = stats[6] * double(evasion) / 100;
	int temp_grip = grip * double(rand_grip) / 100;
	if (temp_evasion > temp_grip) {
		cout << "You have successfully escaped!" << endl << endl;
		return 1;
	}
	cout << "Escaped failed..." << endl;
	return 0;
}

// This function temporarily increases the user's defense stat.
void Avatar::defend() {
	int defend = rand() % 25 + 26; // 25 to 50

	temp_defense *= (1 + double(defend) / 100);
	cout << name << " crossed his/her arms!  " << endl;
}

// This function allows the user to view stats, equipment, or items. Each option
// includes switching out equipment, healing if necessary, or dropping items.
void Avatar::menu(Avatar& a, Boss& r) {
	// All declarations
	string item_name;
	int valid = 0, has_item = 0, has_heal_item = 0, item_number = 1, 
		has_equipment = 0, has_weapon = 0, has_armor = 0, has_headgear = 0, print;
	char menu_choice, drop_choice = ' ', heal_choice, equipment_choice;
	
	// Print out Menu
	cout << endl << "MENU" << endl << endl;
	cout << "(S) - Stats (includes healing)" << endl;
	cout << "(E) - Equipment (includes equipping)" << endl;
	cout << "(I) - Items (includes dropping)" << endl << endl;
	
	// Menu printing and functions
	while (!valid) { 
		cout << "Menu Choice: ";
		cin >> menu_choice; menu_choice = toupper(menu_choice);
		cin.ignore(1000, '\n');  valid = 1;
		switch (menu_choice) {
			case 'E': // Work on when items are added
				cout << endl << "EQUIPMENT" << endl << endl;
				cout << "Weapon: " << equipment[0].first.substr(0, equipment[0].first.find('(')) 
					<< " (+" << equipment[0].second << " Attack)" << endl;
				cout << "Armor: " << equipment[1].first.substr(0, equipment[1].first.find('('))
					<< " (+" << equipment[1].second << " Defense)" << endl;
				cout << "Headgear: " << equipment[2].first.substr(0, equipment[2].first.find('('))
					<< " (+" << equipment[2].second << " Defense)" << endl << endl;
				
				for (pair<string, int> i : items) {
					if (i.first.find("(W)") != -1 || i.first.find("(B)") != -1
						|| i.first.find("(H)") != -1)
						if (i.first.find("(W)") != -1)
							has_weapon = 1;
						else if (i.first.find("(B)") != -1)
							has_armor = 1;
						else
							has_headgear = 1;
						has_equipment = 1;
				}
				if (!has_equipment)
					return;

				valid = 0;
				while (!valid) {
					cout << "Choose equipment to exchange out (W/A/H): ";
					cin >> equipment_choice;
					equipment_choice = toupper(equipment_choice); cin.ignore(1000, '\n');
					valid = 1;
					switch (equipment_choice) {
					case 'W':
						if (!has_weapon) {
							cout << "There are no weapons in the inventory..." << endl << endl;
							break;
						}
						print = print_items();
						equip("(W)");
						break;
					case 'A':
						if (!has_armor) {
							cout << "There is no armor in the inventory..." << endl << endl;
							break;
						}
						print = print_items();
						equip("(B)");
						break;
					case 'H':
						if (!has_headgear) {
							cout << "There are no headgears in the inventory..." << endl << endl;
							break;
						}
						print = print_items();
						equip("(H)");
						break;
					default:
						cout << "That isn't a valid character..." << endl << endl;
						valid = 0;
					}
				}
				break;
			case 'I': // Print out items
				print = print_items(); 
				if (!print) {
					valid = 0;
					while (!valid) { // Allows user to drop items
						cout << "Drop item? (Y/N): ";
						cin >> drop_choice;
						drop_choice = toupper(drop_choice); cin.ignore(1000, '\n');
						valid = valid_choice(drop_choice, 'Y', 'N');
					}
					if (drop_choice == 'Y') {
						drop_item();
						cout << endl;
					}
					else
						cout << endl;
				}
				break;
			case 'S': // Print out stats 
				cout << endl << "STATS" << endl << endl;
				cout << "Level: " << level << endl << endl;
				cout << "HP: (" << stats[1] << " / " << stats[0] << ')' << endl;
				cout << "EXP: (" << stats[3] << " / " << stats[2] << ')' << endl;
				cout << "Attack: " << stats[4] << endl;
				cout << "Defense: " << stats[5] << endl;
				cout << "Evasion: " << stats[6] << endl << endl;

				// Ask user for healing if current HP < max HP
				if (stats[1] < stats[0]) {
					valid = 0;
					while (!valid) {
						cout << "Want to use a heal item? (Y/N): ";
						cin >> heal_choice;
						heal_choice = toupper(heal_choice); cin.ignore(1000, '\n');
						valid = valid_choice(heal_choice, 'Y', 'N');
					}
					if (heal_choice == 'Y') {
						use_item();
						cout << endl;
					}
				}
				break;
			default:
				cout << "That isn't a valid character..." << endl << endl;
				valid = 0;
		}
	}

	if (level == 10) {
		cout << "..........";
		fight(a, r);
	}
}

// This function allows the user to explore the area. Either a fight can occur, you can
// find an item, or nothing happens.
void Avatar::explore(Avatar& a) {
	// All declarations
	string true_item, item, eat_string, type_string, item_name;
	int explore, item_stat, items_allowed, rand_item, valid = 0, item_number = 1, print;
	char pickup_choice = ' ', drop_choice = ' ';

	explore = rand() % 100 + 1; // 0 to 100

	if (explore <= 65) { // A fight occurs
		cout << endl << "!SKIRRTTT!  ";
		Enemies e = Enemies(level); // Enemy object creation
		fight(a, e);
	}
	else if (explore > 80 & explore <= 100) { // Finding an item occurs
		// Determines which items can appear
		switch (level) {
			case 1:
				items_allowed = 2; break;
			case 2: 
				items_allowed = 4; break;
			case 3:
				items_allowed = 6; break;
			case 4:
				items_allowed = 8; break;
			case 5:
				items_allowed = 10; break;
			case 6:
				items_allowed = 12; break;
			case 7:
				items_allowed = 14; break;
			case 8:
				items_allowed = 16; break;
			case 9:
				items_allowed = 17;
		}
		int rand_item = rand() % items_allowed + 1; // 1 to items allowed at current level.

		// Read items in from "Items.txt" file
		ifstream item_file;
		item_file.open("Items.txt");
		if (item_file) {
			for (int i = 0; i < rand_item && !item_file.eof(); ++i) {
				getline(item_file, true_item, '\t');
				item_file >> item_stat;
				getline(item_file, eat_string, '\n');
			}
		}
		item_file.close();

		// Print found item
		item = true_item.substr(0, true_item.find('('));
		type_string = true_item.substr(true_item.find('('));
		cout << endl << "You have discovered a(n) " << item << " (+" << item_stat;
		if (type_string.find('W') != -1)
			cout << " Attack)" << endl;
		else
			cout << " Defense)" << endl;
		
		// Allows user to pickup item
		pick_up(true_item, item_stat);
		cout << endl;
	}
	else // Nothing happens...
		cout << endl << "You explored the gloomy area, but found nothing..." << endl << endl;
}

// This function returns 1 if the current HP stat is <= 0. Returns 0 otherwise.
int Avatar::die() { 
	if (stats[1] <= 0) {
		return 1;
	}
	return 0; 
}

// This function levels up the avatar if the received EXP from an enemy + the current EXP >
// then the EXP cap. Activates after fight win.
void Avatar::level_up(Enemy& e) {
	if (level == 10)
		return;
	cout << "You gained " << e.get_EXP() << " EXP.";
	if (stats[3] + e.get_EXP() >= stats[2]) { // Level up occurs
		stats[3] = (stats[3] + e.get_EXP()) % stats[2]; // Carry over leftover current EXP
		level++;
		switch (level) {
			case 2:
			case 3:
			case 4:
				stats[0] += 10; // Max HP increase
				stats[2] += 20; // EXP cap increase
				stats[4] += 5; // Attack stat increase
				stats[5] += 5; // Defense stat increase
				stats[6] += 10; // Evasion stat increase
				break;
			case 5:
				stats[0] += 30;
				stats[2] += 50;
				stats[4] += 15;
				stats[5] += 15;
				stats[6] += 30;
				break;
			case 6:
			case 7:
			case 8:
			case 9:
				stats[0] += 10;
				stats[2] += 20;
				stats[4] += 5;
				stats[5] += 5;
				stats[6] += 10;
				break;
			case 10:
				stats[0] += 30;
				stats[2] += 50;
				stats[4] += 15;
				stats[5] += 15;
				stats[6] += 30;
				stats[7] += 30;
				break;
		}
		stats[1] = stats[0]; // current HP = max HP
		cout << " Congratulatons, you are now level " << level << '!';
	}
	else { // No level up occurs
		stats[3] += e.get_EXP();
	}
	cout << " ---> (" << stats[3] << " / " << stats[2] << ')' << endl << endl;
}

// This function allows the user to swap equipment and adjusts stats accordingly.
void Avatar::equip(string equip_choice) {
	// All Declarations
	int item_choice, temp_int;
	string temp_string;
	
	// Asks which item to remove
	while (true) {
		cout << "Which of the matching equipment should be swapped? ";
		try {
			if (cin >> item_choice) {
				if (item_choice <= items.size() && item_choice > 0 &&
					items[item_choice - 1].first.find(equip_choice) != -1) {
					cin.ignore(1000, '\n'); break;
				}
				else {
					cin.clear(); cin.ignore(1000, '\n');
					throw invalid_argument("That isn't a valid number...");
				}
			}
			else {
				cin.clear(); cin.ignore(1000, '\n');
				throw invalid_argument("That isn't a number...");
			}
		}
		catch (exception& i) {
			cout << i.what() << endl << endl;
		}
	}
	
	// Swaps equipment and adjusts stats accordingly
	for (int i = 0; i < 3; i++) { // For-loop will manipulate equipment values.
		if (equipment[i].first.find(equip_choice) != -1) {
			temp_string = items[item_choice - 1].first;
			items[item_choice - 1].first = equipment[i].first;
			equipment[i].first = temp_string;
			temp_int = items[item_choice - 1].second;
			items[item_choice - 1].second = equipment[i].second;
			equipment[i].second = temp_int;
			if (equipment[i].first.find("(W)") != -1) {
				stats[4] += equipment[i].second;
				stats[4] -= items[item_choice - 1].second;
			}
			else {
				stats[5] += equipment[i].second;
				stats[5] -= items[item_choice - 1].second;
			}
		}
	}

	// Removal of n/a equipment and printing
	if (items[item_choice - 1].second == 0)
		items.erase(items.begin() + item_choice - 1); 
	cout << endl;
}

// This function prints out a list of items to output and "Inventory.txt" file
int Avatar::print_items() {
	int item_number = 1, has_item = 0; // All Declarations
	
	// Creates "Inventory.txt"
	ofstream inv_file;
	inv_file.open("Inventory.txt");

	// Prints out items if they are items in inventory
	cout << endl << "ITEMS" << endl << endl;
	inv_file << "ITEMS" << endl << endl;
	for (pair<string, int> i : items) {
		string item_name = i.first.substr(0, i.first.find('('));
		cout << item_number << ") " << item_name << " (+" << i.second;
		inv_file << item_number++ << ") " << item_name << " (+" << i.second;
		if (i.first.find("(W)") != -1) {
			cout << " Attack)" << endl;
			inv_file << " Attack)" << endl;
		}
		else if (i.first.find("(B)") != -1 || i.first.find("(H)") != -1) {
			cout << " Defense)" << endl;
			inv_file << " Defense)" << endl;
		}
		else {
			cout << " HP)" << endl;
			inv_file << " HP)" << endl;
		}
		has_item = 1;
	}

	if (!has_item) {// Occurs when no items in inventory
		cout << "No items in inventory..." << endl << endl;
		inv_file << "No items in inventory..." << endl << endl;
		return 1;
	}
	cout << endl;
	inv_file.close();
	return 0;
}

// This function drops one chosen item from inventory.
void Avatar::drop_item() {
	int item_choice;

	// Asks which item to remove
	while (true) {
		cout << "Which item number should be removed? ";
		try {
			if (cin >> item_choice) {
				if (item_choice <= items.size() && item_choice > 0) {
						cin.ignore(1000, '\n'); break;
				}
				else {
					cin.clear(); cin.ignore(1000, '\n');
					throw invalid_argument("That isn't a valid number...");
				}
			}
			else {
				cin.clear(); cin.ignore(1000, '\n');
				throw invalid_argument("That isn't a number...");
			}
		}
		catch (exception& i) {
			cout << i.what() << endl << endl;
		}
	}
	items.erase(items.begin() + item_choice - 1); // Removal
}

// This function allows a user to pickup an item found.
void Avatar::pick_up(string item, int item_stat) {
	// All Declarations
	int valid = 0, print;
	char pickup_choice, drop_choice;
	
	while (!valid) {
		cout << "Pick up item? (Y/N): ";
		cin >> pickup_choice;
		pickup_choice = toupper(pickup_choice); cin.ignore(1000, '\n');
		valid = valid_choice(pickup_choice, 'Y', 'N');
	}
	if (pickup_choice == 'Y' && items.size() < 5)
		items.push_back(pair<string, int> {item, item_stat});
	else if (pickup_choice == 'Y' && item.size() >= 5) {
		valid = 0;
		while (!valid) { // Allows user to drop item
			cout << "Inventory full... Drop item to collect? (Y/N): ";
			cin >> drop_choice;
			drop_choice = toupper(drop_choice); cin.ignore(1000, '\n');
			valid = valid_choice(drop_choice, 'Y', 'N');
		}
		if (drop_choice == 'Y') {
			print = print_items();
			drop_item();
			items.push_back(pair<string, int> {item, item_stat});
		}
		else
			cout << endl;
	}
}

// This function checks to see whether an entered character matches either of 
// two required characters.
int Avatar::valid_choice(char check_char, char chr1, char chr2) {
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
