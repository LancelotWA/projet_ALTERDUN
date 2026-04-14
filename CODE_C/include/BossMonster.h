#pragma once
#include "Monster.h"

class BossMonster : public Monster {
public:
	using Monster::Monster;

	int nbActs() const override;
};
