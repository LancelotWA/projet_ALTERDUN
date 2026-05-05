#include "../include/MiniBossMonster.h"

int MiniBossMonster::nbActs() const {
	return 3;
}

string MiniBossMonster::getCategory() const {
	return "MINIBOSS";
}

Monster* MiniBossMonster::clone() const {
	return new MiniBossMonster(*this);
}
