#pragma once

int random(int min, int max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(min, max);

	return distrib(gen);
}

int random(int max) {
	return random(0, max - 1);
}

struct Attack {
	int min;
	int max;
	int pen;
	int hitChance;
	Attack(int mn, int mx, int p = 100, int c = 100) {
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

struct Message {
	std::vector<int> players = {};
	std::string data = "";
	Message* next = nullptr;
	bool done = false;

	Message(std::string d = "", std::vector<int> p = {}) {
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
		attack = Attack(min, max, pen, chance);
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
		index = id1 + "." + random(99999);
	}
};

struct Effect {
	std::string id = "";
	std::string causedBy = "";
	int turns = 0;
	int stacks = 0;

	Effect(std::string i = "", std::string cB = "", int t = 0, int s = 1) {
		id = i;
		turns = t;
		stacks = s;
		causedBy = cB;
	}
};

struct UI_Effect {
	std::string type = "";
	std::string name = "";
	std::string id = "";
	std::string desc = "";
	bool canStack = false;
	bool canRefresh = true;

	UI_Effect(std::string t, std::string n, std::string d, bool cS = false, bool cR = true) {
		type = t;
		id = low(n);
		name = n;
		desc = d;
		canStack = cS;
		canRefresh = cR;
	}

	UI_Effect() {}
};

struct Terrain {
	int index;
	std::string name;

	int sX;
	int sY;

	int moveCost;
	int hp;

	int damage;
	std::vector<Effect> effects;

	//std::vector<Effect> passEffects;
	//int passDamage;

	//std::vector<Effect> standEffects;
	//int standDamage;

	Terrain(std::string n, int mC = 2, int health = 0, int dmg = 0, std::vector<Effect> effectList = {}) {
		name = n;
		moveCost = mC;
		hp = health;

		//passEffects = pE;
		//passDamage = pD;

		effects = effectList;
		damage = dmg;
		//standEffects = sE;
		//standDamage = sD;
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
	std::string DEATH = "";
	std::string NAME = "";
	std::string TRADING = "";

	int HP = 30;
	int ARMOR = 0;

	int X = 0;
	int Y = 0;
	int LEVEL = 1;

	int SX = 0;
	int SY = 176;

	std::vector<Effect> EFFECTS = {};
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
	int TEAM = 0;

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
	int MAX_HP = 30;
	int PHASE = 0;
	int MOVES = 0;
	std::vector<int> ZONES;
	std::vector<Drop> LOOT;

	Character() {}

	Character(std::string name, int hp, int ap, int difficulty, std::vector<int> zones, int moves, std::string type, std::string desc)
	{
		NAME = name;
		ID = name;
		HP = hp;
		MAX_HP = hp;
		LEVEL = difficulty;
		ZONES = zones;
		MOVES = moves;
		TYPE = type;
		DESCRIPTION = desc;
		AP = ap;
	}
};

struct Hazard {
	std::string summoner = "";
	int index = 0;
	int x = 0;
	int y = 0;
	int duration = 999;
	Hazard (int i = 0, int x1 = 0, int y1 = 0, int d = 999, std::string s = "") {
		index = i;
		x = x1;
		y = y1;
		duration = d;
		summoner = s;
	}
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
	std::vector<std::string> characters;
	std::vector<std::string> dead;
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