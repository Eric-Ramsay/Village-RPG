#pragma once

std::string operator+(std::string str, int num)
{
	return str + to_str(num);
}

Location getLocation(std::string id) {
	for (Location loc : LOCATIONS) {
		if (low(loc.id) == low(id)) {
			return loc;
		}
	}
	std::cout << "Error, Location " + id + " not found!" << std::endl;
	return Location("error", "");
}

float min(float a, float b) {
	if (a < b) {
		return a;
	}
	return b;
}

float max(float a, float b) {
	if (a < b) {
		return b;
	}
	return a;
}

int MaxHP(Character C) {
	return C.MaxHP;
}

Item parseItem(std::string data) {
	Item item;
	while (data.length() > 0) {
		std::string type = readStr(data);
		if (type == "ID") {
			item.id = readStr(data);
		}
		if (type == "NAME") {
			item.name = readStr(data);
		}
		if (type == "DESCRIPTION") {
			item.description = readStr(data);
		}
		if (type == "EQUIPPED") {
			item.equipped = (bool)readInt(data);
		}
		if (type == "RUNES") {
			std::string rune = readStr(data);
			while (rune != "") {
				item.runes.push_back(rune);
				rune = readStr(data);
			}
		}
	}
	return item;
}

void characterChange(Character& character, std::string type, std::string data) {
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
		character.INVENTORY.push_back(parseItem(data));
	}
}
