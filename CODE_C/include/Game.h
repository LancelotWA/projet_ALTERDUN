#pragma once
#include "common.h"
#include "Player.h"
#include "Monster.h"
#include "BestiaryEntry.h"
#include "ActAction.h"

class Game {
private:
	Player player;
	vector<Monster*> monsters;
	vector<BestiaryEntry> bestiary;
	map<string, ActAction> actCatalogue;

	static const int WIN_CONDITION = 2;

public:
	Game();
	~Game();

	void run();
	void showMenu();
	bool loadItems(const string& path, int& warnings);
	bool loadMonsters(const string& path, int& warnings);
	void startCombat();
	void showBestiary();
	void showStats();
	void showItems();
	void showEnding();
};
