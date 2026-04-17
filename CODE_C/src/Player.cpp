#include "../include/Player.h"
#include "../include/Monster.h"

Player::Player(int hp, int hpMax, const string& name)
	: Entity(hp, hpMax), name(name) {}

void Player::fight(Monster& m) {
	static mt19937 rng(random_device{}());
	uniform_int_distribution<int> dist(0, m.getHpMax());
	int dmg = dist(rng);
	m.takeDamage(dmg);
}

void Player::useItem(int index, Monster& m) {
	if (index < 0 || index >= (int)inventory.size()) return;
	if (inventory[index].getQuantity() <= 0) return;
	inventory[index].apply(*this, m);
	inventory[index].decrement();
}

bool Player::mercy(Monster& m) {
	return m.canBeSpared();
}

string Player::getName() const {
	return name;
}

vector<Item>& Player::getInventory() {
	return inventory;
}
