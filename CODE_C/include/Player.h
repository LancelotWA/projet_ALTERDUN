#pragma once
#include "common.h"
#include "Entity.h"
#include "Item.h"

class Monster;

class Player : public Entity {
private:
	string name;
	vector<Item> inventory;

public:
	Player(int hp, int hpMax, const string& name);

	void fight(Monster& m);
	void useItem(int index, Monster& m);
	bool mercy(Monster& m);
	string getName() const;
	vector<Item>& getInventory();
};
