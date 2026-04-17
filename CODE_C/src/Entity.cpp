#include "../include/Entity.h"

Entity::Entity(int hp, int hpMax) : hp(hp), hpMax(hpMax) {}

void Entity::takeDamage(int dmg) {
	hp -= dmg;
	if (hp < 0) hp = 0;
}

void Entity::heal(int amount) {
	hp += amount;
	if (hp > hpMax) hp = hpMax;
}

bool Entity::isAlive() const {
	return hp > 0;
}

int Entity::getHp() const {
	return hp;
}

int Entity::getHpMax() const {
	return hpMax;
}
