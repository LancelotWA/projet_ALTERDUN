#include "../include/Game.h"
#include "../include/NormalMonster.h"
#include "../include/MiniBossMonster.h"
#include "../include/BossMonster.h"

Game::Game() : player(20, 20, "Player") {}

Game::~Game() {
}

void Game::run() {
	loadItems("items.csv");
	loadMonsters("monsters.csv");

	cout << "=== ALTERDUNE ===\n";
	cout << "Entrez votre nom : ";
	string nom;
	cin >> nom;
	player = Player(20, 20, nom);

	int choice = 0;
	do {
		showMenu();
		cin >> choice;

		switch (choice) {
		case 1:
			if (monsters.empty())
				cout << "Aucun monstre restant a combattre.\n";
			else
				startCombat();
			break;
		case 2: showStats();    break;
		case 3: showItems();    break;
		case 4: showBestiary(); break;
		case 5: cout << "Au revoir !\n"; break;
		default: cout << "Choix invalide.\n"; break;
		}

		if (!player.isAlive()) {
			cout << "\n=== GAME OVER ===\n";
			break;
		}

	} while (choice != 5);
}

void Game::showMenu() {
	cout << "\n=== MENU ===\n";
	cout << player.getName() << " | HP : " << player.getHp() << "/" << player.getHpMax() << "\n";
	cout << "Monstres restants : " << monsters.size() << "\n";
	cout << "\n";
	cout << "1. Combattre\n";
	cout << "2. Stats\n";
	cout << "3. Inventaire\n";
	cout << "4. Bestiaire\n";
	cout << "5. Quitter\n";
	cout << "Choix : ";
}

void Game::loadItems(const string& path) {
	ifstream file(path);
	if (!file.is_open()) {
		cout << "Erreur : impossible d'ouvrir " << path << endl;
		return;
	}

	string line;
	getline(file, line); // skip header

	while (getline(file, line)) {
		if (line.empty()) continue;
		istringstream ss(line);
		string nom, typeStr, valStr, qtyStr;

		getline(ss, nom, ',');
		getline(ss, typeStr, ',');
		getline(ss, valStr, ',');
		getline(ss, qtyStr, ',');

		ItemType type;
		bool on_player;
		int value = stoi(valStr);
		int qty = stoi(qtyStr);

		if (typeStr == "HEAL") {
			type = ItemType::HEAL;
			on_player = true;
		} else if (typeStr == "DAMAGE") {
			type = ItemType::ATTACK;
			on_player = false;
			value = -value; // valeur négative pour endommager le monstre via takeDamage(-value)
		} else if (typeStr == "MERCY") {
			type = ItemType::MERCY;
			on_player = false;
		} else {
			continue;
		}

		player.getInventory().push_back(Item(nom, type, value, qty, on_player));
	}
}

void Game::loadMonsters(const string& path) {
	ifstream file(path);
	if (!file.is_open()) {
		cout << "Erreur : impossible d'ouvrir " << path << endl;
		return;
	}

	string line;
	getline(file, line); // skip header

	while (getline(file, line)) {
		if (line.empty()) continue;
		istringstream ss(line);
		string categorie, nom, hpStr, atkStr, defStr, mercyGoalStr;
		string act1, act2, act3, act4;

		getline(ss, categorie, ',');
		getline(ss, nom, ',');
		getline(ss, hpStr, ',');
		getline(ss, atkStr, ',');
		getline(ss, defStr, ',');
		getline(ss, mercyGoalStr, ',');
		getline(ss, act1, ',');
		getline(ss, act2, ',');
		getline(ss, act3, ',');
		getline(ss, act4, ',');

		int hp = stoi(hpStr);
		int atk = stoi(atkStr);
		int def = stoi(defStr);
		int mercyGoal = stoi(mercyGoalStr);

		Monster* m = nullptr;
		if (categorie == "NORMAL")
			m = new NormalMonster(hp, hp, nom, atk, def, mercyGoal);
		else if (categorie == "MINIBOSS")
			m = new MiniBossMonster(hp, hp, nom, atk, def, mercyGoal);
		else if (categorie == "BOSS")
			m = new BossMonster(hp, hp, nom, atk, def, mercyGoal);

		if (m) {
			for (const string& act : {act1, act2, act3, act4}) {
				if (act != "-")
					m->getActIds().push_back(act);
			}
			monsters.push_back(m);
		}
	}
}

void Game::startCombat() {
	if (monsters.empty()) {
		cout << "Aucun monstre disponible.\n";
		return;
	}

	// Choix aleatoire d'un monstre
	static mt19937 rng(random_device{}());
	uniform_int_distribution<int> dist(0, (int)monsters.size() - 1);
	int idx = dist(rng);
	Monster* m = monsters[idx];

	// Reinitialise les PV du monstre pour le combat
	m->heal(m->getHpMax());

	// Table des actions ACT : id -> {texte affiché, impact mercy}
	auto getActAction = [](const string& id) -> ActAction {
		if (id == "COMPLIMENT")  return ActAction(id, "Tu complimentes le monstre chaleureusement.", 34);
		if (id == "DISCUSS")     return ActAction(id, "Tu engages une conversation avec le monstre.", 34);
		if (id == "JOKE")        return ActAction(id, "Tu racontes une blague. Le monstre rit un peu.", 34);
		if (id == "PET")         return ActAction(id, "Tu carresses doucement le monstre.", 34);
		if (id == "DANCE")       return ActAction(id, "Tu danses avec le monstre.", 34);
		if (id == "OBSERVE")     return ActAction(id, "Tu observes attentivement le monstre.", 25);
		if (id == "OFFER_SNACK") return ActAction(id, "Tu offres un snack au monstre.", 40);
		if (id == "REASON")      return ActAction(id, "Tu tentes de raisonner le monstre.", 30);
		if (id == "INSULT")      return ActAction(id, "Tu insules le monstre... ca marche quand meme.", 20);
		return ActAction(id, "Tu agis mysterieusement.", 20);
	};

	cout << "\n=== COMBAT : " << m->getName() << " (" << m->getCategory() << ") ===\n";

	bool combatOver = false;
	FightResult result = FightResult::KILLED;

	while (!combatOver) {
		// Affichage de l'état
		cout << "\n";
		cout << m->getName() << " HP : " << m->getHp() << "/" << m->getHpMax()
			 << " | Mercy : " << m->getMercy() << "/" << m->getMercyObj() << "\n";
		cout << player.getName() << " HP : " << player.getHp() << "/" << player.getHpMax() << "\n";
		cout << "\n> 1. FIGHT   2. ACT   3. ITEM   4. MERCY\n";
		cout << "Choix : ";

		int choice;
		cin >> choice;

		switch (choice) {
		case 1: // FIGHT
			player.fight(*m);
			cout << "Vous attaquez " << m->getName() << " ! (HP restants : " << m->getHp() << ")\n";
			if (!m->isAlive()) {
				cout << m->getName() << " est vaincu !\n";
				result = FightResult::KILLED;
				combatOver = true;
			}
			break;

		case 2: // ACT
		{
			vector<string>& acts = m->getActIds();
			cout << "Choisissez une action :\n";
			for (int i = 0; i < (int)acts.size(); i++)
				cout << "  " << i + 1 << ". " << acts[i] << "\n";
			cout << "Choix : ";
			int actChoice;
			cin >> actChoice;
			if (actChoice >= 1 && actChoice <= (int)acts.size()) {
				ActAction act = getActAction(acts[actChoice - 1]);
				cout << act.getText() << "\n";
				act.execute(*m);
				cout << "(Mercy : " << m->getMercy() << "/" << m->getMercyObj() << ")\n";
			}
			break;
		}

		case 3: // ITEM
		{
			vector<Item>& inv = player.getInventory();
			if (inv.empty()) {
				cout << "Inventaire vide.\n";
				break;
			}
			showItems();
			cout << "Choisissez un item (0 pour annuler) : ";
			int itemChoice;
			cin >> itemChoice;
			if (itemChoice >= 1 && itemChoice <= (int)inv.size()) {
				string itemName = inv[itemChoice - 1].getName();
				player.useItem(itemChoice - 1, *m);
				cout << itemName << " utilise !\n";
			}
			break;
		}

		case 4: // MERCY
			if (player.mercy(*m)) {
				cout << m->getName() << " est epargne !\n";
				result = FightResult::SPARED;
				combatOver = true;
			} else {
				cout << "Le monstre n'est pas encore pret a etre epargne. "
					 << "(Mercy : " << m->getMercy() << "/" << m->getMercyObj() << ")\n";
			}
			break;

		default:
			cout << "Choix invalide.\n";
			break;
		}

		// Attaque du monstre si le combat continue
		if (!combatOver && m->isAlive()) {
			m->attack(player);
			cout << m->getName() << " vous attaque ! Vous perdez " << m->getAtk() << " PV. "
				 << "(HP : " << player.getHp() << "/" << player.getHpMax() << ")\n";
			if (!player.isAlive()) {
				cout << "Vous etes mort. Game over.\n";
				return;
			}
		}
	}

	// Ajout au bestiaire et suppression du monstre de la liste
	bestiary.push_back(BestiaryEntry((int)bestiary.size() + 1, result, m));
	monsters.erase(monsters.begin() + idx);
}

void Game::showBestiary() {
	if (bestiary.empty()) {
		cout << "Bestiaire vide.\n";
		return;
	}
	cout << "=== BESTIAIRE ===\n";
	for (const BestiaryEntry& entry : bestiary)
		entry.display();
}

void Game::showStats() {
	cout << "=== STATS ===\n";
	cout << "Nom    : " << player.getName() << "\n";
	cout << "HP     : " << player.getHp() << " / " << player.getHpMax() << "\n";
}

void Game::showItems() {
	const vector<Item>& inv = player.getInventory();
	if (inv.empty()) {
		cout << "Inventaire vide.\n";
		return;
	}
	cout << "=== INVENTAIRE ===\n";
	for (int i = 0; i < (int)inv.size(); i++) {
		cout << "[" << i + 1 << "] " << inv[i].getName()
			 << " (x" << inv[i].getQuantity() << ")\n";
	}
}
