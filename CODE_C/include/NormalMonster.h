#pragma once
#include "Monster.h"

class NormalMonster : public Monster {
public:
	using Monster::Monster;

	int nbActs() const override;
	string getCategory() const override;
};
