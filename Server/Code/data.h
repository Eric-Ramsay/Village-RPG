#pragma once
// This file concerns saving/loading data & transmitting it to the player

std::string serialize(std::vector<Item> items) {
	std::string data = "";
	for (Item item : items) {
		data += str("ITEM");
		data += str("ID") + str(item.id);
		data += str("NAME") + str(item.name);
		data += str("DESCRIPTION") + str(item.description);
		data += str("EQUIPPED") + str(item.equipped);
		data += str("RUNES");
		for (std::string rune : item.runes) {
			data += str(rune);
		}
		data += "\n";
	}
	return data;
}

std::string serialize(Character C) {
	std::string data = "";
	data += addLine("ID", C.ID);
	data += addLine("HP", C.HP);
	data += addLine("ROW", C.ROW);
	data += "ARMOR!!!" + str(C.ARMOR[0]) + str(C.ARMOR[1]) + "\n";
	data += addLine("MAXHP", C.MaxHP);
	if (C.TYPE == "enemy") {
		return data;
	}
	data += addLine("NAME", C.NAME);
	data += addLine("DESCRIPTION", C.DESCRIPTION);
	data += addLine("LOCATION", C.LOCATION);
	data += addLine("LEVEL", C.LEVEL);
	data += addLine("XP", C.XP);
	data += addLine("SP", C.SP);
	data += addLine("GOLD", C.GOLD);
	
	data += addLine("AP", C.AP);
	data += addLine("BACKPACK", C.BACKPACK);
	data += addLine("ATTACKS", C.ATTACKS);
	data += addLine("CASTS", C.CASTS);
	
	data += addLine("ROW_PREFERENCE", C.ROW_PREFERENCE);
	data += addLine("LEFT", C.LEFT);
	data += addLine("RIGHT", C.RIGHT);
	data += "HITS!!!" + str(C.HITS[0]) + str(C.HITS[1]) + "\n";
	data += "MISSES!!!" + str(C.MISSES[0]) + str(C.MISSES[1]) + "\n";
	data += serialize(C.INVENTORY);
	return data;
}

std::string serialize(Battle battle) {
	std::string data = "";
	data += addLine("ID", battle.id);
	data += addLine("ZONE", battle.zone);
	data += addLine("TURN", battle.turn);
	data += addLine("ROUND", battle.round);
	data += addLine("DIFFICULTY", battle.difficulty);
	data += "ONE!!!";
	for (std::string id : battle.teams[0]) {
		data += str(id);
	}
	data += "\n";
	data += "TWO!!!";
	for (std::string id : battle.teams[1]) {
		data += str(id);
	}
	data += "\n";
	data += serialize(battle.loot);
	return data;
}

void saveToFile(std::string filePath, std::string content) {
	std::ofstream MyFile("./Saves/" + filePath + ".txt");
	MyFile << content;
	MyFile.close();
}

void save(Character character) {
	saveToFile("Characters/" + character.ID, serialize(character));
}

void save(Battle battle) {
	saveToFile("Battles/" + battle.id, serialize(battle));
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