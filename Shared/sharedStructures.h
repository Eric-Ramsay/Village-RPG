#pragma once
enum TYPE {
	PHYSICAL,
	MAGICAL,
	TRUE_DMG
};

struct Attack {
	TYPE type;
	int min;
	int max;
	int pen;
	int hitChance;
	Attack(TYPE t, int mn, int mx, int p, int c) {
		type = t;
		min = mn;
		max = mx;
		pen = p;
		hitChance = c;
	}
	Attack() {}
};

struct Spot {
	int x;
	int y;
	Spot(int x1, int y1) {
		x = x1;
		y = y1;
	}
};


Attack P_Attack(int min, int max, int hitChance, int pen = 0) {
	return Attack(PHYSICAL, min, max, pen, hitChance);
}

Attack M_Attack(int min, int max, int hitChance = 100, int pen = 0) {
	return Attack(MAGICAL, min, max, hitChance, pen);
}

Attack T_Attack(int min, int max, int hitChance = 100, int pen = 100) {
	return Attack(TRUE_DMG, min, max, hitChance, pen);
}

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

enum WEAPON_CLASS {
	BLADE,
	BLUNT,
	WHIP,
	AXE,
	POLEARM,
	SHIELD,
	RANGED
};

struct UI_Item {
	std::string id = "";
	std::string type = "";
	std::string description = "";
	int cost = 0;
	int rarity = 0;

	// Weapon Stats
	bool twoHanded = false;
	int range = 0;
	int AP = 0;
	int attacks = 0;
	bool equipped = false;
	Attack attack;
	std::string subclass;

	// Armor Stats
	int def = 0;
	int armor = 0;
	int HP = 0;
	int stamina = 0;
	int mitigation = 0;


	//WEAPON NAME, Name	Class, 	Description,  #Atks	Value 	Chance 	MinDmg 	MaxDmg 	Pen% 	AP 		Range	Rare?
	UI_Item(std::string name, int value, std::string wepType, std::string desc, bool twoH, int atks, int chance, int min, int max, int pen, int ap, int rng, int rare = 0) {
		type = "weapon";
		id = low(name);
		cost = value;
		subclass = wepType;
		description = desc;
		twoHanded = twoH;
		attacks = atks;
		attack = P_Attack(min, max, chance, pen);
		AP = ap;
		range = rng;
		rarity = rare;
	}

	UI_Item(std::string name, std::string desc, int value, int arm, int defense = 0, int stam = 0, int miti = 0, int hp = 0, int ap = 0) {
		type = "armor";
		id = low(name);
		description = desc;
		cost = value;
		armor = arm;
		def = defense;
		stamina = stam;
		mitigation = miti;
		HP = hp;
		AP = ap;
	}
	UI_Item() {}
};

struct Item {
	std::string index = "";
	std::string id = "";
	std::vector<std::string> runes;
	bool equipped = false;
	int attacks = 0;
	Item(std::string id1 = "") {
		id = id1;
		if (id1 == "") {
			std::cout << "Weird Item detected" << std::endl;
		}
		index = id1 + "." + rand() % 9999;
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

enum STATS {
	VIT,
	END,
	DEX,
	MAG,
	WEP,
	AVD,
	DEF
};

struct Drop {
	std::string item;
	int dropChance;
};

// (Bad) Idea: Overload the equal operator to send data to call sendData
// You can('t) use the preprocessor # operator to get a variable name, then uppercase it
struct Character {
	std::string USER = "";
	std::string ID = "";
	std::string TYPE = "player";
	std::string DESCRIPTION = "";
	std::string LOOK = "";
	std::string NAME = "";
	std::string TRADING = "";

	int HP = 30;
	int ARMOR = 0;

	int X = 0;
	int Y = 0;
	int LEVEL = 1;

	int SX = 0;
	int SY = 176;

	std::vector<Effect> effects = {};
	std::string COLOR = "";
	std::unordered_map<std::string, Item> INVENTORY = {};
	std::string LEFT = "";
	std::string RIGHT = "";

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

	std::vector<int> STATS = { 0, 0, 0, 0, 0, 0, 0 };

	int ATTACKS = 0;
	int CASTS = 0;
	int X_PREFERENCE = 0;
	int Y_PREFERENCE = 0;

	// Enemy Stats
	int MaxHP = 30;
	int PHASE = 0;
	int MOVES = 0;
	std::vector<int> ZONES;
	std::vector<Drop> LOOT;

	Character() {}

	Character(std::string name, int hp, int def, int difficulty, std::vector<int> zones, int moves, std::string type, std::string desc)
	{
		NAME = name;
		ID = name;
		MaxHP = hp;
		STATS[DEF] = def;
		LEVEL = difficulty;
		ZONES = zones;
		MOVES = moves;
		TYPE = type;
		DESCRIPTION = desc;
	}
};

struct Hazard {
	std::string type = "";
	int x = 0;
	int y = 0;
	int duration = 999;
};

enum ZONE {
	HauntedCrypts,
	AcridSwamp,
	WiltedWoods
};

struct Battle {
	std::string id = "";
	std::string zone = "";
	int turn = 0;
	int round = 0;
	int difficulty = 0;

	std::vector<Hazard> hazards;
	std::vector<std::string> teams[2];
	std::vector<std::string> dead[2];
	std::unordered_map<std::string, Item> loot;

	Battle(std::string ID, std::string Zone) {
		id = ID;
		zone = Zone;
		std::vector<std::string> zoneNames = {
			"Haunted Crypts",
			"Acrid Swamp",
			"Wilted Woods"
		};
	}

	Battle() {}
};

struct NPC {
	bool MERCHANT = false;
	std::string NAME = "";
	std::string DESCRIPTION = "";
	int index = -1;
	std::vector<std::string> CONVERSATIONS;
	std::vector<std::string> ITEMS;
	NPC(std::string n, bool isMerchant, std::string d) {
		NAME = n;
		DESCRIPTION = d;
		MERCHANT = isMerchant;
	}
	NPC() {}
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
	std::vector<std::string> buildings;
	std::vector<std::string> people;

	bool dungeon = false;
	bool canTravel = true;
	Location(std::string n, std::string desc) {
		id = n;
		description = desc;
	}
	Location() {}
};