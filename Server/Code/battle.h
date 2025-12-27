#pragma once

int getZoneIndex(std::string zone) {
	for (int i = 0; i < zoneNames.size(); i++) {
		if (zoneNames[i] == zone) {
			return i;
		}
	}
	return 0;
}

std::string validateBattle(std::string battleId) {
	if (BATTLES.count(battleId) == 0) {
		return "";
	}
	std::string changes = "";
	int numPlayers = 0;
	std::vector<std::string> newCharacters = {};
	for (std::string id : BATTLES[battleId].characters) {
		if (CHARACTERS.count(id) > 0) {
			Character C = CHARACTERS[id];
			if (C.HP <= 0) {
				BATTLES[id].dead.push_back(id);
				changes += removeCharacter(C);
			}
			else if (C.LOCATION == battleId) {
				newCharacters.push_back(id);
				if (C.TYPE == "player") {
					numPlayers++;
				}
			}
		}
	}
	if (numPlayers == 0) {
		std::string filePath = "./Saves/Battles/" + BATTLES[battleId].id + ".txt";
		std::remove(filePath.c_str());
		for (std::string id : newCharacters) {
			changes += removeCharacter(CHARACTERS[id]);
		}
		BATTLES.erase(battleId);
	}
	else {
		BATTLES[battleId].characters = newCharacters;
		if (changes != "") {
			saveBattle(BATTLES[battleId]);
		}
	}
	return changes;
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
	std::string changes = textChange("*GREEN*The enemies are all slain!*GREY*\n");
	changes += makeChange(battle, "ROUND", str(0));

	// Process dead characters
	int lootMax = max(20, battle.difficulty / 3);
	int lootValue = 0;
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
	//changes += textChange("*GREEN*Each player gains " + goldReward + " gold and " + expReward + " experience!\n");

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

			changes += printStat(*C, "GOLD");
			changes += printStat(*C, "XP");
			changes += printStat(*C, "SP");
			changes += printStat(*C, "LEVEL");
			changes += printStat(*C, "AP");
			changes += printStat(*C, "STAMINA");
			changes += printStat(*C, "EFFECTS");
		}
	}

	for (std::string id : dropList) {
		Item newItem(id);
		battle.loot[newItem.index] = newItem;
	}

	battle.dead = {};

	changes += printStat(battle, "LOOT");

	return changes;
}

std::string startTurn(Battle& battle) {
	std::string changes = "";

	battle.turn = !battle.turn;
	changes += printStat(battle, "TURN");
	if (battle.turn == 0) {
		battle.round++;
		changes += printStat(battle, "ROUND");
	}

	std::vector<std::vector<Hazard>> hazards = {};
	for (int i = 0; i < 12; i++) {
		hazards.push_back({});
		for (int j = 0; j < 12; j++) {
			hazards[i].push_back(Hazard(0, j, i));
		}
	}

	if (battle.turn == 0 && battle.round > 0 && battle.hazards.size() > 0) {
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
		changes += printStat(battle, "HAZARDS");
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
				changes += dealDamage(hazard.summoner, C->ID, T_Attack(TRUE_DMG, terrain.damage, terrain.damage)).changes;
			}
			for (Effect effect : terrain.effects) {
				changes += addEffect(C->ID, hazard.summoner, effect.id, effect.turns, effect.stacks);
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
		changes += printStat(*C, "EFFECTS");
		if (C->TYPE == "player") {
			C->STAMINA = min(C->STAMINA + 2 * (1 + C->STATS[END]), MaxStamina(*C));
			int diff = min(C->STAMINA, MaxAP(*C) - C->AP);
			C->AP += diff;
			C->STAMINA -= diff;
			C->ENDED = false;
			changes += printStat(*C, "AP");
			changes += printStat(*C, "STAMINA");
			changes += printStat(*C, "ENDED");
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
			changes += enemyAttack(C->ID, battle);
			C->ENDED = true;
		}
	}
	saveBattle(battle);

	return changes;
}

void handleCombat(std::string id) {
	std::string changes = validateBattle(id);
	if (BATTLES.count(id) == 0 || BATTLES[id].round == 0) {
		return;
	}

	Battle* battle = &BATTLES[id];

	bool battleWon = true;
	for (std::string id : battle->characters) {
		if (CHARACTERS.count(id) > 0 && CHARACTERS[id].TEAM > 0) {
			battleWon = false;
			break;
		}
	}
	if (battleWon) {
		changes += winBattle(*battle);
		saveBattle(*battle);
	}
	else {
		while (BATTLES.count(id) > 0 && turnCompleted(battle->characters, battle->turn)) {
			changes += startTurn(*battle);
			saveBattle(*battle);
			changes += validateBattle(id);
		}
	}
	if (changes != "") {
		sendData(changes);
	}
}

void summon(Battle& battle, Character& enemy, int x = 0, int y = 0, int team = 1) {
	std::string id = enemy.NAME + "." + battle.id + "." + to_str(rand() % 9999);
	enemy.ID = id;
	enemy.X = x;
	enemy.Y = y;
	enemy.LOCATION = battle.id;
	enemy.TEAM = team;
	battle.characters.push_back(id);
	CHARACTERS[id] = enemy;
	//sendCharacter(enemy);
	save(enemy);
}

void summon(Battle& battle, std::vector<Character>& enemyList, int team = 1) {
	for (Character enemy : enemyList) {
		summon(battle, enemy, team);
	}
}

void summon(Battle& battle, std::string name, int team = 1) {
	for (auto enemy : ENEMIES) {
		if (low(enemy.second.NAME) == low(name)) {
			summon(battle, enemy.second, team);
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
	std::string bundle = "";

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
		bundle += addBundle("CHARACTER", serialize(enemy));
	}

	addRiver(battle);
	addTrees(battle);

	bundle += addBundle("BATTLE", serialize(battle));

	saveBattle(battle);
	sendData(bundle);
}