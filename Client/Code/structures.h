#pragma once

struct Message {
	std::string type;
	Message* next = nullptr;
	std::string data = {};
	bool done = false;
	Message(std::string t = "", std::string d = "") {
		type = t;
		data = d;
	}
};

struct C {
	int x;
	int y;
	C(int x1, int y1) {
		x = x1;
		y = y1;
	}
	bool operator==(const C rhs) {
		return this->x == rhs.x && this->y == rhs.y;
	}
	bool operator!=(const C	 rhs) {
		return this->x != rhs.x || this->y != rhs.y;
	}
};

struct Box {
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	Box() {};
	Box(int x1, int y1, int width, int height) {
		x = x1;
		y = y1;
		w = width;
		h = height;
	}
};

struct AmbientSound {
	float volume = 0;
	float target = 0;
};

struct UIState {
	bool vSync = true;

	bool drawingBox = false;
	C box = C(0, 0);
	C boxTile = C(0, 0);

	int timer = 0;
	bool blink = true;
	//Ambient Audio Settings
	AmbientSound sounds[3];

	//Mouse Position on Screen
	float mX = 0;
	float mY = 0;

	//Screen Information
	int W = 1920;
	int H = 1080;
	int scale = 3;

	//Generic game state flags
	bool inGame = true;
	bool paused = false;
	bool WINDOW_DRAW = false;

	bool drawBuildings = true;
	bool drawUnits = true;
	bool mouseHeld = false;
	bool mousePressed = false;

	std::vector<char> uFollowed = {};
	int uHovered = -1;
	int bHovered = -1;
	int uSelected = -1;
	int bSelected = -1;
	bool UIClick = false;

	//Map View and Interaction Information
	C tS = C(0, 0);
	C tH = C(0, 0);
	float zoom = 3;
	int xShift = 0;
	int yShift = 0;
	int xView = 0;
	int yView = 0;

	//UI Element Locations
	C ResPanel = C(0, 0);
	C UnitList = C(0, 0);
	C UnitUpkeep = C(0, 0);
	C BuildingList = C(0, 0);
	C IncomeBreakdown = C(0, 0);
	C ClaimIcon = C(0, 0);
	C TopScores = C(0, 0);
};

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
	std::vector<Item*> INVENTORY;
	int LEFT = -1;
	int RIGHT = -1;

	int SP = 0;
	int GOLD = 0;
	int STAMINA = 0;
	int AP = 0;
	int XP = 0;

	std::string LOCATION = "";

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