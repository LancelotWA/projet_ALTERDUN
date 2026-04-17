#pragma once
#include "common.h"

class Monster;

class ActAction {
private:
	string id;
	string text;
	int mercyImpact;

public:
	ActAction(const string& id, const string& text, int mercyImpact);

	void execute(Monster& m);
	string getId() const;
	string getText() const;
	int getMercyImpact() const;
};
