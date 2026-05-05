#include "../include/NormalMonster.h"

int NormalMonster::nbActs() const {
	return 2;
}

string NormalMonster::getCategory() const {
	return "NORMAL";
}

Monster* NormalMonster::clone() const {
	return new NormalMonster(*this);
}
