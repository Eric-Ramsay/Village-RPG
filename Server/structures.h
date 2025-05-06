#pragma once

struct Item {
	std::string name = "";
	std::string description = "";
	std::string alias = "";
	std::string type = "";
	std::vector<std::string> runes;
	int value = 0;
	bool equipped = false;
	bool rare = false;
};

struct Weapon : Item {
	std::string type = "weapon";
	std::string subclass = "";
	int attacks;
	int maxAttacks;
	int chance;
	int min;
	int max;
	int AP;
	int range;
	int hands;
};

struct Armor : Item {
	std::string type = "armor";
	int AP;
	int armor[2];
};


struct Effect {
	std::string name;
	bool isDebuff = false;
	std::string description;

	std::string target = "";
	std::string causedBy = "";

	bool stackable = false;
	int duration = 1;
	int stacks = 1;
	Effect(std::string n, bool isD, std::string desc, bool stacks = false) {
		name = n;
		isDebuff = isD;
		description = desc;
		stackable = stacks;
	}
};

struct Fighter {
	std::string ID;
	std::string TYPE;
	std::string DESCRIPTION;
	std::string NAME;

	int HP;
	int ARMOR[2] = { 0, 0 };
	int ROW = 0;
	int LEVEL;

	std::vector<Effect> effects;
};

struct Battle {
	std::string location = "";
	int difficulty = 0;
	int zone = 0;
	int turn = 0;
	bool started = false;

	std::vector<Fighter*> dead[2];

	std::vector<Item> loot;
	std::vector<Fighter*> teams[2];
};


struct Enemy : Fighter {
	int MaxHP = 0;
	int PHASE = 0;
	int MOVES = 0;
	std::vector<int> ZONES;
	std::vector<std::string> LOOT;
};


struct Character : Fighter {
	std::string COLOR = "";
	std::vector<Item> INVENTORY;
	int LEFT = -1;
	int RIGHT = -1;

	int SP = 0;
	int MANA = 0;
	int GOLD = 0;
	int STAMINA = 0;
	int AP = 0;
	int XP = 0;

	std::string LOCATION = "";
	std::string BUILDING = "";

	int INVENTORY_SLOTS = 5;
	bool BACKPACK = false;

	int HITS[2] = { 0, 0 };
	int ATTEMPTS[2] = { 0, 0 };

	int ATTACKS = 0;
	int CASTS = 0;
	int ROW = 0;
};

struct NPC {
	std::string TYPE = "NPC";
	std::string NAME = "";
	std::string DESCRIPTION = "";
	std::vector<Item> ITEMS;
};

struct Location {
	std::string id = "";
	std::string description = "";

	std::vector<std::string> connections;
	std::vector<std::string> players;
	std::vector<std::string> buildings;
	std::vector<std::string> people;

	bool dungeon = false;
	bool canTravel = true;
};