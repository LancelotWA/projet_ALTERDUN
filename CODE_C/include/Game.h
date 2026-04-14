#pragma once
#include <string>
#include <vector>
#include "Player.h"
#include "Monster.h"
#include "BestiaryEntry.h"
using std::string;
using std::vector;

class Game {
private:
	Player player;
	vector<Monster*> monsters;
	vector<BestiaryEntry> bestiary;

public:
	Game();
	~Game();

	void run();
	void showMenu();
	void loadItems(const string& path);
	void loadMonsters(const string& path);
	void startCombat();
	void showBestiary();
	void showStats();
	void showItems();
};
