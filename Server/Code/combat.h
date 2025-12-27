#pragma once

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

Result takeDamage(std::string targetId, Attack attack) {
	std::string msg = "";
	Result result;
	if (CHARACTERS.count(targetId) == 0) {
		return result;
	}
	Character* target = &CHARACTERS[targetId];

	Battle battle = BATTLES[target->LOCATION];

	int hitValue = rand() % 100;
	int dmg = attack.min;
	if (attack.max > attack.min) {
		dmg = attack.min + rand() % (attack.max - attack.min);
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


Result dealDamage(std::string attackerId, std::string targetId, Attack attack) {
	std::string msg = "";
	Result result;
	if (CHARACTERS.count(attackerId) == 0) {
		return takeDamage(targetId, attack);
	}

	if (CHARACTERS.count(targetId) == 0) {
		return result;
	}
	
	Battle battle = BATTLES[attackerId];

	Character* attacker = &CHARACTERS[attackerId];
	Character* target = &CHARACTERS[targetId];

	int hitValue = rand() % 100;
	int dmg = attack.min;
	if (attack.max > attack.min) {
		dmg = attack.min + rand() % (attack.max - attack.min);
	}
	if (hitValue < attack.hitChance) {
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

	if (msg != "") {
		result.changes += addBundle("TEXT", str(battle.id) + msg);
	}

	return result;
}
