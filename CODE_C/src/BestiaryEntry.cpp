#include "../include/BestiaryEntry.h"
#include "../include/Monster.h"

BestiaryEntry::BestiaryEntry(int id_bestiary, FightResult fightResult, Monster* best_monster)
	: id_bestiary(id_bestiary), fightResult(fightResult), best_monster(best_monster) {}

void BestiaryEntry::display() const {
}

string BestiaryEntry::getResult() const {
	return "";
}
