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

int MaxStamina(Character C) {
	return 20 + C.STATS[END] * 10;
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
	if (type == "USER") {
		character.USER = readStr(data);
	}
	if (type == "ENDED") {
		character.ENDED = (bool)readInt(data);
	}
	if (type == "TYPE") {
		character.TYPE = readStr(data);
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
	if (type == "AP") {
		character.AP = readInt(data);
	}
	if (type == "GOLD") {
		character.GOLD = readInt(data);
	}
	if (type == "HP") {
		character.HP = readInt(data);
	}
	if (type == "STAMINA") {
		character.STAMINA = readInt(data);
	}
	if (type == "BACKPACK") {
		character.BACKPACK = (bool)readInt(data);
	}
	if (type == "HITS") {
		character.HITS = readInts(data);
	}
	if (type == "MISSES") {
		character.MISSES = readInts(data);
	}
	if (type == "ARMOR") {
		character.ARMOR = readInts(data);
	}
	if (type == "STATS") {
		character.STATS = readInts(data);
	}
	if (type == "X") {
		character.X = readInt(data);
	}
	if (type == "Y") {
		character.Y = readInt(data);
	}
	if (type == "X_PREFERENCE") {
		character.X_PREFERENCE = readInt(data);
	}
	if (type == "Y_PREFERENCE") {
		character.Y_PREFERENCE = readInt(data);
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


void parseChange(Battle& battle, std::string type, std::string data) {
	if (type == "ID") {
		battle.id = readStr(data);
	}
	if (type == "ZONE") {
		battle.zone = readStr(data);
	}
	if (type == "TURN") {
		battle.turn = readInt(data);
	}
	if (type == "DIFFICULTY") {
		battle.difficulty = readInt(data);
	}
	if (type == "ROUND") {
		battle.round = readInt(data);
	}
	if (type == "ONE" || type == "TWO") {
		int index = 0;
		if (type == "TWO") {
			index = 1;
		}
		battle.teams[index].clear();
		while (data.length() > 0) {
			battle.teams[index].push_back(readStr(data));
		}
	}
	if (type == "ITEM") {
		battle.loot.push_back(parseItem(data));
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