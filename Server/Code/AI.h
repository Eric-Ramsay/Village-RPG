#pragma once

std::vector<std::string> findTargets(int x, int y, int range, std::vector<std::string> potentialTargets) {
	std::vector<std::string> targets = {};
	for (std::string id : potentialTargets) {
		if (atkDist(x, y, CHARACTERS[id].X, CHARACTERS[id].Y) <= range) {
			targets.push_back(id);
		}
	}
	return targets;
}

std::string moveInRange(Character* C, int range, bool maximizeRange = true) {
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
	if (startDist == range || (!maximizeRange && startDist < range)) {
		// No need to move, character is already at or within desired range
		return "";
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
	if (C->X != newX || C->Y != newY) {
		C->X = newX;
		C->Y = newY;
		C->AP -= movementCosts[newY][newX];
		changes += printStat(*C, "X");
		changes += printStat(*C, "Y");
	}
	return changes;
}

Result attack(std::string attackerId, std::string targetId, Attack attack, std::string description) {
	Result result;
	std::string changes = addBundle("TEXT", str(CHARACTERS[attackerId].LOCATION) + "*RED*" + pretty(name(attackerId)) + " " + description + " " + name(CHARACTERS[targetId]));
	result = dealDamage(attackerId, targetId, attack);
	result.changes = changes + result.changes;
	return result;
}

Result moveAttack(std::string id, int MP, int range, int damage, int hitChance, int pen, std::string description, int multiplier = 2) {

}

std::string enemyAttack(std::string enemyId, Battle battle) {
	std::string changes = "";
	Character* C = &CHARACTERS[enemyId];

	std::vector<std::string> allies;
	std::vector<std::string> enemies;
	
	std::string eName = low(C->NAME);

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

	if (eName == "crazed wolf") {
		// Damage doubles below half HP
		// Has a strong bite attack when not moving, or a weaker nip attack when moving that afflicts bleeding.
		std::vector<std::string> targets = findTargets(C->X, C->Y, 1, enemies);
		if (targets.size() > 0) {
			std::string targetId = targets[rand() % targets.size()];
			Result result = attack(C->ID, targetId, P_Attack(10, 16, 90), "bites savagely at");
			changes += result.changes;
		}
		else {
			changes += moveInRange(C, 1);
		}
	}
	else if (eName == "giant amoeba") {

	}
	else if (eName == "goblin archer") {

	}
	else if (eName == "goblin swordsman") {

	}
	else if (eName == "goblin spearman") {

	}
	else if (eName == "slugbeast") {

	}
	else if (eName == "acidic slime") {

	}
	else if (eName == "skeletal spearman") {

	}
	else if (eName == "skeletal archer") {

	}
	else if (eName == "skeletal archer") {

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
	else if (ename == "swamp mage") {

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