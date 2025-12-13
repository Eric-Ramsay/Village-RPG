#pragma once
// This file concerns saving/loading data & transmitting it to the player

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

std::string serialize(std::vector<Item> items) {
	std::string data = "";
	for (Item item : items) {
		data += serialize(item);
		data += "\t";
	}
	return data;
}

std::string serialize(std::unordered_map<std::string, Item> items) {
	std::string data = "";
	for (auto item : items) {
		data += serialize(item.second);
		data += "\t";
	}
	return data;
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

std::string serialize(std::vector<Hazard> hazards) {
	std::string data = "";
	for (Hazard hazard : hazards) {
		data += serialize(hazard) + "\t";
	}
	return data;
}

std::string serialize(Account P) {
	std::string data = "";
	data += addLine("USERNAME", P.USERNAME);
	data += addLine("PASSWORD", P.PASSWORD);
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

std::string serialize(std::vector<Effect> effects) {
	std::string data = "";
	for (Effect effect : effects) {
		data += serialize(effect) + "\t";
	}
	return data;
}

std::string serialize(Character C) {
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
	data += addLine("MAXHP", C.MaxHP);
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
}



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