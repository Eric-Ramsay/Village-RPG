#pragma once

struct Spot {
	int x;
	int y;
	Spot(int x1, int y1) {
		x = x1;
		y = y1;
	}
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

struct Item {
	std::string id = "";
	std::string name = "";
	std::string description = "";
	std::vector<std::string> runes;
	bool equipped = false;
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

enum STATS {
	VIT,
	END,
	DEX,
	MAG,
	WEP,
	AVD
};

struct Character {
	std::string USER;
	std::string ID;
	std::string TYPE = "player";
	std::string DESCRIPTION;
	std::string NAME;

	int HP;
	std::vector<int> ARMOR = { 0, 0 };
	int X = 0;
	int Y = 0;
	int LEVEL;

	std::vector<Effect> effects;
	std::string COLOR = "";
	std::vector<Item> INVENTORY;
	int LEFT = -1;
	int RIGHT = -1;

	int SP = 0;
	int GOLD = 0;
	int STAMINA = 0;
	int AP = 0;
	int XP = 0;

	std::string LOCATION = "Harbor";

	bool BACKPACK = false;

	std::vector<int> HITS = { 0, 0 };
	std::vector<int> MISSES = { 0, 0 };

	bool ENDED = false;

	std::vector<int> STATS = { 0, 0, 0, 0, 0, 0 };

	int ATTACKS = 0;
	int CASTS = 0;
	int X_PREFERENCE = 0;
	int Y_PREFERENCE = 0;

	// Enemy Stats
	int MaxHP = 30;
	int PHASE = 0;
	int MOVES = 0;
	std::vector<int> ZONES;
	std::vector<std::string> LOOT;

	Character() {}

	Character(std::string name, int hp, std::vector<int> armor, int difficulty, std::vector<int> zones, int moves, std::string type, std::string desc)
	{
		NAME = name;
		ID = name;
		MaxHP = hp;
		ARMOR[0] = armor[0];
		ARMOR[1] = armor[1];
		LEVEL = difficulty;
		ZONES = zones;
		MOVES = moves;
		TYPE = type;
		DESCRIPTION = desc;
	}
};

enum TYPE {
	PHYSICAL,
	MAGICAL,
	TRUE_DMG
};

struct Attack {
	TYPE type;
	int dmg;
	int pen;
	int hitChance;
	Attack(TYPE t, int d, int p, int c) {
		type = t;
		dmg = d;
		pen = p;
		hitChance = c;
	}
};

Attack P_Attack(int min, int max, int hitChance, int pen = 0) {
	return Attack(PHYSICAL, min + rand() % (max - min), pen, hitChance);
}

Attack M_Attack(int min, int max, int hitChance = 100, int pen = 0) {
	return Attack(MAGICAL, min + rand() % (max - min), hitChance, pen);
}

Attack T_Attack(int min, int max, int hitChance = 100, int pen = 100) {
	return Attack(TRUE_DMG, min + rand() % (max - min), hitChance, pen);
}

struct Hazard {
	std::string type = "";
	int x = 0;
	int y = 0;
	int duration = 999;
};

struct Battle {
	std::string id = "";
	std::string zone = "";
	int turn = 0;
	int round = 0;
	int difficulty = 0;

	std::vector<Hazard> hazards;
	std::vector<std::string> teams[2];
	std::vector<Item> loot;

	Battle(std::string ID, std::string Zone) {
		id = ID;
		zone = Zone;
	}

	Battle() {}
};

struct NPC {
	std::string TYPE = "NPC";
	std::string NAME = "";
	std::string DESCRIPTION = "";
	std::vector<Item> ITEMS;
};

struct Connection {
	std::string direction = "";
	std::string location = "";
	Connection(std::string d, std::string loc) {
		direction = d;
		location = loc;
	}
};

struct Location {
	std::string id = "";
	std::string description = "";

	std::string parent = "";

	std::vector<Connection> connections;
	std::vector<std::string> players;
	std::vector<std::string> buildings;
	std::vector<std::string> people;

	bool dungeon = false;
	bool canTravel = true;
	Location(std::string n, std::string desc) {
		id = n;
		description = desc;
	}
};