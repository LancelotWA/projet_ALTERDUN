#include "../include/Entity.h"

Entity::Entity(int hp, int hpMax) : hp(hp), hpMax(hpMax) {}

void Entity::takeDamage(int dmg) {
}

void Entity::heal(int amount) {
}

bool Entity::isAlive() const {
	return false;
}

int Entity::getHp() const {
	return 0;
}

int Entity::getHpMax() const {
	return 0;
}
