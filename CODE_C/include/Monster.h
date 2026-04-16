#pragma once
#include "common.h"
#include "Entity.h"
#include "ActAction.h"

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
	virtual string getCategory() const = 0;
	void attack(Player& p);
	bool canBeSpared() const;
	string getName() const;
	int getAtk() const;
	int getDef() const;
	int getMercy() const;
	void addMercy(int amount);
	vector<string>& getActIds();
};
