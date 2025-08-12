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
