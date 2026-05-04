#include "../include/Game.h"
#include "../include/NormalMonster.h"
#include "../include/MiniBossMonster.h"
#include "../include/BossMonster.h"
using namespace std;

static int readInt() {
	int val;
	while (!(cin >> val)) {
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Entree invalide, entrez un nombre : ";
	}
	return val;
}

Game::Game() : player(100, 100, "Player") {
	actCatalogue = {
		{ "COMPLIMENT",  ActAction("COMPLIMENT",  "Tu complimentes le monstre chaleureusement.",         34) },
		{ "DISCUSS",     ActAction("DISCUSS",     "Tu engages une conversation avec le monstre.",        34) },
		{ "JOKE",        ActAction("JOKE",        "Tu racontes une blague. Le monstre rit un peu.",      34) },
		{ "PET",         ActAction("PET",         "Tu carresses doucement le monstre.",                  34) },
		{ "DANCE",       ActAction("DANCE",       "Tu danses avec le monstre.",                          34) },
		{ "OBSERVE",     ActAction("OBSERVE",     "Tu observes attentivement le monstre.",               25) },
		{ "OFFER_SNACK", ActAction("OFFER_SNACK", "Tu offres un snack au monstre.",                      40) },
		{ "REASON",      ActAction("REASON",      "Tu tentes de raisonner le monstre.",                  30) },
		{ "INSULT",      ActAction("INSULT",      "Tu insules le monstre. Il est perturbe.",            -20) },
		{ "TAUNT",       ActAction("TAUNT",       "Tu nargues le monstre. Il devient furieux.",         -30) },
	};
}

Game::~Game() {
	for (Monster* m : monsters)
		delete m;
}

void Game::run() {
	int warnings = 0;
	if (!loadMonsters("monsters.csv", warnings)) {
		cout << "\nAppuyez sur Entree pour quitter..."; cin.get();
		return;
	}

	cout << "=== ALTERDUNE ===\n";
	cout << "Entrez votre nom : ";
	string nom;
	cin >> nom;
	player = Player(100, 100, nom);

	if (!loadItems("items.csv", warnings)) {
		cout << "\nAppuyez sur Entree pour quitter..."; cin.get();
		return;
	}

	cout << "\n--- Bienvenue, " << player.getName() << " ! ---\n";
	cout << "HP : " << player.getHp() << "/" << player.getHpMax() << "\n";
	cout << "Inventaire de depart :\n";
	for (const Item& item : player.getInventory())
		cout << "  - " << item.getName() << " x" << item.getQuantity() << "\n";
	if (warnings > 0)
		cout << "\n[!] " << warnings << " ligne(s) ignoree(s) lors du chargement des fichiers (format invalide).\n";
	cout << "\nAppuyez sur Entree pour continuer..."; cin.ignore(); cin.get();

	int choice = 0;
	do {
		showMenu();
		choice = readInt();

		switch (choice) {
		case 1:
			if (monsters.empty())
				cout << "Aucun monstre restant a combattre.\n";
			else
				startCombat();
			break;
		case 2: showStats();    cout << "\nAppuyez sur Entree pour continuer..."; cin.ignore(); cin.get(); break;
		case 3:
		{
			showItems();
			vector<Item>& inv = player.getInventory();
			cout << "Utiliser un item ? (numero, 0 pour annuler) : ";
			int itemChoice = readInt();
			if (itemChoice >= 1 && itemChoice <= (int)inv.size()) {
				if (!inv[itemChoice - 1].isOnPlayer()) {
					cout << "Cet item ne peut etre utilise qu'en combat.\n";
				} else if (inv[itemChoice - 1].getQuantity() <= 0) {
					cout << "Plus de stock.\n";
				} else {
					string itemName = inv[itemChoice - 1].getName();
					int hpBefore = player.getHp();
					int val = inv[itemChoice - 1].getValue();
					if (val >= 0) player.heal(val);
					else player.takeDamage(-val);
					inv[itemChoice - 1].decrement();
					cout << itemName << " utilise ! HP : " << hpBefore << " -> " << player.getHp() << "\n";
				}
			}
			cout << "\nAppuyez sur Entree pour continuer..."; cin.ignore(); cin.get();
			break;
		}
		case 4: showBestiary(); cout << "\nAppuyez sur Entree pour continuer..."; cin.ignore(); cin.get(); break;
		case 5: cout << "Au revoir !\n"; break;
		default: cout << "Choix invalide.\n"; break;
		}

		if (!player.isAlive()) {
			cout << "\n=== GAME OVER ===\n";
			cout << "T'etais pas l'elu, retourne a ta chaumiere.\n";
			break;
		}

		if ((int)bestiary.size() >= WIN_CONDITION) {
			showEnding();
			break;
		}

	} while (choice != 5);
}

void Game::showMenu() {
	system("cls");
	cout << "\n=== MENU ===\n";
	cout << player.getName() << " | HP : " << player.getHp() << "/" << player.getHpMax() << "\n";
	cout << "Victoires : " << bestiary.size() << "/" << WIN_CONDITION << "\n";
	cout << "\n";
	cout << "1. Combattre\n";
	cout << "2. Stats\n";
	cout << "3. Inventaire\n";
	cout << "4. Bestiaire\n";
	cout << "5. Quitter\n";
	cout << "Choix : ";
}

bool Game::loadItems(const string& path, int& warnings) {
	ifstream file(path);
	if (!file.is_open()) {
		cout << "\n=== ERREUR : fichier introuvable ===\n";
		cout << "Impossible d'ouvrir \"" << path << "\".\n";
		cout << "Verifiez que le fichier est present a cote de l'executable.\n";
		return false;
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

		try {
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
				value = -value;
			} else if (typeStr == "MERCY") {
				type = ItemType::MERCY;
				on_player = false;
			} else {
				continue;
			}

			player.getInventory().push_back(Item(nom, type, value, qty, on_player));
		} catch (...) {
			warnings++;
		}
	}
	return true;
}

bool Game::loadMonsters(const string& path, int& warnings) {
	ifstream file(path);
	if (!file.is_open()) {
		cout << "\n=== ERREUR : fichier introuvable ===\n";
		cout << "Impossible d'ouvrir \"" << path << "\".\n";
		cout << "Verifiez que le fichier est present a cote de l'executable.\n";
		return false;
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

		try {
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
			} else {
				warnings++;
			}
		} catch (...) {
			warnings++;
		}
	}
	if (monsters.empty()) {
		cout << "\n=== ERREUR : aucun monstre valide charge ===\n";
		cout << "Verifiez le contenu de \"" << path << "\".\n";
		return false;
	}
	return true;
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

	// Reinitialise les PV et la Mercy du monstre pour le combat
	m->heal(m->getHpMax());
	m->resetMercy();

	auto getActAction = [&](const string& id) -> ActAction {
		auto it = actCatalogue.find(id);
		if (it != actCatalogue.end()) return it->second;
		return ActAction(id, "Tu agis mysterieusement.", 0);
	};

	bool combatOver = false;
	FightResult result = FightResult::KILLED;

	while (!combatOver) {
		// Affichage de l'état
		system("cls");
		cout << "\n=== COMBAT : " << m->getName() << " (" << m->getCategory() << ") ===\n";
		cout << m->getName() << " HP : " << m->getHp() << "/" << m->getHpMax()
			 << " | Mercy : " << m->getMercy() << "/" << m->getMercyObj() << "\n";
		cout << player.getName() << " HP : " << player.getHp() << "/" << player.getHpMax() << "\n";
		cout << "\n> 1. FIGHT   2. ACT   3. ITEM   4. MERCY\n";
		cout << "Choix : ";

		int choice = readInt();

		bool playerActed = false;
		switch (choice) {
		case 1: // FIGHT
		{
			playerActed = true;
			int dmg = player.fight(*m);
			if (dmg == 0)
				cout << "Vous attaquez " << m->getName() << " mais vous ratez !\n";
			else
				cout << "Vous attaquez " << m->getName() << " et infligez " << dmg << " degats ! (HP restants : " << m->getHp() << ")\n";
			if (!m->isAlive()) {
				cout << m->getName() << " est vaincu !\n";
				result = FightResult::KILLED;
				combatOver = true;
			}
			break;
		}

		case 2: // ACT
		{
			vector<string>& acts = m->getActIds();
			int maxActs = min(m->nbActs(), (int)acts.size());
			cout << "Choisissez une action (0 pour annuler) :\n";
			for (int i = 0; i < maxActs; i++)
				cout << "  " << i + 1 << ". " << acts[i] << "\n";
			cout << "Choix : ";
			int actChoice = readInt();
			if (actChoice == 0) {
				cout << "Action annulee.\n";
			} else if (actChoice >= 1 && actChoice <= maxActs) {
				playerActed = true;
				ActAction act = getActAction(acts[actChoice - 1]);
				cout << act.getText() << "\n";
				act.execute(*m);
				cout << "(Mercy : " << m->getMercy() << "/" << m->getMercyObj() << ")\n";
			} else {
				cout << "Choix invalide.\n";
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
			int itemChoice = readInt();
			if (itemChoice >= 1 && itemChoice <= (int)inv.size()) {
				playerActed = true;
				string itemName = inv[itemChoice - 1].getName();
				player.useItem(itemChoice - 1, *m);
				cout << itemName << " utilise !\n";
				if (!m->isAlive()) {
					cout << m->getName() << " est vaincu !\n";
					result = FightResult::KILLED;
					combatOver = true;
				}
			}
			break;
		}

		case 4: // MERCY
			playerActed = true;
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
		if (playerActed && !combatOver && m->isAlive()) {
			int dmg = m->attack(player);
			if (dmg == 0)
				cout << m->getName() << " vous attaque mais rate !\n";
			else
				cout << m->getName() << " vous attaque et inflige " << dmg << " degats ! "
					 << "(HP : " << player.getHp() << "/" << player.getHpMax() << ")\n";
			if (!player.isAlive()) {
				cout << "Vous etes mort. Game over.\n";
				cout << "\nAppuyez sur Entree pour continuer..."; cin.ignore(); cin.get();
				return;
			}
		}

		cout << "\nAppuyez sur Entree pour continuer..."; cin.ignore(); cin.get();
	}

	// Ajout au bestiaire
	bestiary.push_back(BestiaryEntry((int)bestiary.size() + 1, result, m));
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
	int killed = 0, spared = 0;
	for (const BestiaryEntry& e : bestiary) {
		if (e.getResult() == "Tue") killed++;
		else spared++;
	}

	cout << "=== STATS ===\n";
	cout << "Nom       : " << player.getName() << "\n";
	cout << "HP        : " << player.getHp() << " / " << player.getHpMax() << "\n";
	cout << "Tues      : " << killed << "\n";
	cout << "Epargnes  : " << spared << "\n";
	cout << "Victoires : " << bestiary.size() << " / " << WIN_CONDITION << "\n";
}

void Game::showEnding() {
	int killed = 0, spared = 0;
	for (const BestiaryEntry& e : bestiary) {
		if (e.getResult() == "Tue") killed++;
		else spared++;
	}

	cout << "\n";
	if (killed == WIN_CONDITION) {
		cout << "=== FIN GENOCIDAIRE ===\n";
		cout << "Vous avez tue tous les monstres sans pitie.\n";
		cout << "Le monde souterrain est desormais vide et silencieux.\n";
	} else if (spared == WIN_CONDITION) {
		cout << "=== FIN PACIFISTE ===\n";
		cout << "Vous avez epargne chaque monstre rencontre.\n";
		cout << "Les monstres vous ouvrent les bras. La paix regne.\n";
	} else {
		cout << "=== FIN NEUTRE ===\n";
		cout << "Vous avez vaincu " << WIN_CONDITION << " monstres (" << killed << " tues, " << spared << " epargnes).\n";
		cout << "Votre chemin reste ambigu. Le monde vous regarde, incertain.\n";
	}
}

void Game::showItems() {
	const vector<Item>& inv = player.getInventory();
	if (inv.empty()) {
		cout << "Inventaire vide.\n";
		return;
	}
	cout << "=== INVENTAIRE ===\n";
	for (int i = 0; i < (int)inv.size(); i++) {
		string typeLabel;
		int val = inv[i].getValue();
		switch (inv[i].getType()) {
			case ItemType::HEAL:   typeLabel = "Soin +" + to_string(val) + " HP";     break;
			case ItemType::ATTACK: typeLabel = "Degat " + to_string(-val) + " HP";    break;
			case ItemType::MERCY:  typeLabel = "Grace";                                break;
			default:               typeLabel = "?";                                    break;
		}
		cout << "[" << i + 1 << "] " << inv[i].getName()
			 << " (" << typeLabel << ") x" << inv[i].getQuantity() << "\n";
	}
}
