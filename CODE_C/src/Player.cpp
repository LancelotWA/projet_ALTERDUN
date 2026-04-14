#include "../include/Player.h"
#include "../include/Monster.h"

Player::Player(int hp, int hpMax, const string& name)
	: Entity(hp, hpMax), name(name) {}

void Player::fight(Monster& m) {
}

void Player::useItem(int index) {
}

bool Player::mercy(Monster& m) {
	return false;
}

string Player::getName() const {
	return "";
}

vector<Item>& Player::getInventory() {
	return inventory;
}
