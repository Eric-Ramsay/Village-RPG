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

	if (turnCompleted(battle.teams[battle.turn])) {
		battle.turn = !battle.turn;
		startTurn(battle);
	}

	if (validateBattle(battle.id)) {
		if (battle.teams[1].size() == 0) {
			msg = winBattle(battle);
		}
		updateBattle(battle);
	}
	return msg;
}

void startBattle(Battle& battle) {
	battle.round = 1;
	battle.turn = 0;

	Character wolf;
	wolf.NAME = "crazed wolf";
	wolf.TYPE = "enemy";
	wolf.ID = "crazed wolf " + battle.id + " " + to_str(rand() % 9999);
	wolf.HP = 15;
	wolf.MaxHP = 15;
	wolf.AP = 12;
	wolf.LOCATION = battle.id;

	CHARACTERS[wolf.ID] = wolf;
	battle.teams[1].push_back(wolf.ID);

	startTurn(battle);

	save(wolf);
	sendCharacter(wolf);
	updateBattle(battle);
}