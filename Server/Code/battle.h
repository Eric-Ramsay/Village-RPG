#pragma once

bool validateBattle(std::string id) {
	if (BATTLES.count(id) == 0) {
		return false;
	}
	int numCharacters = 0;
	for (int i = 0; i < 2; i++) {
		for (int j = BATTLES[id].teams[i].size() - 1; j >= 0; j--) {
			std::string character = BATTLES[id].teams[i][j];
			if (CHARACTERS.count(character) == 0 || CHARACTERS[character].LOCATION != BATTLES[id].id || CHARACTERS[character].HP <= 0) {
				BATTLES[id].teams[i].erase(BATTLES[id].teams[i].begin() + j);
				if (CHARACTERS[character].HP <= 0) {
					removeCharacter(CHARACTERS[character]);
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
		BATTLES.erase(id);
		return false;
	}
	return true;
}

bool turnCompleted(std::vector<std::string> ids) {
	for (std::string id : ids) {
		if (CHARACTERS[id].HP > 0 && !CHARACTERS[id].ENDED) {
			return false;
		}
	}
	return true;
}

std::string winBattle(Battle& battle) {
	std::string msg = "*GREEN*The enemies are all slain!*GREY*\n";
	battle.round = 0;

	// Have dead characters drop some of their gear

	// Give out loot, gold, XP

	return msg;
}

std::string startTurn(Battle& battle) {
	std::string msg = "";
	// Handle Effects for team who's starting (battle.turn)
	for (int i = battle.teams[battle.turn].size() - 1; i >= 0; i--) {
		std::string id = battle.teams[battle.turn][i];
		Character* C = &CHARACTERS[id];
		if (C->TYPE == "player") {
			setStat(*C, "AP", 12);
			setStat(*C, "ENDED", false);
		}
		else {
			C->AP = 12;
			msg += enemyAttack(i, battle.teams[battle.turn], battle.teams[!battle.turn], battle);
			C->ENDED = true;
		}
	}
	return msg;
}

std::string handleCombat(Battle& battle) {
	std::string msg = "";
	if (battle.round == 0) {
		return "";
	}
	int maxTurns = 0;
	while (maxTurns++ < 10 && turnCompleted(battle.teams[battle.turn])) {
		battle.turn = !battle.turn;
		startTurn(battle);

		if (validateBattle(battle.id)) {
			if (battle.teams[1].size() == 0) {
				msg = winBattle(battle);
			}
			updateBattle(battle);
		}
	}
	return msg;
}

void summon(Battle& battle, Character enemy, int team = 1) {
	std::string id = enemy.NAME + " " + battle.id + " " + to_str(rand() % 9999);
	enemy.ID = id;
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
	for (Character enemy : ENEMIES) {
		if (low(enemy.NAME) == low(name)) {
			summon(battle, enemy, team);
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

	float lvl = 0.0;
	int num = 0;

	for (std::string id : battle.teams[0]) {
		if (CHARACTERS[id].TYPE == "player") {
			num++;
			lvl += CHARACTERS[id].LEVEL;
		}
	}

	lvl /= (float)num;
	int rating = 45 * num * std::pow(1.22, lvl) + (15 * (lvl - 1) * num);

	std::vector<Character> validEnemies = {};
	for (Character enemy : ENEMIES) {
		if (enemy.LEVEL < rating && contains(enemy.ZONES, battle.zone)) {
			validEnemies.push_back(enemy);
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

	summon(battle, enemies, 1, false);

	startTurn(battle);

	updateBattle(battle);
}