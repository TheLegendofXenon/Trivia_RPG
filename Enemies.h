#ifndef ENEMIES_H
#define ENEMIES_H

#include "Enemy.h"

class Enemies : public Enemy {
public:
	Enemies(int); 
	
	void random_choice(vector<int>&, int&, string); // DONE.
	void rps(vector<int>&, int&, string); // DONE.
};

#endif 
