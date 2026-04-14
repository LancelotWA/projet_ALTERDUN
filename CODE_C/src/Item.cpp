#include "../include/Item.h"
#include "../include/Player.h"
#include "../include/Monster.h"

Item::Item(const string& name, ItemType type, int value, int quantity)
	: name(name), type(type), value(value), quantity(quantity) {}

void Item::apply(Player& p, Monster& m) {
}

string Item::getName() const {
	return "";
}

int Item::getQuantity() const {
	return 0;
}

void Item::decrement() {
}
