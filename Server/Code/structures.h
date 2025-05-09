#pragma once

struct Item {
	std::string name = "";
	std::string description = "";
	std::string type = "";
	std::vector<std::string> runes;
	int value = 0;
	bool equipped = false;
	bool rare = false;
	int AP = 0;

	// Weapon Properties
	std::string subclass = "";
	int attacks = 0;
	int maxAttacks = 0;
	int chance = 0;
	int min = 0;
	int max = 0;
	int pen = 0;
	int range = 0;
	int hands = 0;

	// Armor Properties
	int stamina;
	int physical;
	int magical;

	Item() {}

	Item(std::string n, std::string sC, std::string d, int h, int a, int v, int c, int mn, int mx, int p, int ap, int rnge, bool rar = false) {
		type = "weapon";
		name = n;
		subclass = sC;
		description = d;
		hands = h;
		attacks = a;
		value = v;
		chance = c;
		min = mn;
		max = mx;
		pen = p;
		AP = ap;
		range = rnge;
		rare = rar;
	}

	Item(std::string n, std::string desc, int val, int ap, int sta, int phys, int mag) {
		AP = ap;
		stamina = sta;
		physical = phys;
		magical = mag;
		value = val;
		name = n;
		description = desc;
		type = "armor";
	}
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

	int HP = 0;
	int ARMOR[2] = { 0, 0 };
	int ROW = 0;
	int LEVEL = 0;

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
	std::vector<Item*> INVENTORY;
	int LEFT = -1;
	int RIGHT = -1;

	int SP = 0;
	int GOLD = 0;
	int STAMINA = 0;
	int AP = 0;
	int XP = 0;

	std::string LOCATION = "";
	std::string BUILDING = "";

	bool BACKPACK = false;

	int HITS[2] = { 0, 0 };
	int MISSES[2] = { 0, 0 };

	int ATTACKS = 0;
	int CASTS = 0;
	int ROW = 0;
	int ROW_PREFERENCE = 0;
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

struct Message {
	std::vector<int> players = {};
	std::string type = "";
	std::string data = "";
	Message* next = nullptr;
	bool done = false;

	Message(std::string t = "", std::string d = "", std::vector<int> p = {}) {
		type = t;
		data = d;
		players = p;
	}

	template<typename T>
	Message& operator &=(const T& rhs)
	{
		this.data += to_str(rhs);
		return *this;
	}
};

struct Player {
	SOCKET socket;
	std::string ID;

	Character character;
	std::vector<Message> messages = {};

	bool connected = true;

	Player(SOCKET s) {
		socket = s;
	}
};