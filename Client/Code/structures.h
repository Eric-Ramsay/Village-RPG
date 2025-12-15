#pragma once

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

enum LOGIN {
	CHOOSE,
	REGISTER_USERNAME,
	REGISTER_PASSWORD,
	LOGIN_USERNAME,
	LOGIN_PASSWORD,
	COMPLETED
};

enum ALIGN {
	LEFT,
	CENTER,
	RIGHT
};

struct TextSegment {
	std::string color = "";
	std::string text = "";
	TextSegment(std::string c = "", std::string t = "") {
		color = c;
		text = t;
	}
};

struct UIState {
	bool vSync = false;

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

	bool mouseHeld = false;
	bool rightHeld = false;
	bool mouseReleased = false;
	bool rightReleased = false;
	bool mousePressed = false;
	bool rightPressed = false;
	bool doubleClicked = false;
	bool scrollDown = false;
	bool scrollUp = true;

	bool UIClick = false;

	bool guarding = false;

	int view = 0;
	bool viewLocked = false;
	bool topLocked = false;

	bool hairCut = false;
	bool viewingGraves = false;
	std::string graveSearch = "";


	std::vector<TextSegment> description = {};
	std::string color = "GREY";

	std::string tooltip = "";

	Item viewedItem;
	std::string viewedEnemy;

	Hazard viewedHazard;

	std::string viewedPlayer;
	std::string viewedEffect;

	std::vector<int> styles = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	std::vector<int> colors = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	std::string charName = "";
	bool randomize = true;
	bool creatingCharacter = false;


	LOGIN signInState = CHOOSE;
};


struct Tooltip {
	std::string title = "";
	std::string description = "";
	Tooltip(std::string t = "", std::string d = "") {
		title = t;
		description = d;
	}
};

struct Scrollbar {
	int orbPos = 0;
	int index = 0;
	int numThings = 1;
	int visibleAtOnce = 1;
	int dX = 0;
	int dY = 0;
	int height = 0;
	bool dragging = false;
	bool visible = true;
	bool inverse = false;

	Scrollbar(int x, int y, int h, bool inv = false) {
		dX = x;
		dY = y;
		height = h;
		inverse = inv;
	}
};


struct Tab {
	int x;
	int y;
	int index = 0;
	std::vector<std::string> tabs;
	std::vector<int> tabSizes = {};
	int totalSize = 0;
	std::string breakColor = "*ORANGE*";
	std::string onColor = "*WHITE*";
	std::string offColor = "*BLACK*";
	std::string indexColor = "*PURPLE*";

	Tab(std::vector<std::string> t = {}, std::string selColor = "*PALE*", std::string bColor = "*ORANGE*", std::string deselColor = "*BLACK*") {
		tabs = t;
		onColor = selColor;
		offColor = deselColor;
		breakColor = bColor;
		for (int i = 0; i < tabs.size(); i++) {
			tabSizes.push_back(0);
		}
	}
};