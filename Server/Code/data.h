#pragma once
// This file concerns saving/loading data & transmitting it to the player

std::string serializeCharacter(Character character) {
	std::string data = "";
	addLine(data, "NAME!!!" + character.NAME + "!!!");
	addLine(data, "ID!!!" + character.ID + "!!!");
	addLine(data, "DESCRIPTION!!!" + character.DESCRIPTION + "!!!");
	addLine(data, "LOCATION!!!" + character.LOCATION + "!!!");
	addLine(data, "BUILDING!!!" + character.BUILDING + "!!!");
	addLine(data, "LEVEL!!!" + str(character.LEVEL));
	addLine(data, "XP!!!" + str(character.XP));
	addLine(data, "SP!!!" + str(character.SP));
	addLine(data, "GOLD!!!" + str(character.GOLD));
	addLine(data, "HP!!!" + str(character.HP));
	addLine(data, "AP!!!" + str(character.AP));
	addLine(data, "BACKPACK!!!" + str(character.BACKPACK));
	addLine(data, "HITS!!!" + str(character.HITS[0]) + str(character.HITS[1]));
	addLine(data, "MISSES!!!" + str(character.MISSES[0]) + str(character.MISSES[1]));
	addLine(data, "ATTACKS!!!" + str(character.ATTACKS));
	addLine(data, "CASTS!!!" + str(character.CASTS));
	addLine(data, "ROW" + str(character.ROW));
	addLine(data, "ROW_PREFERENCE!!!" + str(character.ROW_PREFERENCE));
	addLine(data, "LEFT!!!" + str(character.LEFT));
	addLine(data, "RIGHT!!!" + str(character.RIGHT));

	for (Item* item : character.INVENTORY) {
		std::string inventory = "ITEM!!!";
		inventory += "TYPE!!!" + item->type + "!!!";
		inventory += "NAME!!!" + item->name + "!!!";
		inventory += "DESCRIPTION!!!" + item->description + "!!!";
		inventory += "VALUE!!!" + str(item->value);
		inventory += "EQUIPPED!!!" + str(item->equipped);
		inventory += "RARE!!!" + str(item->rare);
		if (item->type == "weapon") {
			inventory += "SUBCLASS!!!" + item->subclass + "!!!";
			inventory += "ATTACKS!!!" + str(item->attacks);
			inventory += "MAX_ATTACKS!!!" + str(item->maxAttacks);
			inventory += "CHANCE!!!" + str(item->chance);
			inventory += "MIN!!!" + str(item->min);
			inventory += "MAX!!!" + str(item->max);
			inventory += "AP!!!" + str(item->AP);
			inventory += "RANGE!!!" + str(item->range);
			inventory += "HANDS!!!" + str(item->hands);
		}
		if (item->type == "armor") {
			inventory += "AP!!!" + str(item->AP);
			inventory += "STAMINA!!!" + str(item->stamina);
			inventory += "PHYSICAL_DEFENSE!!!" + str(item->physical);
			inventory += "MAGICAL_DEFENSE!!!" + str(item->magical);
		}
		data += inventory + "\n";
	}
	return data;
}

void parseItem(Item* item, std::string data) {
	while (data.length() > 0) {
		std::string type = readStr(data);
		if (type == "TYPE") {
			item->type = readStr(data);
		}
		if (type == "NAME") {
			item->name = readStr(data);
		}
		if (type == "DESCRIPTION") {
			item->description = readStr(data);
		}
		if (type == "VALUE") {
			item->value = readInt(data);
		}
		if (type == "EQUIPPED") {
			item->equipped = (bool)readInt(data);
		}
		if (type == "RARE") {
			item->rare = (bool)readInt(data);
		}
		if (item->type == "weapon") {
			if (type == "SUBCLASS") {
				item->subclass = readStr(data);
			}
			if (type == "ATTACKS") {
				item->attacks = readInt(data);
			}
			if (type == "MAX_ATTACKS") {
				item->maxAttacks = readInt(data);
			}
			if (type == "CHANCE") {
				item->chance = readInt(data);
			}
			if (type == "MIN") {
				item->min = readInt(data);
			}
			if (type == "MAX") {
				item->max = readInt(data);
			}
			if (type == "AP") {
				item->AP = readInt(data);
			}
			if (type == "RANGE") {
				item->range = readInt(data);
			}
			if (type == "HANDS") {
				item->hands = readInt(data);
			}
		}
		else if (item->type == "armor") {
			if (type == "AP") {
				item->AP = readInt(data);
			}
			if (type == "STAMINA") {
				item->stamina = readInt(data);
			}
			if (type == "PHYSICAL_DEFENSE") {
				item->physical = readInt(data);
			}
			if (type == "MAGICAL_DEFENSE") {
				item->magical = readInt(data);
			}
		}
	}
}

void CharacterChange(Character& character, std::string type, std::string data) {
	if (type == "NAME") {
		character.NAME = readStr(data);
	}
	if (type == "ID") {
		character.ID = readStr(data);
	}
	if (type == "DESCRIPTION") {
		character.DESCRIPTION = readStr(data);
	}
	if (type == "COLOR") {
		character.COLOR = readStr(data);
	}
	if (type == "LOCATION") {
		character.LOCATION = readStr(data);
	}
	if (type == "BUILDING") {
		character.BUILDING = readStr(data);
	}
	if (type == "LEVEL") {
		character.LEVEL = readInt(data);
	}
	if (type == "XP") {
		character.XP = readInt(data);
	}
	if (type == "SP") {
		character.SP = readInt(data);
	}
	if (type == "GOLD") {
		character.GOLD = readInt(data);
	}
	if (type == "HP") {
		character.HP = readInt(data);
	}
	if (type == "BACKPACK") {
		character.BACKPACK = (bool)readInt(data);
	}
	if (type == "HITS") {
		character.HITS[0] = readInt(data);
		character.HITS[1] = readInt(data);
	}
	if (type == "MISSES") {
		character.MISSES[0] = readInt(data);
		character.MISSES[1] = readInt(data);
	}
	if (type == "ROW") {
		character.ROW = readInt(data);
	}
	if (type == "ROW_PREFERENCE") {
		character.ROW_PREFERENCE = readInt(data);
	}
	if (type == "LEFT") {
		character.LEFT = readInt(data);
	}
	if (type == "RIGHT") {
		character.RIGHT = readInt(data);
	}
	if (type == "ATTACKS") {
		character.ATTACKS = readInt(data);
	}
	if (type == "CASTS") {
		character.CASTS = readInt(data);
	}
	if (type == "ITEM") {
		Item* item = new Item();
		character.INVENTORY.push_back(item);
		parseItem(item, data);
	}
}

void saveChar(Character character) {
	// Create and open a text file
	std::ofstream MyFile("./Saves/" + character.ID + ".txt");

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
	std::ifstream file("./Saves/" + id + ".txt");
	std::vector<std::string> strings = {};
	std::string line = "";
	while (getline(file, line)) {
		strings.push_back(line);
	}
	for (std::string str : strings) {
		std::string type = readStr(str);
		CharacterChange(character, type, str);
	}
	return character;
}