#include "../include/Monster.h"
#include "../include/Player.h"

Monster::Monster(int hp, int hpMax, const string& name, int atk, int def, int mercy_obj)
	: Entity(hp, hpMax), name(name), atk(atk), def(def), mercy(0), mercy_obj(mercy_obj) {}

void Monster::attack(Player& p) {
}

bool Monster::canBeSpared() const {
	return false;
}

string Monster::getName() const {
	return "";
}

int Monster::getMercy() const {
	return 0;
}

vector<string>& Monster::getActIds() {
	return actIds;
}
