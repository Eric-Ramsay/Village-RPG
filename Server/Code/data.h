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

std::string getStat(Character C, std::string stat) {
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
	if (stat == "HITS") {
		return serialize(C.HITS);
	}
	if (stat == "MISSES") {
		return serialize(C.MISSES);
	}
	if (stat == "STATS") {
		return serialize(C.STATS);
	}
	if (stat == "INVENTORY") {
		return serialize(C.INVENTORY);
	}
}

std::string statLine(Character C, std::string stat) {
	return str(stat) + getStat(C, stat) + "\n";
}

std::string serialize(Character C) {
	std::string data = "";
	data += statLine(C, "ID");
	data += statLine(C, "USER");
	data += statLine(C, "LOOK");
	data += statLine(C, "DEATH");
	data += statLine(C, "HP");
	data += statLine(C, "X");
	data += statLine(C, "Y");
	data += statLine(C, "SX");
	data += statLine(C, "SY");
	data += statLine(C, "ARMOR");
	data += statLine(C, "MAX_HP");
	data += statLine(C, "TYPE");
	data += statLine(C, "NAME");
	data += statLine(C, "LOCATION");
	data += statLine(C, "TEAM");
	data += statLine(C, "EFFECTS");
	if (C.TYPE != "player") {
		return data;
	}
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
	data += statLine(C, "HITS");
	data += statLine(C, "MISSES");
	data += statLine(C, "STATS");
	data += statLine(C, "INVENTORY");
	return data;
}

/*std::string serialize(Character C) {
	std::string data = "";
	data += addLine("ID", C.ID);
	data += addLine("USER", C.USER);
	data += addLine("LOOK", C.LOOK);
	data += addLine("DEATH", C.DEATH);
	data += addLine("HP", C.HP);
	data += addLine("X", C.X);
	data += addLine("Y", C.Y);
	data += addLine("SX", C.SX);
	data += addLine("SY", C.SY);
	data += addLine("ARMOR", C.ARMOR);
	data += addLine("MAX_HP", C.MAX_HP);
	data += addLine("TYPE", C.TYPE);
	data += addLine("NAME", C.NAME);
	data += addLine("LOCATION", C.LOCATION);
	data += addLine("TEAM", C.TEAM);
	data += "EFFECTS!!!" + serialize(C.EFFECTS) + "\n";
	if (C.TYPE != "player") {
		return data;
	}
	data += addLine("STAMINA", C.STAMINA);
	data += addLine("DESCRIPTION", C.DESCRIPTION);
	data += addLine("TRADING", C.TRADING);
	data += addLine("LEVEL", C.LEVEL);
	data += addLine("XP", C.XP);
	data += addLine("SP", C.SP);
	data += addLine("GOLD", C.GOLD);
	
	data += addLine("AP", C.AP);
	data += addLine("BACKPACK", C.BACKPACK);
	data += addLine("ENDED", C.ENDED);
	data += addLine("ATTACKS", C.ATTACKS);
	data += addLine("CASTS", C.CASTS);
	
	data += addLine("X_PREFERENCE", C.X_PREFERENCE);
	data += addLine("Y_PREFERENCE", C.Y_PREFERENCE);
	data += addLine("LEFT", C.LEFT);
	data += addLine("RIGHT", C.RIGHT);
	data += addLine("HITS", C.HITS);
	data += addLine("MISSES", C.MISSES);
	data += addLine("STATS", C.STATS);
	data += "INVENTORY!!!\n";
	data += serialize(C.INVENTORY);
	return data;
}*/

std::string serialize(Battle battle, bool saving = false) {
	std::string data = "";
	data += addLine("ID", battle.id);
	data += addLine("ZONE", battle.zone);
	data += addLine("TURN", battle.turn);
	data += addLine("ROUND", battle.round);
	data += addLine("DIFFICULTY", battle.difficulty);

	data += "CHARACTERS!!!";
	for (std::string id : battle.characters) {
		data += str(id);
	}

	if (saving) {
		data += "DEAD!!!";
		for (std::string id : battle.dead) {
			data += str(id);
		}
	}

	data += "\n";
	data += "HAZARDS!!!";
	data += serialize(battle.hazards);

	data += "\n";
	data += "LOOT!!!";
	data += serialize(battle.loot);
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
