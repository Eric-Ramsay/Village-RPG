#pragma once
// This file concerns saving/loading data & transmitting it to the player
std::string serialize(int data) {
	return str(data);
}

std::string serialize(std::string data) {
	return str(data);
}

std::string serialize(Hazard hazard) {
	std::string data = "";
	data += str(hazard.summoner);
	data += str(hazard.index);
	data += str(hazard.x);
	data += str(hazard.y);
	data += str(hazard.duration);
	return data;
}

std::string serialize(Item item) {
	std::string data = "";
	if (item.id == "") {
		return data;
	}
	data += str("ITEM");
	data += str("INDEX") + str(item.index);
	data += str("ATTACKS") + str(item.attacks);
	data += str("EQUIPPED") + str(item.equipped);
	data += str("HITS") + str(item.hits);
	data += str("MISSES") + str(item.misses);
	data += str("RUNES");
	for (std::string rune : item.runes) {
		data += str(rune);
	}
	return data;
}

std::string serialize(Effect effect) {
	std::string data = "";
	data += str(effect.id);
	data += str(effect.causedBy);
	data += str(effect.turns);
	data += str(effect.stacks);
	return data;
}

template<typename T>
std::string serialize(std::vector<T> data) {
	std::string text = "";
	for (T item : data) {
		text += serialize(item) + "\t";
	}
	return text;
}

std::string serialize(std::unordered_map<std::string, Item> items) {
	std::string data = "";
	for (auto item : items) {
		data += serialize(item.second);
		data += "\t";
	}
	return data;
}

std::string serialize(Account P) {
	std::string data = "";
	data += addLine("USERNAME", P.USERNAME);
	data += addLine("PASSWORD", P.PASSWORD);
	return data;
}

std::string serialize(Report r) {
	std::string data = "";

	data += serialize(r.dmgDealt) + "\t";
	data += serialize(r.dmgTaken) + "\t";
	data += serialize(r.dmgMitigated) + "\t";
	data += serialize(r.healingDone) + "\t";
	data += serialize(r.goldEarned) + "\t";
	data += serialize(r.battlesWon) + "\t";

	return data;
}

std::string getStat(Battle B, std::string stat) {
	if (stat == "ID") {
		return serialize(B.id);
	}
	if (stat == "ZONE") {
		return serialize(B.zone);
	}
	if (stat == "TURN") {
		return serialize(B.turn);
	}
	if (stat == "ROUND") {
		return serialize(B.round);
	}
	if (stat == "DIFFICULTY") {
		return serialize(B.difficulty);
	}
	if (stat == "CHARACTERS") {
		return serialize(B.characters);
	}
	if (stat == "DEAD") {
		return serialize(B.dead);
	}
	if (stat == "HAZARDS") {
		return serialize(B.hazards);
	}
	if (stat == "LOOT") {
		return serialize(B.loot);
	}
}

std::string getStat(Character C, std::string stat, std::string data = "") {
	if (stat == "ID") {
		return serialize(C.ID);
	}
	if (stat == "USER") {
		return serialize(C.USER);
	}
	if (stat == "LOOK") {
		return serialize(C.LOOK);
	}
	if (stat == "DEATH") {
		return serialize(C.DEATH);
	}
	if (stat == "HP") {
		return serialize(C.HP);
	}
	if (stat == "X") {
		return serialize(C.X);
	}
	if (stat == "Y") {
		return serialize(C.Y);
	}
	if (stat == "SX") {
		return serialize(C.SX);
	}
	if (stat == "SY") {
		return serialize(C.SY);
	}
	if (stat == "ARMOR") {
		return serialize(C.ARMOR);
	}
	if (stat == "DEFENSE") {
		return serialize(C.DEFENSE);
	}
	if (stat == "MAX_HP") {
		return serialize(C.MAX_HP);
	}
	if (stat == "TYPE") {
		return serialize(C.TYPE);
	}
	if (stat == "NAME") {
		return serialize(C.NAME);
	}
	if (stat == "LOCATION") {
		return serialize(C.LOCATION);
	}
	if (stat == "TEAM") {
		return serialize(C.TEAM);
	}
	if (stat == "EFFECTS") {
		return serialize(C.EFFECTS);
	}
	if (stat == "STAMINA") {
		return serialize(C.STAMINA);
	}
	if (stat == "DESCRIPTION") {
		return serialize(C.DESCRIPTION);
	}
	if (stat == "TRADING") {
		return serialize(C.TRADING);
	}
	if (stat == "LEVEL") {
		return serialize(C.LEVEL);
	}
	if (stat == "XP") {
		return serialize(C.XP);
	}
	if (stat == "SP") {
		return serialize(C.SP);
	}
	if (stat == "GOLD") {
		return serialize(C.GOLD);
	}
	if (stat == "AP") {
		return serialize(C.AP);
	}
	if (stat == "BACKPACK") {
		return serialize(C.BACKPACK);
	}
	if (stat == "ENDED") {
		return serialize(C.ENDED);
	}
	if (stat == "ATTACKS") {
		return serialize(C.ATTACKS);
	}
	if (stat == "CASTS") {
		return serialize(C.CASTS);
	}
	if (stat == "X_PREFERENCE") {
		return serialize(C.X_PREFERENCE);
	}
	if (stat == "Y_PREFERENCE") {
		return serialize(C.Y_PREFERENCE);
	}
	if (stat == "LEFT") {
		return serialize(C.LEFT);
	}
	if (stat == "RIGHT") {
		return serialize(C.RIGHT);
	}
	if (stat == "STATS") {
		return serialize(C.STATS);
	}
	if (stat == "INVENTORY") {
		return serialize(C.INVENTORY);
	}
	if (stat == "ITEM") {
		return serialize(C.INVENTORY[data]);
	}
	if (stat == "COORDINATES") {
		return serialize(C.X) + serialize(C.Y);
	}
	if (stat == "REPORT") {
		return serialize(C.REPORT);
	}
}

std::string statLine(Character C, std::string stat) {
	return str(stat) + getStat(C, stat) + "\n";
}

std::string statLine(Battle B, std::string stat) {
	return str(stat) + getStat(B, stat) + "\n";
}

std::string serialize(Character C) {
	std::string data = "";
	data += statLine(C, "ID");
	data += statLine(C, "HP");
	data += statLine(C, "X");
	data += statLine(C, "Y");
	data += statLine(C, "SX");
	data += statLine(C, "SY");
	data += statLine(C, "ARMOR");
	data += statLine(C, "DEFENSE");
	data += statLine(C, "MAX_HP");
	data += statLine(C, "TYPE");
	data += statLine(C, "NAME");
	data += statLine(C, "LOCATION");
	data += statLine(C, "TEAM");
	data += statLine(C, "EFFECTS");
	if (C.TYPE != "player") {
		return data;
	}
	data += statLine(C, "USER");
	data += statLine(C, "LOOK");
	data += statLine(C, "DEATH");
	data += statLine(C, "STAMINA");
	data += statLine(C, "DESCRIPTION");
	data += statLine(C, "TRADING");
	data += statLine(C, "LEVEL");
	data += statLine(C, "XP");
	data += statLine(C, "SP");
	data += statLine(C, "GOLD");
	data += statLine(C, "AP");
	data += statLine(C, "BACKPACK");
	data += statLine(C, "ENDED");
	data += statLine(C, "ATTACKS");
	data += statLine(C, "CASTS");
	data += statLine(C, "X_PREFERENCE");
	data += statLine(C, "Y_PREFERENCE");
	data += statLine(C, "LEFT");
	data += statLine(C, "RIGHT");
	data += statLine(C, "STATS");
	data += statLine(C, "INVENTORY");
	data += statLine(C, "REPORT");
	return data;
}

std::string serialize(Battle battle, bool saving = false) {
	std::string data = "";
	data += statLine(battle, "ID");
	data += statLine(battle, "ZONE");
	data += statLine(battle, "TURN");
	data += statLine(battle, "ROUND");
	data += statLine(battle, "DIFFICULTY");
	data += statLine(battle, "CHARACTERS");
	data += statLine(battle, "DEAD");
	data += statLine(battle, "HAZARDS");
	data += statLine(battle, "LOOT");
	return data;
}

void saveToFile(std::string filePath, std::string content) {
	std::ofstream MyFile("./Saves/" + filePath + ".txt");
	MyFile << content;
	MyFile.close();
}

void graveSave(Character character) {
	saveToFile("Characters/Graveyard/" + character.ID, serialize(character));
}

void save(Character character) {
	if (character.ID == "") {
		std::cout << "Saving weird character!" << std::endl;
	}
	else if (character.TYPE == "player") {
		saveToFile("Characters/" + character.ID, serialize(character));
	}
	else {
		saveToFile("Characters/Enemies/" + character.ID, serialize(character));
	}
}

void save(Battle battle) {
	saveToFile("Battles/" + battle.id, serialize(battle, true));
}

void save(Account account) {
	saveToFile("Accounts/" + account.USERNAME, serialize(account));
}

std::string getLine(std::istream file) {
	std::string line;
	std::getline(file, line);
	return line;
}

template <typename T> T load(std::string path) {
	std::string data = "";
	T entity;
	std::ifstream file(path);
	std::vector<std::string> strings = {};
	std::string line = "";
	while (getline(file, line)) {
		strings.push_back(line);
	}
	for (std::string str : strings) {
		std::string type = readStr(str);
		parseChange(entity, type, str);
	}
	return entity;
}
