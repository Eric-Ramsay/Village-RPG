#pragma once

std::string addEffect(std::string target, std::string attacker, std::string id, int turns, int stacks = 1) {
	UI_Effect effect = getEffect(id);
	if (CHARACTERS.count(target) == 0 || effect.name == "") {
		return "";
	}
	Character* C = &CHARACTERS[target];
	for (int i = 0; i < C->EFFECTS.size(); i++) {
		Effect* active = &C->EFFECTS[i];
		if (active->id == effect.id) {
			if (effect.canStack) {
				active->stacks += stacks;
			}
			active->turns = std::max(turns, active->turns);
			return "";
		}
	}
	C->EFFECTS.push_back(Effect(id, attacker, turns, stacks));
	return "";
}

Result takeDamage(Attack attack, std::string targetId, std::vector<std::string> characters) {
	Result result;
	if (CHARACTERS.count(targetId) == 0) {
		return result;
	}
	Character* target = &CHARACTERS[targetId];
	int hitValue = rand() % 100;
	int dmg = attack.min;
	if (attack.max > attack.min) {
		dmg = attack.min + rand() % (attack.max - attack.min);
	}
	if (hitValue < attack.hitChance) {
		result.msg = name(target) + " takes *ORANGE*" + to_str(dmg) + "*RED* damage!\n";
		result.damage = dmg;
		target->HP -= dmg;
		if (target->HP <= 0) {
			target->DEATH = "The Environment";
		}
	}

	return result;
}

Result dealDamage(Attack attack, std::string attackerId, std::string targetId, std::vector<std::string> characters) {
	Result result;
	if (CHARACTERS.count(attackerId) == 0) {
		return takeDamage(attack, targetId, characters);
	}

	if (CHARACTERS.count(targetId) == 0) {
		return result;
	}
	Character* attacker = &CHARACTERS[attackerId];
	Character* target = &CHARACTERS[targetId];

	int hitValue = rand() % 100;
	int dmg = attack.min;
	if (attack.max > attack.min) {
		dmg = attack.min + rand() % (attack.max - attack.min);
	}
	if (hitValue < attack.hitChance) {
		result.msg = "*ORANGE*" + pretty(name(attacker)) + " *RED*attacks " + name(target) + " for *ORANGE*" + to_str(dmg) + "*RED* damage!\n";
		result.damage = dmg;
		target->HP -= dmg;
		if (target->HP <= 0) {
			target->DEATH = pretty(name(attacker, false));
		}
	}
	else {
		result.msg += "*ORANGE*" + pretty(name(attacker)) + " *RED*misses!\n";
		result.damage = 0;
	}

	return result;
}
