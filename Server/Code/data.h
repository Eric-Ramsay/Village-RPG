#pragma once
// This file concerns saving/loading data & transmitting it to the player

std::string serializeItemList(std::vector<Item> items) {
	std::string data = str("ITEM");
	for (Item item : items) {
		data += str("ID") + str(item.id);
		data += str("NAME") + str(item.name);
		data += str("DESCRIPTION") + str(item.description);
		data += str("EQUIPPED") + str(item.equipped);
		data += str("RUNES");
		for (std::string rune : item.runes) {
			data += str(rune);
		}
	}
	return data + "\n";
}

std::string serializeCharacter(Character C) {
	std::string data = "";
	data += addLine("ID", C.ID);
	data += addLine("NAME", C.NAME);
	data += addLine("DESCRIPTION", C.DESCRIPTION);
	data += addLine("LOCATION", C.LOCATION);
	data += addLine("LEVEL", C.LEVEL);
	data += addLine("XP", C.XP);
	data += addLine("SP", C.SP);
	data += addLine("GOLD", C.GOLD);
	data += addLine("HP", C.HP);
	data += addLine("AP", C.AP);
	data += addLine("BACKPACK", C.BACKPACK);
	data += addLine("ATTACKS", C.ATTACKS);
	data += addLine("CASTS", C.CASTS);
	data += addLine("ROW", C.ROW);
	data += addLine("ROW_PREFERENCE", C.ROW_PREFERENCE);
	data += addLine("LEFT", C.LEFT);
	data += addLine("RIGHT", C.RIGHT);
	data += "HITS!!!" + str(C.HITS[0]) + str(C.HITS[1]) + "\n";
	data += "MISSES!!!" + str(C.MISSES[0]) + str(C.MISSES[1]) + "\n";
	data += serializeItemList(C.INVENTORY);
	return data;
}

std::string serializeEnemy(Character C) {
	std::string data = "";
	data += addLine("ID", C.ID);
	data += addLine("HP", C.HP);
	data += addLine("ROW", C.ROW);
	data += "ARMOR!!!" + str(C.ARMOR[0]) + str(C.ARMOR[1]) + "\n";
	data += addLine("MAXHP", C.MaxHP);
	return data;
}

std::string serializeBattle(Battle battle) {
	std::string data = "";
	data += addLine("ZONE", battle.zone);
	data += addLine("DIFFICULTY", battle.difficulty);
	data += addLine("ROUND", battle.round);
	data += addLine("TURN", battle.turn);
	data += serializeItemList(battle.loot);
	return data;
}

void savePlayer(Character character) {
	// Create and open a text file
	std::ofstream MyFile("./Saves/Characters/" + character.ID + ".txt");

	MyFile << serializeCharacter(character);

	MyFile.close();
}

std::string getLine(std::istream file) {
	std::string line;
	std::getline(file, line);
	return line;
}

Character loadChar(std::string id) {
	std::string data = "";
	Character character;
	std::ifstream file("./Saves/Characters/" + id + ".txt");
	std::vector<std::string> strings = {};
	std::string line = "";
	while (getline(file, line)) {
		strings.push_back(line);
	}
	for (std::string str : strings) {
		std::string type = readStr(str);
		characterChange(character, type, str);
	}
	return character;
}