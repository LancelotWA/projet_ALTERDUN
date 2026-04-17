#pragma once
#include "common.h"
#include "ItemType.h"

class Player;
class Monster;

class Item {
private:
	string name;
	ItemType type;
	int value;
	int quantity;
	bool on_player;

public:
	Item(const string& name, ItemType type, int value, int quantity, bool on_player);

	void apply(Player& p, Monster& m);
	string getName() const;
	int getQuantity() const;
	void decrement();
};
