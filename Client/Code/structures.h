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

	bool UIClick = false;

	bool guarding = false;

	int view = 0;
	bool viewLocked = false;

	std::string tooltip = "";

	Item viewedItem;
	Character viewedCharacter;
	std::string viewedEffect;

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


struct Tab {
	int x;
	int y;
	int index = 0;
	std::vector<std::string> tabs;
	std::vector<int> tabSizes = {};
	std::string tabString = "";
	std::string breakColor = "*PINK*";
	std::string onColor = "*GREY*";
	std::string offColor = "*BLACK*";
	std::string indexColor = "*PURPLE*";
	bool visible = false;

	Tab(std::vector<std::string> t = {}, std::string selColor = "*WHITE*", std::string bColor = "*PINK*", std::string deselColor = "*BLACK*") {
		tabs = t;
		onColor = selColor;
		offColor = deselColor;
		breakColor = bColor;
		for (int i = 0; i < tabs.size(); i++) {
			tabSizes.push_back(0);
		}
	}
};