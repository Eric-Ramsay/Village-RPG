#pragma once

std::string summon(Battle& battle, std::string n, int x = 0, int y = 0, int team = 1, int hp = -1) {
	std::string changes = "";
	std::string name = low(n);
	if (ENEMIES.count(name) > 0) {
		Character enemy = ENEMIES[name];
		if (hp > 0) {
			enemy.HP = hp;
		}
		else {
			enemy.HP = enemy.MAX_HP;
		}
		std::string id = name + "." + battle.id + "." + to_str(random(10000));
		enemy.ID = id;
		enemy.X = x;
		enemy.Y = y;
		enemy.LOCATION = battle.id;
		enemy.TEAM = team;
		battle.characters.push_back(id);
		CHARACTERS[id] = enemy;
		save(enemy);
		changes += addBundle("CHARACTER", serialize(enemy));
		return addBundle("CHARACTER", serialize(enemy));
	}
	std::cout << "Couldn't summon enemy '" + name + "'!" << std::endl;
	return "";
	
}

std::string summon(Battle& battle, std::vector<Character>& enemyList, int team = 1, int hp = -1) {
	std::string changes = "";
	for (Character enemy : enemyList) {
		changes += summon(battle, enemy.NAME, team);
	}
	return changes;
}

std::string addEffect(std::string target, std::string attacker, std::string id, int turns, int stacks = 1) {
	UI_Effect effect = getEffect(id);
	if (id == "" || CHARACTERS.count(target) == 0 || effect.name == "") {
		return "";
	}
	Character* C = &CHARACTERS[target];
	for (int i = 0; i < C->EFFECTS.size(); i++) {
		Effect* active = &C->EFFECTS[i];
		if (active->id == effect.id) {
			if (effect.canStack) {
				active->stacks += stacks;
			}
			if (effect.canRefresh) {
				active->turns = std::max(turns, active->turns);
			}
			return "";
		}
	}
	C->EFFECTS.push_back(Effect(id, attacker, turns, stacks));
	return "";
}

int numStacks(Character C, std::string effectId) {
	std::string id = low(effectId);
	for (Effect effect : C.EFFECTS) {
		if (effect.id == id) {
			return effect.stacks;
		}
	}
	return -1;
}

DamageResult takeDamage(std::string targetId, Attack attack) {
	std::string msg = "";
	DamageResult result;
	if (CHARACTERS.count(targetId) == 0) {
		return result;
	}
	Character* target = &CHARACTERS[targetId];

	Battle battle = BATTLES[target->LOCATION];

	int hitValue = random(100);
	int dmg = attack.min;
	if (attack.max > attack.min) {
		dmg = random(attack.min, attack.max);
	}
	if (hitValue < attack.hitChance) {
		target->HP -= dmg;

		result.damage = dmg;
		result.changes += addBundle("STAT", str(targetId) + str("HP") + str(target->HP));
		msg = name(target) + " takes *ORANGE*" + to_str(dmg) + "*RED* damage!\n";
		
		if (target->HP <= 0) {
			target->DEATH = "The Environment";
			result.changes += addBundle("STAT", str(targetId) + str("DEATH") + str(target->DEATH));
			msg += "*RED*" + pretty(name(target)) + " is struck down!\n";
		}
	}
	if (msg != "") {
		result.changes += addBundle("TEXT", str(battle.id) + msg);
	}

	return result;
}


int mitigate(Character* target, int dmg, int pen) {
	if (dmg < 1) {
		return dmg;
	}
	int armor = Armor(*target);
	int defense = Defense(*target) * (float)(100 - pen)/100.0;

	float percentMitigation = (100.0 - (max(0, armor - pen))) / 100.0;

	dmg *= percentMitigation;
	dmg -= defense;
	
	if (dmg < 1) {
		dmg = 1;
	}

	int dodgeChance = min(50, target->STATS[AVD] * 5);
	if (random(100) < dodgeChance) {
		dmg = 0;
	}

	return dmg;
}

DamageResult dealDamage(std::string attackerId, std::string targetId, Attack attack) {
	std::string msg = "";
	DamageResult result;
	if (CHARACTERS.count(attackerId) == 0) {
		return takeDamage(targetId, attack);
	}

	if (CHARACTERS.count(targetId) == 0) {
		return result;
	}

	Character* attacker = &CHARACTERS[attackerId];
	Character* target = &CHARACTERS[targetId];

	int hitValue = random(100);
	int dmg = attack.min;
	if (attack.max > attack.min) {
		dmg = random(attack.min, attack.max);
	}
	if (hitValue < attack.hitChance) {
		dmg = mitigate(target, dmg, attack.pen);
		target->HP -= dmg;

		result.damage = dmg;
		result.changes += addBundle("STAT", str(targetId) + str("HP") + str(target->HP));
		msg += "*RED*" + pretty(name(target)) + " takes *ORANGE*" + to_str(dmg) + "*RED* damage!\n";

		if (target->HP <= 0) {
			target->DEATH = pretty(name(attacker, false));
			result.changes += printStat(*target, "DEATH");
			msg += "*RED*" + pretty(name(target)) + " is struck down!\n";
		}
	}
	else {
		msg += "*ORANGE*" + pretty(name(attacker)) + " *RED*misses!\n";
	}

	result.changes += addBundle("ATTACK", str(attackerId) + str(targetId));

	if (msg != "") {
		Battle battle = BATTLES[attacker->LOCATION];
		result.changes += addBundle("TEXT", str(battle.id) + msg);
	}

	return result;
}
