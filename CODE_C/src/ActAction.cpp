#include "../include/ActAction.h"
#include "../include/Monster.h"

ActAction::ActAction(const string& id, const string& text, int mercyImpact)
	: id(id), text(text), mercyImpact(mercyImpact) {}

void ActAction::execute(Monster& m) {
}

string ActAction::getId() const {
	return "";
}

string ActAction::getText() const {
	return "";
}

int ActAction::getMercyImpact() const {
	return 0;
}
