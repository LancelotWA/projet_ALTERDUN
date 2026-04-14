#pragma once
#include <string>
#include <vector>
#include "Entity.h"
#include "ActAction.h"
using std::string;
using std::vector;

class Player;

class Monster : public Entity {
protected:
	string name;
	int atk;
	int def;
	int mercy;
	int mercy_obj;
	vector<string> actIds;

public:
	Monster(int hp, int hpMax, const string& name, int atk, int def, int mercy_obj);
	virtual ~Monster() = default;

	virtual int nbActs() const = 0;
	void attack(Player& p);
	bool canBeSpared() const;
	string getName() const;
	int getMercy() const;
	vector<string>& getActIds();
};
