#pragma once

int getZoneIndex(std::string zone) {
	for (int i = 0; i < zoneNames.size(); i++) {
		if (zoneNames[i] == zone) {
			return i;
		}
	}
	return 0;
}

bool validateBattle(std::string id) {
	if (BATTLES.count(id) == 0) {
		return false;
	}
	bool update = false;
	int numCharacters = 0;
	for (int i = BATTLES[id].characters.size() - 1; i >= 0; i--) {
		std::string character = BATTLES[id].characters[i];
		if (CHARACTERS.count(character) == 0 || CHARACTERS[character].LOCATION != id) {
			BATTLES[id].characters.erase(BATTLES[id].characters.begin() + i);
		}
		else if (CHARACTERS[character].HP <= 0) {
			BATTLES[id].characters.erase(BATTLES[id].characters.begin() + i);
			BATTLES[id].dead.push_back(character);
			removeCharacter(CHARACTERS[character]);
			update = true;
		}
		else if (CHARACTERS[character].TYPE == "player") {
			numCharacters++;
		}
	}
	if (numCharacters == 0) {
		std::string filePath = "./Saves/Battles/" + BATTLES[id].id + ".txt";
		std::remove(filePath.c_str());
		for (int i = BATTLES[id].characters.size() - 1; i >= 0; i--) {
			std::string playerId = BATTLES[id].characters[i];
			if (CHARACTERS[playerId].TYPE != "player") {
				removeCharacter(CHARACTERS[playerId]);
			}
		}
		BATTLES.erase(id);
		return false;
	}
	else if (update) {
		updateBattle(BATTLES[id]);
	}
	return true;
}

bool turnCompleted(std::vector<std::string> ids, int turn) {
	for (std::string id : ids) {
		Character C = CHARACTERS[id];
		if (C.TEAM == turn) {
			if (C.HP > 0 && (!C.ENDED && C.TYPE == "player")) {
				return false;
			}
		}
	}
	return true;
}

std::string winBattle(Battle& battle) {
	std::string msg = "*GREEN*The enemies are all slain!*GREY*\n";
	battle.round = 0;

	// Process dead characters
	int lootMax = max(20, battle.difficulty / 3);
	int lootValue = 0;
	lootMax = 0;
	std::vector<std::string> dropList = {};
	for (std::string id : battle.dead) {
		std::vector<std::string> splitId = split(id, '.');
		if (splitId.size() == 3) {
			// Dead Enemy - Give out Loot
			std::string id = splitId[0];
			for (Drop drop : ENEMIES[id].LOOT) {
				int chance = rand() % 100;
				if (chance < drop.dropChance) {
					UI_Item item = getItem(drop.item);
					dropList.push_back(item.id);
					lootValue += item.cost;
				}
			}
		}
		else {
			// Dead Player - Drop Gear
			std::string path = "./Saves/Characters/Graveyard/" + id;
			Character character = load<Character>(path);
			for (auto item : character.INVENTORY) {
				int dropChance = 20;
				if (item.second.equipped) {
					dropChance *= 2;
				}
				if (rand() % 100 < dropChance) {
					battle.loot[item.second.index] = item.second;
				}
			}
		}
	}

	for (Drop drop : zoneLoot[getZoneIndex(battle.zone)]) {
		int chance = rand() % 100;
		if (chance < drop.dropChance) {
			UI_Item item = getItem(drop.item);
			dropList.push_back(item.id);
			lootValue += item.cost;
		}
	}

	while (lootValue < lootMax) {
		int index = rand() % genericLoot.size();
		dropList.push_back(genericLoot[index].id);
		lootValue += genericLoot[index].cost;
	}

	// Give out gold and XP
	int goldReward = min(200, 10 + (battle.difficulty / 20));
	int expReward = battle.difficulty;
	goldReward = 0;
	expReward = 0;
	msg = msg + "*GREEN*Each player gains " + goldReward + " gold and " + expReward + " experience!\n";

	std::string bundle = "";
	for (std::string id : battle.characters) {
		Character* C = &CHARACTERS[id];
		if (C->TYPE == "player") {
			C->GOLD += goldReward;
			C->XP += expReward;
			while (C->XP > C->LEVEL * 100) {
				C->XP -= C->LEVEL * 100;
				C->SP++;
				C->LEVEL++;
			}
			C->AP = MaxAP(*C);
			C->STAMINA = MaxStamina(*C);
			C->EFFECTS = { };
			std::string changes = str(C->ID);
			changes += addLine("GOLD", C->GOLD);
			changes += addLine("XP", C->XP);
			changes += addLine("SP", C->SP);
			changes += addLine("LEVEL", C->LEVEL);
			changes += addLine("AP", C->AP);
			changes += addLine("STAMINA", C->STAMINA);
			changes += "EFFECTS!!!" + serialize(C->EFFECTS) + "\n";
			bundle += changes + '\t';
		}
	}

	for (std::string id : dropList) {
		Item newItem(id);
		battle.loot[newItem.index] = newItem;
	}
	sendData("BUNDLE", bundle);

	battle.dead = {};

	return msg;
}

void startTurn(Battle& battle) {
	std::string msg = "";

	std::vector<std::vector<Hazard>> hazards = {};
	for (int i = 0; i < 12; i++) {
		hazards.push_back({});
		for (int j = 0; j < 12; j++) {
			hazards[i].push_back(Hazard(0, j, i));
		}
	}

	if (battle.turn == 0 && battle.round > 0) {
		for (int i = battle.hazards.size() - 1; i >= 0; i--) {
			if (battle.hazards[i].duration > 0) {
				hazards[battle.hazards[i].y][battle.hazards[i].x] = battle.hazards[i];
			}
			if (battle.hazards[i].duration < 999) {
				battle.hazards[i].duration--;
				if (battle.hazards[i].duration < 0) {
					battle.hazards.erase(battle.hazards.begin() + i);
				}
			}
		}
	}

	// Handle Effects for team who's starting (battle.turn)
	for (int i = battle.characters.size() - 1; i >= 0; i--) {
		Character* C = &CHARACTERS[battle.characters[i]];
		if (C->TEAM != battle.turn) {
			continue;
		}
		if (C->TYPE == "player") {
			Hazard hazard = hazards[C->Y][C->X];
			Terrain terrain = TERRAIN[hazard.index];
			if (terrain.damage > 0) {
				msg += dealDamage(Attack(TRUE_DMG, terrain.damage, terrain.damage, 100, 100), hazard.summoner, C->ID, battle.characters).msg;
			}
			for (Effect effect : terrain.effects) {
				msg += addEffect(C->ID, hazard.summoner, effect.id, effect.turns, effect.stacks);
			}
		}

		for (int i = C->EFFECTS.size() - 1; i >= 0; i--) {
			Effect* effect = &C->EFFECTS[i];
			effect->turns--;
			if (effect->turns > -1) {
				if (effect->id == "poisoned" && C->HP > 1) {
					C->HP = std::max(1, C->HP - effect->stacks);
				}
			}
			else {
				C->EFFECTS.erase(C->EFFECTS.begin() + i);
			}
		}
		if (C->TYPE == "player") {
			C->STAMINA = min(C->STAMINA + 2 * (1 + C->STATS[END]), MaxStamina(*C));
			int diff = min(C->STAMINA, MaxAP(*C) - C->AP);
			C->AP += diff;
			C->STAMINA -= diff;
			C->ENDED = false;
			for (auto item : C->INVENTORY) {
				if (item.second.equipped) {
					UI_Item rawItem = getItem(item.second.id);
					if (rawItem.type == "weapon") {
						C->INVENTORY[item.first].attacks = rawItem.attacks;
					}
				}
			}
		}
		else {
			C->AP = 12;
			msg += enemyAttack(C->ID, battle);
			C->ENDED = true;
		}
	}
	updateBattle(battle);
	sendText(msg, battleIndices(battle));
}

void handleCombat(std::string id) {
	Battle* battle = &BATTLES[id];
	std::string msg = "";
	if (battle->round == 0) {
		return;
	}
	if (validateBattle(id)) {
		bool battleWon = true;
		for (std::string id : battle->characters) {
			if (CHARACTERS[id].TEAM > 0) {
				battleWon = false;
				break;
			}
		}
		if (battleWon) {
			msg = winBattle(*battle);
			updateBattle(*battle);
		}
		else {
			while (turnCompleted(battle->characters, battle->turn) && validateBattle(id)) {
				battle->turn = !battle->turn;
				if (battle->turn == 0) {
					battle->round++;
				}
				startTurn(*battle);
			}
		}
	}
	if (BATTLES.count(id)) {
		sendText(msg, battleIndices(*battle));
	}
}

void summon(Battle& battle, Character enemy, int x = 0, int y = 0, int team = 1) {
	std::string id = enemy.NAME + "." + battle.id + "." + to_str(rand() % 9999);
	enemy.ID = id;
	enemy.X = x;
	enemy.Y = y;
	enemy.LOCATION = battle.id;
	enemy.TEAM = team;
	battle.characters.push_back(id);
	CHARACTERS[id] = enemy;
	sendCharacter(enemy);
	save(enemy);
}

void summon(Battle& battle, std::vector<Character> enemyList, int team = 1, bool update = false) {
	for (Character enemy : enemyList) {
		summon(battle, enemy, team);
	}
	if (update) {
		updateBattle(battle);
	}
}

void summon(Battle& battle, std::string name, int team = 1) {
	for (auto enemy : ENEMIES) {
		if (low(enemy.second.NAME) == low(name)) {
			summon(battle, enemy.second, team);
			updateBattle(battle);
			return;
		}
	}
	std::cout << "Couldn't summon enemy '" + name + "'!" << std::endl;
}

void addRiver(Battle& battle) {
	// Make a river:
	int x = 0;
	int y = 0;
	int bannedDir = 0;
	int lastDir = 0;
	int dir = 0;
	// 0 - Up
	// 1 - Left
	// 2 - Down
	// 3 - Right
	battle.hazards = {};
	if (rand() % 2 == 0) {
		x = 11;
		y = 1 + rand() % 10;
		bannedDir = 3;
		dir = 1;
	}
	else {
		x = 1 + rand() % 10;
		y = 11;
		bannedDir = 2;
		dir = 0;
	}
	bool run = true;
	while (run) {
		for (int i = 0; i < 2; i++) {
			if (run) {
				battle.hazards.push_back(Hazard(5, x, y));
			}
			lastDir = (dir + 2) % 4;
			if (dir == 0) {
				y--;
			}
			if (dir == 1) {
				x--;
			}
			if (dir == 2) {
				y++;
			}
			if (dir == 3) {
				x++;
			}
			run = (x >= 0 && y >= 0 & x < 12 && y < 12);
		}
		do {
			dir = rand() % 4;
			std::cout << dir << std::endl;
		} while (dir == lastDir || dir == bannedDir);
	}
}

void addTrees(Battle& battle) {
	int numTrees = 6;
	while (numTrees > 0) {
		int treeIndex = 1 + rand() % 3;
		int x = rand() % 12;
		int y = rand() % 12;
		bool found = false;
		for (Hazard hazard : battle.hazards) {
			if (hazard.x == x && hazard.y == y) {
				found = true;
				break;	
			}
		}
		if (!found) {
			battle.hazards.push_back(Hazard(treeIndex, x, y, 3));
			numTrees--;
		}
	}
}

void startBattle(Battle& battle) {
	std::vector<Character> enemies = {};
	battle.round = 1;
	battle.turn = 0;

	int zoneIndex = getZoneIndex(battle.zone);

	float lvl = 0.0;
	int num = 0;

	for (std::string id : battle.characters) {
		if (CHARACTERS[id].TYPE == "player") {
			num++;
			lvl += CHARACTERS[id].LEVEL;
			CHARACTERS[id].STAMINA = MaxStamina(CHARACTERS[id]);
			CHARACTERS[id].AP = MaxAP(CHARACTERS[id]);
		}
	}

	lvl /= (float)num;
	int rating = 45 * num * std::pow(1.22, lvl) + (15 * (lvl - 1) * num);
	battle.difficulty = rating;

	std::vector<Character> validEnemies = {};
	for (auto enemy : ENEMIES) {
		if (enemy.second.LEVEL < rating && contains(enemy.second.ZONES, zoneIndex)) {
			validEnemies.push_back(enemy.second);
		}
	}

	while (validEnemies.size() > 0 && rating > 0) {
		int index = rand() % validEnemies.size();

		enemies.push_back(validEnemies[index]);
		rating -= validEnemies[index].LEVEL;

		std::vector<Character> newTeams = {};
		for (Character enemy : validEnemies) {
			if (enemy.LEVEL <= rating) {
				newTeams.push_back(enemy);
			}
		}
		validEnemies = newTeams;
	}

	for (Character enemy : enemies) {
		int x = rand() % 12;
		int y = rand() % 2;
		summon(battle, enemy, x, y);
	}

	addRiver(battle);
	addTrees(battle);

	updateBattle(battle);
}