#pragma once\

std::string Name(Character C) {
	if (C.TYPE != "player") {
		return "the " + C.NAME;
	}
	return pretty(C.NAME);
}

std::vector<std::string> findTargets(int x, int y, int range, std::vector<std::string> potentialTargets) {
	std::vector<std::string> targets = {};
	for (std::string id : potentialTargets) {
		if (atkDist(x, y, CHARACTERS[id].X, CHARACTERS[id].Y) <= range) {
			targets.push_back(id);
		}
	}
	return targets;
}

void moveInRange(Character* C, std::vector<std::string> enemies, int range, std::vector<std::vector<int>> movementCosts, bool retreat = false) {
	float minDist = 9999;
	int minX = C->X;
	int minY = C->Y;
	for (std::string id : enemies) {
		int x = CHARACTERS[id].X;
		int y = CHARACTERS[id].Y;
		int d = atkDist(C->X, C->Y, x, y);
		if (d < minDist) {
			minDist = d;
			minX = x;
			minY = y;
		}
	}

	int startDist = atkDist(minX, minY, C->X, C->Y);
	if ((!retreat && startDist < range) || startDist == range) {
		return;
	}

	int newX = C->X;
	int newY = C->Y;
	minDist = 999;
	int moveCost = 999;
	for (int i = 0; i < movementCosts.size(); i++) {
		for (int j = 0; j < movementCosts.size(); j++) {
			if (movementCosts[i][j] <= C->AP) {
				int d = abs(atkDist(minX, minY, j, i) - range);
				if (d < minDist || (d == minDist && movementCosts[i][j] < moveCost)) {
					moveCost = movementCosts[i][j];
					minDist = d;
					newX = j;
					newY = i;
				}
			}
		}
	}
	if (C->X != newX || C->X != newY) {
		C->X = newX;
		C->Y = newY;
		C->AP -= movementCosts[newY][newX];
	}
}

std::string enemyAttack(std::string enemyId, Battle battle) {
	std::string msg = "";
	Character* C = &CHARACTERS[enemyId];

	std::vector<std::string> allies;
	std::vector<std::string> enemies;
	
	std::string eName = low(C->NAME);
	std::vector<std::vector<int>> movementCosts = moveCosts(*C, battle);

	for (std::string id : battle.characters) {
		if (id != enemyId) {
			if (CHARACTERS[id].TEAM == C->TEAM) {
				allies.push_back(id);
			}
			else {
				enemies.push_back(id);
			}
		}
	}

	/*
	- Prefer not moving. Check if you can attack from where you are.
	- If you can't, consider not attacking. Do you have any non-damaging abilities?
	- Can you move in range of an enemy? If so, do so and attack.
	- If not, move as close to the closest enemy as you can get.
	*/

	if (eName == "crazed wolf") {
		// Damage doubles below half HP
		// Has a strong bite attack when not moving, or a weaker nip attack when moving that afflicts bleeding.
		std::vector<std::string> targets = findTargets(C->X, C->Y, 1, enemies);
		if (targets.size() > 0) {
			int index = rand() % targets.size();
			Result result = dealDamage(P_Attack(10, 16, 90), C->ID, CHARACTERS[enemies[index]].ID, battle.characters);
			msg += "*RED*" + pretty(Name(*C)) + " bites savagely at " + pretty(CHARACTERS[enemies[index]].NAME);
			if (result.damage > 0) {
				msg += ", dealing *ORANGE*" + to_str(result.damage) + "*RED* damage!\n";
			}
		}
		else {
			moveInRange(C, enemies, 1, movementCosts, true);
		}
	}


	std::vector<std::string> playerIds = {};

	for (std::string id : allies) {
		if (CHARACTERS[id].TYPE == "player") {
			playerIds.push_back(id);
		}
	}
	for (std::string id : enemies) {
		if (CHARACTERS[id].TYPE == "player") {
			playerIds.push_back(id);
		}
	}

	/*std::string changes = str("STATS") + str(C->ID);
	changes += addLine("X", C->X);
	changes += addLine("Y", C->Y);
	changes += addLine("HP", C->HP);
	data += changes;
	sendData("BUNDLE", data);
	save(*C);*/
	return msg;
}