#pragma once
std::string itemId(std::string id) {
	return split(id, '.')[0];
}

Location getLocation(std::string id) {
	if (LOCATIONS.count(low(id)) == 0) {
		std::cout << "Error, Location " + id + " not found!" << std::endl;
		return Location("error", "");
	}
	return LOCATIONS[low(id)];
}

NPC getNPC(std::string id) {
	if (PEOPLE.count(low(id)) == 0) {
		std::cout << "Error, NPC " + id + " not found!" << std::endl;
		return PEOPLE.begin()->second;
	}
	return PEOPLE[low(id)];
}

UI_Item getItem(std::string id) {
	if (ITEMS.count(low(id)) == 0) {
		std::cout << "Error, Item " + id + " not found!" << std::endl;
		return ITEMS.begin()->second;
	}
	return ITEMS[low(id)];
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
	return C.MAX_HP + 10 * C.STATS[VIT];
}

int Defense(Character C) {
	int armor = C.DEFENSE;
	for (auto item : C.INVENTORY) {

	}
	return armor;
}

int Armor(Character C) {
	int armor = C.ARMOR;
	for (auto item : C.INVENTORY) {

	}
	return armor;
}

int MaxStamina(Character C) {
	return 20 + C.STATS[END] * 10;
}

int MaxAP(Character C) {
	return 6 + C.STATS[DEX] * 3 + C.STATS[AVD];
}

Effect parseEffect(std::string data) {
	std::string id = readStr(data);
	std::string causedBy = readStr(data);
	int turns = readInt(data);
	int stacks = readInt(data);
	return Effect(id, causedBy, turns, stacks);
}

Item parseItem(std::string data) {
	Item item;
	while (data.length() > 0) {
		std::string type = readStr(data);
		if (type == "INDEX") {
			item.index = readStr(data);
			item.id = split(item.index, '.')[0];
		}
		if (type == "ATTACKS") {
			item.attacks = readInt(data);
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

Hazard parseHazard(std::string data) {
	Hazard hazard;
	hazard.summoner = readStr(data);
	hazard.index = readInt(data);
	hazard.x = readInt(data);
	hazard.y = readInt(data);
	hazard.duration = readInt(data);
	return hazard;
}

int w_attacks(Character C, Item item) {
	UI_Item baseItem = getItem(item.id);

	return baseItem.attacks;
}

int value(Item item) {
	UI_Item baseItem = getItem(item.id);

	return baseItem.cost;
}

int maxRunes(Character C, Item item) {
	int max = 0;
	UI_Item baseItem = getItem(item.id);
	if (baseItem.type == "weapon") {
		max = 1 + 2 * (1 + baseItem.twoHanded);
		if (baseItem.subclass == "blade") {
			max++;
		}
	}
	else if (baseItem.type == "armor") {
		max = 5;
	}
	else if (baseItem.type == "staff") {
		max = 5;
	}
	return max;
}

int w_AP(Character C, Item item) {
	UI_Item baseItem = getItem(item.id);

	return baseItem.AP;
}

int w_pen(Character C, Item item) {
	UI_Item baseItem = getItem(item.id);

	return baseItem.attack.pen;
}

int w_min(Character C, Item item) {
	UI_Item baseItem = getItem(item.id);

	return baseItem.attack.min;
}

int w_max(Character C, Item item) {
	UI_Item baseItem = getItem(item.id);

	return baseItem.attack.max;
}

int w_range(Character C, Item item) {
	UI_Item baseItem = getItem(item.id);

	return baseItem.range;
}


enum TileState {
	UNEXPLORED,
	OPEN,
	CLOSED
};

struct PathTile {
	TileState state = UNEXPLORED;
	int x = 0;
	int y = 0;
	int baseCost = 2;
	int cost = 0;
	int team = 0;
	bool canLand = true;
	PathTile() {}
	PathTile(int x1, int y1) {
		x = x1;
		y = y1;
		team = -1;
	}
};

std::vector<std::vector<PathTile>> createMap(Battle b) {
	std::vector<std::vector<PathTile>> tiles;

	for (int i = 0; i < 12; i++) {
		tiles.push_back(std::vector<PathTile>());
		for (int j = 0; j < 12; j++) {
			tiles[i].push_back(PathTile(j, i));
		}
	}

	for (Hazard hazard : b.hazards) {
		Terrain terrain = TERRAIN[hazard.index];
		tiles[hazard.y][hazard.x].baseCost = terrain.moveCost;
	}

	for (std::string id : b.characters) {
		Character* C = &CHARACTERS[id];
		tiles[C->Y][C->X].canLand = false;
		tiles[C->Y][C->X].team = C->TEAM;
	}

	return tiles;
}

std::vector<std::vector<int>> moveCosts(Character C, Battle battle) {
	std::vector<std::vector<int>> costs = std::vector<std::vector<int>>(12, std::vector<int>(12));
	if (battle.round == 0) {
		return costs;
	}
	int team = C.TEAM;
	std::vector<std::vector<PathTile>> map = createMap(battle);
	map[C.Y][C.X].state = OPEN;
	bool done = false;
	int currentX = C.X;
	int currentY = C.Y;
	do {
		int currentCost = map[currentY][currentX].cost;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				// If tile hasn't been explored yet, or the cost from this point is cheaper than the previously found cost, add the tile to the open list.
				int x = currentX + j;
				int y = currentY + i;
				if (x >= 0 && x < 12 && y >= 0 && y < 12) {
					PathTile* tile = &map[y][x];
					int cost = tile->baseCost;
					if (tile->team > -1 && tile->team != team) {
						cost += 4;
					}
					if (i != 0 && j != 0) {
						cost *= 1.5;
					}
					cost += currentCost;
					if (tile->state == UNEXPLORED || cost < tile->cost) {
						tile->cost = cost;
						tile->state = OPEN;
					}
				}
			}
		}
		currentX = -1;
		currentY = -1;
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 12; j++) {
				if (map[i][j].state == OPEN) {
					currentX = j;
					currentY = i;
					map[i][j].state = CLOSED;
					i = 12; j = 12;
				}
			}
		}
	} while (currentX != -1 && currentY != -1);

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			int cost = map[i][j].cost;
			if (!map[i][j].canLand) {
				cost = 999;
			}
			costs[i][j] = cost;
		}
	}
	return costs;
}

void parseChange(Character& character, std::string type, std::string data) {
	if (type == "NAME") {
		character.NAME = readStr(data);
	}
	else if (type == "USER") {
		character.USER = readStr(data);
	}
	else if (type == "ENDED") {
		character.ENDED = (bool)readInt(data);
	}
	else if (type == "TYPE") {
		character.TYPE = readStr(data);
	}
	else if (type == "ID") {
		character.ID = readStr(data);
	}
	else if (type == "DESCRIPTION") {
		character.DESCRIPTION = readStr(data);
		character.DESCRIPTION = replace(character.DESCRIPTION, "\r", "\n");
	}
	else if (type == "LOOK") {
		character.LOOK = readStr(data);
	}
	else if (type == "DEATH") {
		character.DEATH = readStr(data);
	}
	else if (type == "TRADING") {
		character.TRADING = readStr(data);
	}
	else if (type == "COLOR") {
		character.COLOR = readStr(data);
	}
	else if (type == "LOCATION") {
		character.LOCATION = readStr(data);
	}
	else if (type == "TEAM") {
		character.TEAM = readInt(data);
	}
	else if (type == "LEVEL") {
		character.LEVEL = readInt(data);
	}
	else if (type == "XP") {
		character.XP = readInt(data);
	}
	else if (type == "SP") {
		character.SP = readInt(data);
	}
	else if (type == "AP") {
		character.AP = readInt(data);
	}
	else if (type == "GOLD") {
		character.GOLD = readInt(data);
	}
	else if (type == "HP") {
		character.HP = readInt(data);
	}
	else if (type == "MAX_HP") {
		character.MAX_HP = readInt(data);
	}
	else if (type == "ARMOR") {
		character.ARMOR = readInt(data);
	}
	else if (type == "DEFENSE") {
		character.DEFENSE = readInt(data);
	}
	else if (type == "STAMINA") {
		character.STAMINA = readInt(data);
	}
	else if (type == "BACKPACK") {
		character.BACKPACK = (bool)readInt(data);
	}
	else if (type == "HITS") {
		character.HITS = readInts(data);
	}
	else if (type == "MISSES") {
		character.MISSES = readInts(data);
	}
	else if (type == "ARMOR") {
		character.ARMOR = readInt(data);
	}
	else if (type == "STATS") {
		std::string backup = data;
		character.STATS = readInts(data);
		if (character.STATS.size() < 6) {
			std::cout << "yo wtf" << std::endl;
		}
	}
	else if (type == "X") {
		character.X = readInt(data);
	}
	else if (type == "Y") {
		character.Y = readInt(data);
	}
	else if (type == "SX") {
		character.SX = readInt(data);
	}
	else if (type == "SY") {
		character.SY = readInt(data);
	}
	else if (type == "X_PREFERENCE") {
		character.X_PREFERENCE = readInt(data);
	}
	else if (type == "Y_PREFERENCE") {
		character.Y_PREFERENCE = readInt(data);
	}
	else if (type == "LEFT") {
		character.LEFT = readStr(data);
	}
	else if (type == "RIGHT") {
		character.RIGHT = readStr(data);
	}
	else if (type == "ATTACKS") {
		character.ATTACKS = readInt(data);
	}
	else if (type == "CASTS") {
		character.CASTS = readInt(data);
	}
	else if (type == "EQUIP") {
		std::string id = readStr(data);
		if (character.INVENTORY.count(id) > 0) {
			character.INVENTORY[id].equipped = true;
		}
	}
	else if (type == "DEQUIP") {
		std::string id = readStr(data);
		if (character.INVENTORY.count(id) > 0) {
			character.INVENTORY[id].equipped = false;
		}
	}
	else if (type == "REMOVE_ITEM") {
		character.INVENTORY.erase(readStr(data));
	}
	else if (type == "ITEM") {
		Item item = parseItem(data);
		character.INVENTORY[item.index] = item;
	}
	else if (type == "INVENTORY") {
		character.INVENTORY = {};
		std::vector<std::string> lines = split(data, '\t');
		for (int i = 0; i < lines.size(); i++) {
			if (lines[i] != "") {
				std::string header = readStr(lines[i]);
				parseChange(character, header, lines[i]);
			}
		}
	}
	else if (type == "EFFECT") {
		Effect effect = parseEffect(data);
		character.EFFECTS.push_back(effect);
	}
	else if (type == "EFFECTS") {
		character.EFFECTS = {};
		std::vector<std::string> lines = split(data, '\t');
		for (int i = 0; i < lines.size(); i++) {
			if (lines[i] != "") {
				parseChange(character, "EFFECT", lines[i]);
			}
		}
	}
}


void parseChange(Battle& battle, std::string type, std::string data) {
	if (type == "ID") {
		battle.id = readStr(data);
	}
	else if (type == "ZONE") {
		battle.zone = readStr(data);
	}
	else if (type == "TURN") {
		battle.turn = readInt(data);
	}
	else if (type == "DIFFICULTY") {
		battle.difficulty = readInt(data);
	}
	else if (type == "ROUND") {
		battle.round = readInt(data);
	}
	else if (type == "HAZARD") {
		Hazard hazard = parseHazard(data);
		battle.hazards.push_back(hazard);
	}
	else if (type == "HAZARDS") {
		battle.hazards = {};
		std::vector<std::string> lines = split(data, '\t');
		for (int i = 0; i < lines.size(); i++) {
			if (lines[i] != "") {
				parseChange(battle, "HAZARD", lines[i]);
			}
		}
	}
	else if (type == "CHARACTERS") {
		battle.characters.clear();
		std::vector<std::string> lines = split(data, '\t');
		for (std::string line : lines) {
			if (line != "") {
				battle.characters.push_back(readStr(line));
			}
		}
	}
	else if (type == "DEAD") {
		battle.dead.clear();
		while (data.length() > 0) {
			battle.dead.push_back(readStr(data));
		}
	}
	else if (type == "REMOVE_ITEM") {
		battle.loot.erase(readStr(data));
	}
	else if (type == "ITEM") {
		Item item = parseItem(data);
		battle.loot[item.index] = item;
	}
	else if (type == "LOOT") {
		battle.loot = {};
		std::vector<std::string> lines = split(data, '\t');
		for (int i = 0; i < lines.size(); i++) {
			if (lines[i] != "") {
				parseChange(battle, readStr(lines[i]), lines[i]);
			}
		}
	}
}

std::vector<int> merge(std::vector<int> arr1, std::vector<int> arr2) {
	int i = 0; int j = 0;
	int len1 = arr1.size();
	int len2 = arr2.size();
	std::vector<int> sorted(len1 + len2, 0);
	int k = 0;
	while (i < len1 || j < len2) {
		if (i == len1) {
			sorted[k++] = arr2[j++];
		}
		else if (j == len2) {
			sorted[k++] = arr1[i++];
		}
		else if (arr1[i] > arr2[j]) {
			sorted[k++] = arr1[i++];
		}
		else {
			sorted[k++] = arr2[j++];
		}
	}
	return sorted;
}

std::vector<int> sort(std::vector<int>& nums) {
	if (nums.size() == 1) {
		return nums;
	}
	std::vector<int> first(nums.begin(), nums.begin() + nums.size() / 2);
	std::vector<int> second(nums.begin() + nums.size() / 2, nums.end());
	return merge(sort(first), sort(second));
}

std::vector<Character> merge(std::vector<Character> arr1, std::vector<Character> arr2) {
	int i = 0; int j = 0;
	int len1 = arr1.size();
	int len2 = arr2.size();
	std::vector<Character> sorted(len1 + len2);
	int k = 0;
	while (i < len1 || j < len2) {
		if (i == len1) {
			sorted[k++] = arr2[j++];
		}
		else if (j == len2) {
			sorted[k++] = arr1[i++];
		}
		else if (arr1[i].LEVEL > arr2[j].LEVEL) {
			sorted[k++] = arr1[i++];
		}
		else {
			sorted[k++] = arr2[j++];
		}
	}
	return sorted;
}

std::vector<Character> sortChars(std::vector<Character>& chars) {
	if (chars.size() == 1) {
		return chars;
	}
	std::vector<Character> first(chars.begin(), chars.begin() + chars.size() / 2);
	std::vector<Character> second(chars.begin() + chars.size() / 2, chars.end());
	return merge(sortChars(first), sortChars(second));
}

template <typename T> bool contains(std::vector<T> elements, T element) {
	for (T elem : elements) {
		if (elem == element) {
			return true;
		}
	}
	return false;
}

bool canTake(Character C, std::string item) {
	return (C.INVENTORY.size() < 5 || (C.BACKPACK && C.INVENTORY.size() < 10)) || low(item) == "backpack";
}

template <typename T> std::vector<T> slice(std::vector<T> data, int begin, int end = -1) {
	if (end == -1) {
		end = data.size();
	}
	std::vector<T> newData = {};
	for (int i = begin; i < end; i++) {
		newData.push_back(data[i]);
	}
	return newData;
}

bool isNum(std::string s) {
	return s.find_first_not_of("0123456789") == std::string::npos;
}

int atkDist(int x1, int y1, int x2, int y2) {
	return max(abs(x1 - x2), abs(y1 - y2));
}

std::vector<int> findItem(std::string args, std::vector<Item> inventory) {
	std::vector<int> indices = {};

	if (isNum(args)) {
		int index = std::stoi(args) - 1;
		if (index >= 0 && index < inventory.size()) {
			return { index };
		}
	}
	for (int i = 0; i < inventory.size(); i++) {
		if (low(inventory[i].id) == args) {
			indices.push_back(i);
		}
	}

	return indices;
}

std::vector<std::string> findItem(std::string args, std::unordered_map<std::string, Item> inventory) {
	std::vector<std::string> ids = {};

	args = low(args);

	if (inventory.count(args) > 0) {
		return { args };
	}
	for (auto item : inventory) {
		if (item.second.id == args) {
			ids.push_back(item.second.index);
		}
	}

	return ids;
}

std::string name(Character C, bool addThe = true) {
	std::string msg = "";
	std::vector<std::string> words = split(C.NAME);
	for (int i = 0; i < words.size(); i++) {
		words[i] = pretty(words[i]);
	}
	msg = join(words);
	if (addThe && C.TYPE != "player") {
		msg = "the " + msg;
	}
	return msg;
}

std::string name(Character* C, bool addThe = true) {
	return name(*C, addThe);
}

std::string name(std::string id, bool addThe = true) {
	return name(CHARACTERS[id], addThe);
}

Character getCharacter(std::string id) {
	if (CHARACTERS.count(id) > 0) {
		return CHARACTERS.at(id);
	}
	std::cout << "Character not found" << std::endl;
	return Character();
}

UI_Effect getEffect(std::string id) {
	if (EFFECTS.count(low(id)) > 0) {
		return EFFECTS[low(id)];
	}
	std::cout << "Effect not found: " << id << std::endl;
	return UI_Effect("", "", "");
}


int nextMessage(std::vector<Message>& msgs) {
	int msgIndex = 0;
	while (msgIndex < msgs.size() && msgs[msgIndex].done) {
		msgIndex++;
	}
	if (msgIndex >= msgs.size()) {
		Message m;
		msgs.push_back(m);
	}
	return msgIndex;
}

int safeC(int a, int b) {
	return (a % b + b) % b;
}