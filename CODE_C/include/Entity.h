#pragma once
#include "common.h"

class Entity {
protected:
	int hp;
	int hpMax;

public:
	Entity(int hp, int hpMax);
	virtual ~Entity() = default;

	void takeDamage(int dmg);
	void heal(int amount);
	bool isAlive() const;
	int getHp() const;
	int getHpMax() const;
};
