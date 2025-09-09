#pragma once

int atkDist(int x1, int y1, int x2, int y2) {
	return max(abs(x1 - x2), abs(y1 - y2));
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

void moveInRange(Character* C, std::vector<std::string> enemies, int range, std::vector<std::vector<int>> movementCosts) {
	int minDist = 999;
	int newX = 0;
	int newY = 0;
	for (int iteration = 0; iteration < 12; iteration++) {
		for (int i = (0 - iteration); i <= iteration; i++) {
			for (int j = (0 - iteration); j <= iteration; j++) {
				if (i != 0 || j != 0) {
					int x = C->X + j;
					int y = C->Y + i;
					if (x >= 0 && x < 12 && y >= 0 && y < 12) {
						if (movementCosts[y][x] < C->AP) {
							int dist = movementCosts[y][x];
							if (dist < minDist) {
								newX = x;
								newY = y;
								auto targets = findTargets(x, y, range, enemies);
								if (targets.size() > 0) {
									break;
									break;
									break;
								}
							}
						}
					}
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
			msg += "*RED*The Crazed Wolf*GREY* bites savagely at " + CHARACTERS[enemies[index]].NAME + "!";
			//Result result = dealDamage(P_Attack(10, 16, 90));
		}
		else {
			moveInRange(C, enemies, 1, movementCosts);
		}
	}


	sendStat(C->ID, "X", C->X);
	sendStat(C->ID, "Y", C->Y);
	sendStat(C->ID, "HP", C->HP);
	return msg;
}