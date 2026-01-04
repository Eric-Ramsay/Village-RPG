#pragma once

TargetResult findTargets(int team, int x, int y, int range, std::vector<std::string> potentialTargets) {
	TargetResult response;
	int maxDist = 0;
	int minDist = 999999;
	int minHP = 999999;
	int maxHP = 0;
	for (std::string id : potentialTargets) {
		Character* target = &CHARACTERS[id];
		if (target->TEAM != team) {
			int d = atkDist(x, y, target->X, target->Y);
			if (d <= range) {
				if (d < minDist) {
					response.closest = id;
				}
				if (d > maxDist) {
					response.farthest = id;
				}
				if (target->HP > maxHP) {
					response.strongest = id;
				}
				if (target->HP < minHP) {
					response.weakest = id;
				}
				response.targets.push_back(id);
			}
		}
	}
	return response;
}

std::string moveInRange(std::string id, int movementRange, int attackRange, bool maximizeRange = true) {
	Character* C = &CHARACTERS[id];
	if (BATTLES.count(C->LOCATION) == 0) {
		return "";
	}
	Battle battle = BATTLES[C->LOCATION];
	std::vector<std::vector<int>> movementCosts = moveCosts(*C, battle);
	std::string changes = "";
	float minDist = 9999;
	int minX = C->X;
	int minY = C->Y;
	// Find closest target
	for (std::string id : battle.characters) {
		if (CHARACTERS[id].TEAM != C->TEAM) {
			int x = CHARACTERS[id].X;
			int y = CHARACTERS[id].Y;
			int d = atkDist(C->X, C->Y, x, y);
			if (d < minDist) {
				minDist = d;
				minX = x;
				minY = y;
			}
		}
	}

	int startDist = atkDist(minX, minY, C->X, C->Y);
	if (startDist == attackRange || (!maximizeRange && startDist < attackRange)) {
		// No need to move, character is already at or within desired range
		return "";
	}

	int newX = C->X;
	int newY = C->Y;
	minDist = 999;
	int moveCost = 999;
	for (int i = 0; i < movementCosts.size(); i++) {
		for (int j = 0; j < movementCosts.size(); j++) {
			if (movementCosts[i][j] <= movementRange) {
				int d = abs(atkDist(minX, minY, j, i) - attackRange);
				if (d < minDist || (d == minDist && movementCosts[i][j] < moveCost)) {
					moveCost = movementCosts[i][j];
					minDist = d;
					newX = j;
					newY = i;
				}
			}
		}
	}
	if (C->X != newX || C->Y != newY) {
		C->X = newX;
		C->Y = newY;
		changes += printStat(*C, "COORDINATES");
	}
	return changes;
}

DamageResult attack(std::string attackerId, std::string targetId, Attack attack, std::string description) {
	DamageResult result;
	std::string changes = addBundle("TEXT", str(CHARACTERS[attackerId].LOCATION) + "*RED*" + pretty(name(attackerId)) + " " + description + " " + name(CHARACTERS[targetId]));
	result = dealDamage(attackerId, targetId, attack);
	result.changes = changes + result.changes;
	return result;
}

enum Target {
	RANDOM,
	WEAKEST,
	STRONGEST,
	CLOSEST,
	FARTHEST
};

DamageResult moveAttack(std::string id, Attack attack, int movementRange, int attackRange, std::string description, int multiplier = 2, bool maximizeRange = true, Target targetType = RANDOM, bool moveAndAttack = true) {
	Character* C = &CHARACTERS[id];
	Battle* battle = &BATTLES[C->LOCATION];
	std::vector<std::string> enemies;

	std::string changes = "";

	bool moved = false;
	TargetResult result = findTargets(C->TEAM, C->X, C->Y, attackRange, battle->characters);
	if (result.targets.size() == 0) {
		moved = true;
		changes += moveInRange(id, movementRange, attackRange, maximizeRange);
		result = findTargets(C->TEAM, C->X, C->Y, attackRange, battle->characters);
	}
	if (result.targets.size() > 0 && (!moved || moveAndAttack)) {
		std::string targetId = result.targets[random(result.targets.size())];
		if (targetType == WEAKEST) {
			targetId = result.weakest;
		}
		else if (targetType == STRONGEST) {
			targetId = result.strongest;
		}
		else if (targetType == CLOSEST) {
			targetId = result.closest;
		}
		else if (targetType == FARTHEST) {
			targetId = result.farthest;
		}
		if (!moved) {
			attack.min *= multiplier;
			attack.max *= multiplier;
		}
		changes += addBundle("TEXT", str(C->LOCATION) + "*RED*" + pretty(name(id)) + " " + description + " " + name(targetId));
		DamageResult dmgResult = dealDamage(id, targetId, attack);
		dmgResult.changes = changes + dmgResult.changes;
		return dmgResult;
	}
	return DamageResult(changes);
}

std::string enemyAttack(std::string enemyId) {
	std::string changes = "";
	Character* C = &CHARACTERS[enemyId];
	Battle* battle = &BATTLES[C->LOCATION];

	std::vector<std::string> allies;
	std::vector<std::string> enemies;
	
	std::string eName = low(C->NAME);

	for (std::string id : battle->characters) {
		if (id != enemyId) {
			if (CHARACTERS[id].TEAM == C->TEAM) {
				allies.push_back(id);
			}
			else {
				enemies.push_back(id);
			}
		}
	}

	if (eName == "crazed wolf") {
		std::string description = "bites at";
		Attack attack(0, 0, 0, 90);
		if (C->HP < MaxHP(*C) / 2) {
			attack.min *= 2;
			attack.max	*= 2;
			description = "bites savagely at";
		}
		changes += moveAttack(C->ID, attack, 12, 1, description, 3).changes;
	}
	else if (eName == "giant amoeba") {
		std::string description = "bumps into";
		Attack attack(0, 0, 0, 90);
		changes += moveAttack(C->ID, attack, 6, 1, description, 2).changes;
		std::vector<std::vector<int>> movementCosts = moveCosts(*C, *battle);

		if (C->HP > 5) {
			for (int i = -1; i < 2; i++) {
				for (int j = -1; j < 2; j++) {
					if (i != 0 || j != 0) {
						if (random(16) == 0) {
							int x = C->X + j;
							int y = C->Y + i;
							if (x >= 0 && y >= 0 && x < 12 && y < 12) {
								if (movementCosts[y][x] < 12) {
									changes += summon(*battle, "giant amoeba", x, y, C->TEAM, C->HP);
									C->HP -= 5;
									changes += printStat(*C, "HP");
									i = 2; j = 2;
								}
							}
						}
					}
				}
			}
		}
	}
	else if (eName == "goblin archer") {
		std::string description = "looses an arrow at";
		Attack attack(0, 0, 0, 80);
		changes += moveAttack(C->ID, attack, 8, 6, description).changes;
	}
	else if (eName == "goblin swordsman") {
		std::string description = "slices at";
		Attack attack(0, 0, 0, 80);
		changes += moveAttack(C->ID, attack, 8, 1, description, 2).changes;
	}
	else if (eName == "goblin spearman") {
		std::string description = "stabs at";
		Attack attack(0, 0, 0, 80);
		changes += moveAttack(C->ID, attack, 8, 2, description, 2).changes;
	}
	else if (eName == "slugbeast") {

	}
	else if (eName == "acidic slime") {

	}
	else if (eName == "skeletal spearman") {
		std::string description = "stabs at";
		Attack attack(0, 0, 0, 80);
		changes += moveAttack(C->ID, attack, 8, 2, description, 2).changes;
	}
	else if (eName == "skeletal archer") {
		std::string description = "looses an arrow at";
		Attack attack(0, 0, 0, 80);
		changes += moveAttack(C->ID, attack, 8, 6, description).changes;
	}
	else if (eName == "skeletal swordsman") {
		std::string description = "slices at";
		Attack attack(0, 0, 0, 80);
		changes += moveAttack(C->ID, attack, 8, 1, description, 2).changes;
	}
	else if (eName == "ogre") {

	}
	else if (eName == "swamp simian") {

	}
	else if (eName == "giant frog") {

	}
	else if (eName == "living vine") {

	}
	else if (eName == "flaming wisp") {

	}
	else if (eName == "caustic snail") {

	}
	else if (eName == "wisp knight") {

	}
	else if (eName == "giant spider") {

	}
	else if (eName == "lich") {

	}
	else if (eName == "floating spores") {

	}
	else if (eName == "toxic mushroom") {

	}
	else if (eName == "mushroom mage") {

	}
	else if (eName == "hungry ghoul") {

	}
	else if (eName == "imp") {

	}
	else if (eName == "apparition") {

	}
	else if (eName == "wandering moon") {

	}
	else if (eName == "bronze bellbeast") {

	}
	else if (eName == "witch") {

	}
	else if (eName == "scaled drake") {

	}
	else if (eName == "scoundrel") {

	}
	else if (eName == "cultist") {

	}
	else if (eName == "wild bear") {

	}
	else if (eName == "swamp mage") {

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

	return changes;
}