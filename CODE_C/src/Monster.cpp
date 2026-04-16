#include "../include/Monster.h"
#include "../include/Player.h"

Monster::Monster(int hp, int hpMax, const string& name, int atk, int def, int mercy_obj)
	: Entity(hp, hpMax), name(name), atk(atk), def(def), mercy(0), mercy_obj(mercy_obj) {}

void Monster::attack(Player& p) {
	p.takeDamage(atk);
}

bool Monster::canBeSpared() const {
	return mercy >= mercy_obj;
}

string Monster::getName() const {
	return name;
}

int Monster::getAtk() const {
	return atk;
}

int Monster::getDef() const {
	return def;
}

int Monster::getMercy() const {
	return mercy;
}

void Monster::addMercy(int amount) {
	mercy += amount;
	if (mercy < 0) mercy = 0;
	if (mercy > mercy_obj) mercy = mercy_obj;
}

vector<string>& Monster::getActIds() {
	return actIds;
}
