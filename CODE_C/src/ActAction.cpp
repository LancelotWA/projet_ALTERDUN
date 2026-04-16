#include "../include/ActAction.h"
#include "../include/Monster.h"

ActAction::ActAction(const string& id, const string& text, int mercyImpact)
	: id(id), text(text), mercyImpact(mercyImpact) {}

void ActAction::execute(Monster& m) {
	m.addMercy(mercyImpact);
}

string ActAction::getId() const {
	return id;
}

string ActAction::getText() const {
	return text;
}

int ActAction::getMercyImpact() const {
	return mercyImpact;
}
