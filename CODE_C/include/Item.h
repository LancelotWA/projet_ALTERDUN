#pragma once
#include <string>
#include "ItemType.h"
using std::string;

class Player;
class Monster;

class Item {
private:
	string name;
	ItemType type;
	int value;
	int quantity;

public:
	Item(const string& name, ItemType type, int value, int quantity);

	void apply(Player& p, Monster& m);
	string getName() const;
	int getQuantity() const;
	void decrement();
};
