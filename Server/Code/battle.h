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
	for (int i = 0; i < 2; i++) {
		for (int j = BATTLES[id].teams[i].size() - 1; j >= 0; j--) {
			std::string character = BATTLES[id].teams[i][j];
			if (CHARACTERS.count(character) == 0 || CHARACTERS[character].LOCATION != BATTLES[id].id) {
				BATTLES[id].teams[i].erase(BATTLES[id].teams[i].begin() + j);
			}
			else if (CHARACTERS[character].HP <= 0) {
				BATTLES[id].teams[i].erase(BATTLES[id].teams[i].begin() + j);
				if (CHARACTERS[character].HP <= 0) {
					BATTLES[id].dead[i].push_back(CHARACTERS[character].ID);
					removeCharacter(CHARACTERS[character]);
					update = true;
				}
			}
			else if (CHARACTERS[character].TYPE == "player") {
				numCharacters++;
			}
		}
	}
	if (numCharacters == 0) {
		std::string filePath = "./Saves/Battles/" + BATTLES[id].id + ".txt";
		std::remove(filePath.c_str());
		for (int i = 0; i < 2; i++) {
			for (int j = BATTLES[id].teams[i].size() - 1; j >= 0; j--) {
				std::string playerId = BATTLES[id].teams[i][j];
				if (CHARACTERS[playerId].TYPE != "player") {
					removeCharacter(CHARACTERS[playerId]);
				}
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

bool turnCompleted(std::vector<std::string> ids) {
	for (std::string id : ids) {
		if (CHARACTERS[id].HP > 0 && (!CHARACTERS[id].ENDED && CHARACTERS[id].TYPE == "player")) {
			return false;
		}
	}
	return true;
}

std::string winBattle(Battle& battle) {
	std::string msg = "*GREEN*The enemies are all slain!*GREY*\n";
	battle.round = 0;

	// Have dead characters drop some of their gear
	for (std::string id : battle.dead[0]) {
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

	// Give out loot
	int lootMax = max(20, battle.difficulty / 3);
	int lootValue = 0;
	std::vector<std::string> dropList = {};
	for (std::string str : battle.dead[1]) {
		std::string id = split(str, '.')[0];
		for (Drop drop : ENEMIES[id].LOOT) {
			int chance = rand() % 100;
			if (chance < drop.dropChance) {
				UI_Item item = getItem(drop.item);
				dropList.push_back(item.id);
				lootValue += item.cost;
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
	msg = msg + "*GREEN*Each player gains " + goldReward + " gold and " + expReward + " experience!\n";
	for (std::string id : battle.teams[0]) {
		Character* C = &CHARACTERS[id];
		setStat(*C, "GOLD", CHARACTERS[id].GOLD + goldReward);
		C->XP += expReward;
		bool levelled = false;
		while (C->XP > C->LEVEL * 100) {
			C->XP -= C->LEVEL * 100;
			C->SP++;
			C->LEVEL++;
			levelled = true;
		}
		if (levelled) {
			sendStat(id, "SP", C->SP);
			sendStat(id, "LEVEL", C->LEVEL);
		}
		sendStat(id, "XP", C->XP);
	}
	for (std::string id : dropList) {
		Item newItem(id);
		battle.loot[newItem.index] = newItem;
	}

	battle.dead[0] = {};
	battle.dead[1] = {};

	return msg;
}

void startTurn(Battle& battle) {
	std::string msg = "";
	// Handle Effects for team who's starting (battle.turn)
	for (int i = battle.teams[battle.turn].size() - 1; i >= 0; i--) {
		std::string id = battle.teams[battle.turn][i];
		Character* C = &CHARACTERS[id];
		if (C->TYPE == "player") {
			C->STAMINA = min(C->STAMINA + 2 * (1 + C->STATS[END]), MaxStamina(*C));
			int diff = min(C->STAMINA, MaxAP(*C) - C->AP);
			setStat(*C, "AP", C->AP + diff);
			setStat(*C, "STAMINA", C->STAMINA - diff);
			setStat(*C, "ENDED", false);
			for (auto item : C->INVENTORY) {
				if (item.second.equipped) {
					UI_Item rawItem = getItem(item.second.id);
					if (rawItem.type == "weapon") {
						C->INVENTORY[item.first].attacks = rawItem.attacks;
						sendItem(C->ID, item.second);
					}
				}
			}
		}
		else {
			C->AP = 12;
			msg += enemyAttack(i, battle.teams[battle.turn], battle.teams[!battle.turn], battle);
			C->ENDED = true;
		}
	}
	sendText(msg, battleIndices(battle));
}

void handleCombat(std::string id) {
	Battle* battle = &BATTLES[id];
	std::string msg = "";
	if (battle->round == 0) {
		return;
	}
	if (validateBattle(battle->id)) {
		if (battle->teams[1].size() == 0) {
			msg = winBattle(*battle);
			updateBattle(*battle);
		}
		else {
			while (turnCompleted(battle->teams[battle->turn]) && validateBattle(battle->id)) {
				battle->turn = !battle->turn;
				if (battle->turn == 0) {
					battle->round++;
				}
				startTurn(*battle);
			}
		}
	}
	sendText(msg, battleIndices(*battle));
}

void summon(Battle& battle, Character enemy, int x = 0, int y = 0, int team = 1) {
	std::string id = enemy.NAME + "." + battle.id + "." + to_str(rand() % 9999);
	enemy.ID = id;
	enemy.X = x;
	enemy.Y = y;
	enemy.LOCATION = battle.id;
	battle.teams[team].push_back(id);
	CHARACTERS[id] = enemy;
	sendCharacter(enemy);
	save(enemy);
}

void summon(Battle& battle, std::vector<Character> enemyList, int team = 1, bool update = true) {
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

void startBattle(Battle& battle) {
	std::vector<Character> enemies = {};
	battle.round = 1;
	battle.turn = 0;

	int zoneIndex = getZoneIndex(battle.zone);

	float lvl = 0.0;
	int num = 0;

	for (std::string id : battle.teams[0]) {
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

	updateBattle(battle);
}