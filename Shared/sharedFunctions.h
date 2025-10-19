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
	if (ITEMS.count(id) == 0) {
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
	return C.MaxHP;
}

int MaxStamina(Character C) {
	return 20 + C.STATS[END] * 10;
}

int MaxAP(Character C) {
	return 6 + C.STATS[DEX] * 3 + C.STATS[AVD];
}

Item parseItem(std::string data) {
	Item item;
	while (data.length() > 0) {
		std::string type = readStr(data);
		if (type == "INDEX") {
			item.index = readStr(data);
			item.key = split(item.index, '.')[0];
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

struct PathTile {
	std::string type = "blank_tile";
	std::string state = "unexplored";
	int x = 0;
	int y = 0;
	int cost = 0;
	int baseCost = 0;
	int team = -1;
	bool canLand = true;
	PathTile() {}
	PathTile(int x1, int y1) {
		x = x1;
		y = y1;
	}
};

int moveCost(std::string type) {
	int cost = 2;
	return cost;
}

std::vector<std::vector<PathTile>> createMap(Battle b) {
	std::vector<std::vector<PathTile>> tiles;

	for (int i = 0; i < 12; i++) {
		tiles.push_back(std::vector<PathTile>());
		for (int j = 0; j < 12; j++) {
			tiles[i].push_back(PathTile());
			tiles[i][j].x = j;
			tiles[i][j].y = i;
			tiles[i][j].baseCost = moveCost(tiles[i][j].type);
		}
	}

	for (int i = 0; i < 2; i++) {
		for (std::string id : b.teams[i]) {
			Character C = CHARACTERS[id];
			tiles[C.Y][C.X].canLand = false;
			tiles[C.Y][C.X].team = i;
		}
	}

	// Check where characters & hazards are & shit


	return tiles;
}

std::vector<std::vector<int>> moveCosts(Character C, Battle battle) {
	int team = 1;
	for (std::string id : battle.teams[0]) {
		if (C.ID == id) {
			team = 0;
			break;
		}
	}
	std::vector<std::vector<PathTile>> map = createMap(battle);
	std::vector<Spot> open = { Spot(C.X, C.Y) };
	while (open.size() > 0) {
		Spot current = open.back();
		map[current.y][current.x].state = "closed";
		open.pop_back();
		int currentCost = map[current.y][current.x].cost;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				// If tile hasn't been explored yet, or the cost from this point is cheaper than the previously found cost, add the tile to the open list.
				int x = min(11, max(current.x + j, 0));
				int y = min(11, max(current.y + i, 0));
				int baseCost = map[y][x].baseCost;
				if (map[y][x].team > -1 && map[y][x].team != team) {
					baseCost += 4;
				}
				if (i != 0 && j != 0) {
					baseCost *= 1.5;
				}
				int cost = currentCost + baseCost;
				PathTile* tile = &map[y][x];
				if (tile->state == "unexplored" || cost < tile->cost) {
					tile->cost = cost;
					if (tile->state != "open") {
						tile->state = "open";
						open.push_back(Spot(x, y));
					}
				}
			}
		}
	}
	std::vector<std::vector<int>> costs = {};
	
	for (int i = 0; i < 12; i++) {
		costs.push_back(std::vector<int>());
		for (int j = 0; j < 12; j++) {
			int cost = map[i][j].cost;
			if (!map[i][j].canLand) {
				cost = 999;
			}
			costs[i].push_back(cost);
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
		character.ARMOR = readInts(data);
	}
	else if (type == "STATS") {
		character.STATS = readInts(data);
	}
	else if (type == "X") {
		character.X = readInt(data);
	}
	else if (type == "Y") {
		character.Y = readInt(data);
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
		character.INVENTORY.erase(data);
	}
	else if (type == "ITEM") {
		Item item = parseItem(data);
		character.INVENTORY[item.index] = item;
	}
	else if (type == "INVENTORY") {
		character.INVENTORY = {};
		std::vector<std::string> lines = split(data, '\n');
		for (int i = 0; i < lines.size(); i++) {
			std::string header = readStr(lines[i]);
			parseChange(character, header, lines[i]);
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
	else if (type == "ONE" || type == "TWO") {
		int index = 0;
		if (type == "TWO") {
			index = 1;
		}
		battle.teams[index].clear();
		while (data.length() > 0) {
			battle.teams[index].push_back(readStr(data));
		}
	}
	else if (type == "REMOVE_ITEM") {
		battle.loot.erase(data);
	}
	else if (type == "ITEM") {
		Item item = parseItem(data);
		battle.loot[item.index] = item;
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
		if (low(inventory[i].key) == args) {
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
		if (item.second.key == args) {
			ids.push_back(item.second.index);
		}
	}

	return ids;
}

std::string name(Character C) {
	std::string msg = "";
	std::vector<std::string> words = split(C.NAME);
	for (int i = 0; i < words.size(); i++) {
		words[i] = pretty(words[i]);
	}
	msg = join(words);
	if (C.TYPE != "player") {
		msg = "the " + msg;
	}
	return msg;
}

std::string name(Character* C) {
	return name(*C);
}