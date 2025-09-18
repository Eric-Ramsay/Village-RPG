#pragma once
bool fileExists(std::string name) {
	std::ifstream f(name.c_str());
	return f.good();
}

void parseChange(Account& account, std::string type, std::string data) {
	if (type == "USERNAME") {
		account.USERNAME = readStr(data);
	}
	if (type == "PASSWORD") {
		account.PASSWORD = readStr(data);
	}
}

Result dealDamage(Attack attack, std::string attackerId, std::string targetId, std::vector<std::string> allies, std::vector<std::string> enemies) {
	Result result;
	if (CHARACTERS.count(attackerId) * CHARACTERS.count(targetId) == 0) {
		return result;
	}
	Character* attacker = &CHARACTERS[attackerId];
	Character* target = &CHARACTERS[targetId];

	int hitValue = rand() % 100;
	int dmg = attack.dmg;
	if (hitValue < attack.hitChance) {
		result.msg = "*RED*" + target->NAME + " takes *ORANGE*" + to_str(dmg) + "*RED* damage!";
		result.damage = dmg;
		target->HP -= dmg;
	}
	else {
		result.msg += "*RED*" + attacker->NAME + " misses!";
		result.damage = 0;
	}

	return result;
}