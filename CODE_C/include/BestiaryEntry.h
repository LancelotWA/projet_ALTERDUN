#pragma once
#include "common.h"

class Monster;

enum class FightResult {
	KILLED,
	SPARED
};

class BestiaryEntry {
private:
	int id_bestiary;
	FightResult fightResult;
	Monster* best_monster;

public:
	BestiaryEntry(int id_bestiary, FightResult fightResult, Monster* best_monster);

	void display() const;
	string getResult() const;
};
