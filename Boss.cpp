#include "Boss.h"

// This constructor that initilizes Boss' data members.
// It also seeds in the random number generator.
Boss::Boss() {
	name = "Ragnarok"; // The boss' name
	stats.push_back(500); // stats[0] is the MAX HP
	stats.push_back(stats[0]); // stats[1] is the current HP
	stats.push_back(125); // stats[2] is the critical HP
	stats.push_back(300); // stats[3] is the attack stat
	stats.push_back(300); // stats[4] is the defense stat
	stats.push_back(9999999); // stats[5] is the grip stat (No escape)
	EXP = 0; // No Exp
	item = { "Med-Kit(HP)", 200 }; // Med-Kit item

	// Seeds the random number generator with a value based on time().
	int seconds = time(nullptr);
	srand(seconds);
}

// This function performs either dice roll or use item.
// Meant to overide pure virtual random_choice function.
void Boss::random_choice(vector<int>& a_stats, int& temp_defense, string a_name) {

	// Use item occurs at critical HP and when item hasn't been used.
	// Skips rolls dice
	if (stats[1] <= stats[2] && item.second) {
		use_item();
		return;
	}

	roll_dice(a_stats, temp_defense, a_name); 
}

// This function rolls 2 dice which gives special effects only
// from the boss.
void Boss::roll_dice(vector<int>& a_stats, int& temp_defense, string a_name) {
	int die1, die2, temp_attack, temp_def, rand_var1, rand_var2, HP_steal,
		real_attack;
	
	die1 = 4; //rand() % 6 + 1; // 1 to 6
	die2 = 5; //rand() % 6 + 1; // 1 to 6
	rand_var1 = rand() % 20 + 41; // 41 to 60
	rand_var2 = rand() % 20 + 21; // 21 to 40

	cout << name << " rolls his deadly, fuzzy dice." << endl;
	cout << "They read " << die1 << " and " << die2 << ". ";
	int dice = die1 + die2;
	switch (dice) { // Work on Cases!!!
	case 2: // Reduces the user HP to 1.
		cout << "Snake Eyes! " << a_name << "'s HP has been reduced to 1..." 
			<< endl << endl;
		a_stats[1] = 1;
		break;
	case 3: // Performs a normal attack
		cout << "Normal Punch! ";
		attack(a_stats[1], temp_defense, a_name);
		break;
	case 4: // Swaps the user and the boss' attack and defense stats.
		cout << name << " and " << a_name <<
			"'s attack and defense stats have swapped!!!" << endl << endl;
		temp_def = stats[4];
		stats[4] = temp_defense;
		temp_defense = temp_def;
		temp_attack = stats[3];
		stats[3] = a_stats[4];
		a_stats[4] = temp_attack;
		break;
	case 5: // Boss damages himself.
		temp_attack = stats[3] * double(rand_var1) / 100;
		temp_def = stats[4] * double(rand_var2) / 100;
		if (temp_def > temp_attack)
			temp_def = temp_attack - 1;
		cout << name << " did " << temp_attack - temp_def << 
			" damage to himself!!!" << endl << endl;
		stats[1] -= (temp_attack - temp_def);
		break;
	case 6: // Boosts attack and defense stat by 3% each.
		cout << name << " is bulking up..." << endl << endl;
		stats[3] *= 1.03;
		stats[4] *= 1.03;
 		break;
	case 7:
		real_attack = stats[3];
		stats[3] *= 2;
		cout << "Laser Beam! ";
		attack(a_stats[1], temp_defense, a_name);
		stats[3] = real_attack;
		break;
	case 8: // Performs a triple attack, each with half the regular damage.
		cout << "Triple Claw! " << endl;
		attack(a_stats[1], temp_defense * 2, a_name);
		attack(a_stats[1], temp_defense * 2, a_name);
		attack(a_stats[1], temp_defense * 2, a_name);
		break;
	case 9: // Sets the user's HP to half of the max HP.
		a_stats[1] = a_stats[0] / 2;
		cout << "SNAP! Your HP is now half of your max HP!" << endl << endl;
		break;
	case 10: // Perform a normal attack
		cout << "Normal Kick! ";
		attack(a_stats[1], temp_defense, a_name);
		break;
	case 11: // Steal 20% of the user's current HP.
		HP_steal = a_stats[1] * .2;
		a_stats[1] -= (HP_steal);
		if (stats[1] + HP_steal > stats[0])
			stats[1] = stats[0];
		else
			stats[1] += (HP_steal);
		cout << name << " stole " << HP_steal << " of your HP..."
			<< endl << endl;
		break;
	case 12: // Restores half of the boss' HP
		cout << "Restoration! " << name << " has recovered 250 HP..." 
			<< endl << endl;
		if (stats[1] + (stats[0] / 2) > stats[0])
			stats[1] = stats[0];
		else
			stats[1] += stats[0] / 2;
	}
}
