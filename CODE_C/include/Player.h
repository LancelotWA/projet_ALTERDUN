#pragma once
#include <string>
#include <vector>
#include "Entity.h"
#include "Item.h"
using std::string;
using std::vector;

class Monster;

class Player : public Entity {
private:
	string name;
	vector<Item> inventory;

public:
	Player(int hp, int hpMax, const string& name);

	void fight(Monster& m);
	void useItem(int index);
	bool mercy(Monster& m);
	string getName() const;
	vector<Item>& getInventory();
};
