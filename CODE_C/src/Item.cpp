#include "../include/Item.h"
#include "../include/Player.h"
#include "../include/Monster.h"

Item::Item(const string& name, ItemType type, int value, int quantity, bool on_player)
	: name(name), type(type), value(value), quantity(quantity), on_player(on_player) {}

void Item::apply(Player& p, Monster& m) {
	if (on_player) {
		if (value >= 0)
			p.heal(value);
		else
			p.takeDamage(-value);
	} else {
		if (value >= 0)
			m.heal(value);
		else
			m.takeDamage(-value);
	}
}

string Item::getName() const {
	return name;
}

ItemType Item::getType() const {
	return type;
}

int Item::getValue() const {
	return value;
}

int Item::getQuantity() const {
	return quantity;
}

void Item::decrement() {
	if (quantity > 0) quantity--;
}
