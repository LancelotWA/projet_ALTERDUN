#include "../include/BestiaryEntry.h"
#include "../include/Monster.h"

BestiaryEntry::BestiaryEntry(int id_bestiary, FightResult fightResult, Monster* best_monster)
	: id_bestiary(id_bestiary), fightResult(fightResult), best_monster(best_monster->clone()) {}

BestiaryEntry::BestiaryEntry(const BestiaryEntry& other)
	: id_bestiary(other.id_bestiary), fightResult(other.fightResult), best_monster(other.best_monster->clone()) {}

BestiaryEntry& BestiaryEntry::operator=(const BestiaryEntry& other) {
	if (this != &other) {
		delete best_monster;
		id_bestiary = other.id_bestiary;
		fightResult = other.fightResult;
		best_monster = other.best_monster->clone();
	}
	return *this;
}

BestiaryEntry::~BestiaryEntry() {
	delete best_monster;
}

void BestiaryEntry::display() const {
	cout << "[" << id_bestiary << "] " << best_monster->getName()
		 << " (" << best_monster->getCategory() << ")"
		 << " | HP max: " << best_monster->getHpMax()
		 << " | ATK: " << best_monster->getAtk()
		 << " | DEF: " << best_monster->getDef()
		 << " | Resultat: " << getResult()
		 << "\n";
}

string BestiaryEntry::getResult() const {
	return (fightResult == FightResult::KILLED) ? "Tue" : "Epargne";
}
