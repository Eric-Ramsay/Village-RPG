#pragma once
Result dealDamage(Attack attack, std::string attackerId, std::string targetId, std::vector<std::string> allies, std::vector<std::string> enemies) {
	Result result;
	if (CHARACTERS.count(attackerId) * CHARACTERS.count(targetId) == 0) {
		return result;
	}
	Character* attacker = &CHARACTERS[attackerId];
	Character* target = &CHARACTERS[targetId];

	int hitValue = rand() % 100;
	int dmg = attack.min + rand() % (attack.max - attack.min);
	if (hitValue < attack.hitChance) {
		result.msg = "*ORANGE*" + pretty(name(attacker)) + " *RED*attacks " + name(target) + " for *ORANGE*" + to_str(dmg) + "*RED* damage!\n";
		result.damage = dmg;
		target->HP -= dmg;
	}
	else {
		result.msg += "*ORANGE*" + pretty(name(attacker)) + " *RED*misses!\n";
		result.damage = 0;
	}

	return result;
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

std::string enemyAttack(int enemyIndex, std::vector<std::string> allies, std::vector<std::string> enemies, Battle battle) {
	std::string msg = "";
	Character* C = &CHARACTERS[allies[enemyIndex]];
	std::string eName = low(C->NAME);
	std::vector<std::vector<int>> movementCosts = moveCosts(*C, battle);

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
			msg += "*RED*The Crazed Wolf*GREY* bites savagely at " + CHARACTERS[enemies[index]].NAME + "!\n";
			Result result = dealDamage(P_Attack(10, 16, 90), C->ID, CHARACTERS[enemies[index]].ID, allies, enemies);
			msg += result.msg;
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

	std::string changes = str(C->ID);
	changes += addLine("X", C->X);
	changes += addLine("Y", C->Y);
	changes += addLine("HP", C->HP);
	sendData("STATS", changes);
	save(*C);
	return msg;
}