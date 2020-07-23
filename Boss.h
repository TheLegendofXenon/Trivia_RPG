#ifndef BOSS_H
#define BOSS_H

#include "Enemy.h"
#include <cstdlib>
#include <ctime>

class Boss : public Enemy {
public:
	Boss();

	void random_choice(vector<int>&, int&, string);
	void roll_dice(vector<int>&, int&, string);
};
#endif
