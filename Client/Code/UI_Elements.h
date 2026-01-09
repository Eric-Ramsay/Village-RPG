#pragma once

std::string getSymbol(SYMBOL symbol) {
	std::string s(1, (char)(symbol));
	return s;
}

std::string DrawBar(int val, int max, int size, std::string color, bool drawNum = true, std::string braceColor = "*GREY*", std::string dashColor = "*BLACK*", bool dashes = true) {
	std::string str = braceColor + "[" + color;
	int threshold = size;
	if (max > 0) {
		threshold = (size * val) / max;
	}
	for (int i = 0; i < size; i++) {
		if (i == threshold) {
			str += dashColor;
		}
		if (i < threshold || !dashes) {
			str += "\1";
		}
		else {
			str += "-";
		}
	}
	str += braceColor + "]";
	if (drawNum) {
		return str + "*BLACK* " + val + "/" + max;
	}
	return str;
}

Box DrawButton(std::string text, int x, int y, std::string color = "*ORANGE*", std::string tColor = "", bool center = false) {
	int w = measureText(text) + 8;
	int h = 15;

	if (center) {
		x -= w / 2;
	}

	fillRect(x, y, w, h, getColor(color));
	fillRect(x + 1, y + 1, w - 2, h - 2, sf::Color(10, 20, 30));
	std::string textColor = "*BLACK*";
	if (range(UI.mX, UI.mY, x, y, w, h)) {
		if (UI.mouseHeld) {
			textColor = "*YELLOW*";
		}
		else {
			textColor = "*GREY*";
		}
	}
	if (tColor != "") {
		textColor = tColor;
	}
	Print(textColor + text, x + 4, y + 4);
	return Box(x, y, w, h);
}

Box CenterButton(std::string text, int x, int y, std::string color = "*ORANGE*", std::string tColor = "") {
	return DrawButton(text, x, y, color, tColor, true);
}

void DrawScrollbar(Scrollbar& bar) {
	if (!bar.visible) {
		return;
	}
	fillRect(bar.dX + 2, bar.dY + 7, 4, 1, getColor("GREY")); // Draw Top Bar Styling
	fillRect(bar.dX + 2, bar.dY + (bar.height - 8), 4, 1, getColor("GREY")); // Draw Bot Bar Styling
	fillRect(bar.dX + 3, bar.dY + 8, 2, bar.height - 16, getColor("GREY")); // Draw Long Bar

	Draw(112, 48, 8, 4, bar.dX, bar.dY, 1, getColor("WHITE")); // Draw Up Arrow
	Draw(112, 62, 8, 4, bar.dX, bar.dY + bar.height - 4, 1, getColor("WHITE")); // Draw Down Arrow
	Draw(112, 53, 8, 8, bar.dX, bar.dY + 8 + bar.orbPos, 1, getColor("TEAL")); // Draw Orb

	//Print("+" + to_str(bar.index), bar.dX + 15, bar.dY);
}

void DrawTabs(Tab& tab, int x1, int y1) {
	tab.x = x1;
	tab.y = y1;
	int w = tab.totalSize;

	for (int i = 0; i < tab.tabs.size(); i++) {
		std::string color = tab.breakColor;
		std::string tColor = "";
		if (tab.index == i) {
			tColor = tab.onColor;
			color = tab.indexColor;
		}
		Box box = DrawButton(tab.tabs[i], x1 - w / 2, tab.y, color, tColor);
		if (UI.mouseReleased) {
			if (mRange(box)) {
				tab.index = i;
			}
		}
		x1 += box.w;
	}
}


void handleLoginInput(std::string input) {
	if (UI.signInState == CHOOSE) {
		if (input == "log in") {
			UI.signInState = LOGIN_USERNAME;
		}
		else if (input == "register") {
			UI.signInState = REGISTER_USERNAME;
		}
		stateMessage(UI.signInState);
	}
	else if (UI.signInState == REGISTER_USERNAME) {
		sendData("REGISTER_USERNAME", input);
	}
	else if (UI.signInState == REGISTER_PASSWORD) {
		sendData("REGISTER_PASSWORD", input);
	}
	else if (UI.signInState == LOGIN_USERNAME) {
		USERNAME = input;
		sendData("LOGIN_USERNAME", input);
	}
	else if (UI.signInState == LOGIN_PASSWORD) {
		PASSWORD = input;
		sendData("LOGIN_PASSWORD", input);
	}
}


void measureTab(Tab* tab) {
	tab->totalSize = 0;
	tab->tabSizes = {};
	for (int i = 0; i < tab->tabs.size(); i++) {
		int len = measureText(tab->tabs[i]) + 8;
		tab->tabSizes.push_back(len);
		tab->totalSize += len;
	}
}

void initTabs() {
	tabList.push_back(&playerMenu);
	tabList.push_back(&combatMenu);
	tabList.push_back(&viewMenu);
	tabList.push_back(&tradeMenu);

	for (int i = 0; i < tabList.size(); i++) {
		measureTab(tabList[i]);
	}
}

void initScrollbars() {
	scrollbars.push_back(&tradeBar);
	scrollbars.push_back(&logBar);
	scrollbars.push_back(&graveBar);
	scrollbars.push_back(&lootBar);
	scrollbars.push_back(&allyBar);
	scrollbars.push_back(&enemyBar);
	logBar.orbPos = logBar.height - 24;
}

void setScrollBar(Scrollbar& scrollbar, int dX, int dY, int numThings, int& startIndex, int& maxIndex) {
	scrollbar.visible = true;
	scrollbar.numThings = numThings;
	scrollbar.dX = dX;
	scrollbar.dY = dY;
	DrawScrollbar(scrollbar);

	startIndex = scrollbar.index;
	maxIndex = scrollbar.index + scrollbar.visibleAtOnce;
	if (maxIndex > scrollbar.numThings) {
		maxIndex = scrollbar.numThings;
	}
}

void initToolTips() {
	TOOLTIPS["VIT"] = Tooltip("Vitality *RED*\2", "*GREEN*+10*GREY* Max *RED*HP*GREY*\n\n*RED*HP*GREY* is a measure of how much *ORANGE*damage*GREY* you can take before your character dies");
	TOOLTIPS["END"] = Tooltip("Endurance *GREEN*\5", "*GREEN*+10*GREY* Max Stamina\n*GREEN*+1 *GREY*Stamina Regeneration\n\n*GREEN*Stamina*GREY* is used to refill your *YELLOW*AP*GREY* at the start of each turn");
	TOOLTIPS["DEX"] = Tooltip("Dexterity *YELLOW*\3", "*GREEN*+3 AP*GREY* per turn\n\n*YELLOW*AP*GREY* is used to perform actions in your turn, like *ORANGE*attacking*GREY*, *BLUE*casting spells*GREY*, or *GREEN*moving");
	TOOLTIPS["MAG"] = Tooltip("Magic", "*BLUE*+2*GREY* max learned spells\n*BLUE*+1*GREY* spell casts per turn");
	TOOLTIPS["WEP"] = Tooltip("Weapon Handling", "*RED*+1 *GREY*Max Attack Damage\n*RED*+5%*GREY* total attack damage");
	TOOLTIPS["DEF"] = Tooltip("Defense *NAVY*\4", "*GREEN*+2*GREY* Defense\n\n*GREY*Incoming *RED*damage*GREY* is reduced by both your *BLUE*Armor \x8*GREY* and *NAVY*Defense \4*GREY* stats.\n\n*BLUE*Armor *GREY*applies first, and is percent reduction.\n\n*NAVY*Defense*GREY* is then flat reduction off of any remaining *RED*damage.");
	TOOLTIPS["AVD"] = Tooltip("Avoidance", "*GREEN*+5% *GREY*Dodge Chance\n*GREEN*+5% *GREY*Flee Chance\n*YELLOW*+1 AP*GREY* per turn\n\nAvoidance is *ORANGE*capped*GREY* at *RED*10*GREY* points");
	TOOLTIPS["BUTTON_END"] = Tooltip("Ending your Turn", "Once you've done everything you want to do, you should end your turn.\n\nOnce everyone on your team has ended their turn, the enemies will take a turn.");
	TOOLTIPS["BUTTON_BRACE"] = Tooltip("Bracing", "It costs 6 Stamina to Brace. Bracing gives you a 50% chance to dodge the first damage that you would take next turn.");
	TOOLTIPS["BUTTON_GUARD"] = Tooltip("Guarding", "It costs 6 AP to Guard.");
	TOOLTIPS["BUTTON_FLEE"] = Tooltip("Fleeing", "It costs 3 AP to attempt to Flee. If you fail, you can't attack/cast for the remainder of the turn.");

	TOOLTIPS["AP"] = Tooltip("AP *YELLOW*\3", "*YELLOW*AP*GREY* is used to perform actions in your turn, like *ORANGE*attacking*GREY*, *BLUE*casting spells*GREY*, or *GREEN*moving");
	TOOLTIPS["STAMINA"] = Tooltip("Stamina *GREEN*\5", "*GREEN*Stamina*GREY* is the pool from which your *YELLOW*AP*GREY* is refilled each turn.");
	TOOLTIPS["DEFENSE"] = Tooltip("Defense *NAVY*\4", "Incoming *RED*damage*GREY* is reduced by a flat amount equal to your *NAVY*Defense*GREY* stat.\n\nThis reduction happens AFTER the percent reduction provided by your *BLUE*Armor*GREY* stat.");
	TOOLTIPS["ARMOR"] = Tooltip("Armor *BLUE*\x8", "Incoming *RED*damage*GREY* is reduced by a percent equal to your *BLUE*Armor*GREY* stat.\n\nThis reduction happens BEFORE the flat reduction provided by your *NAVY*Defense*GREY* stat.");
	TOOLTIPS["HP"] = Tooltip("HP *RED*\x2", "*RED*HP*GREY* is a measure of how much *ORANGE*damage*GREY* you can take before your character dies");

	//"ATKS", "DMG", "HITCHANCE", "AP", "RANGE", "PEN"
	TOOLTIPS["ATKS"] = Tooltip("Number of Attacks", "How many times this weapon can attack each turn, if you have enough *YELLOW*AP " + getSymbol(AP));
	TOOLTIPS["DMG"] = Tooltip("Attack Damage", "How much damage a single attack will deal, before mitigation");
	TOOLTIPS["HITCHANCE"] = Tooltip("Hit Chance", "The chance to hit an enemy when you make an attack");
	TOOLTIPS["WEAPON AP"] = Tooltip("Attack Cost", "How much *YELLOW*AP " + getSymbol(AP) + "*GREY* it costs to perform a single attack.");
	TOOLTIPS["RANGE"] = Tooltip("Weapon Range", "How many tiles away a target can be hit by this weapon");
	TOOLTIPS["PEN"] = Tooltip("Penetration", "The damage your enemy mitigates is reduced by this percentage");
	TOOLTIPS["HANDS"] = Tooltip("1H vs 2H", "You can either wield two one-handed weapons, or a single two-handed weapon");

	TOOLTIPS["blade"] = Tooltip("Bladed Weapons", "+1 Max Runes");
	TOOLTIPS["blunt"] = Tooltip("Blunt Weapons", "Each attack deals 2 + 10% damage to enemies within 1 tile of your target");
	TOOLTIPS["axe"] = Tooltip("Axes", "Axes have a 15% chance to deal double damage");
	TOOLTIPS["shield"] = Tooltip("Shields", "Shields offer worse damage in exchange for a chance to block damage");
	TOOLTIPS["polearm"] = Tooltip("Polearms", "Polearms have decent range, and push enemies within 1 tile away.");
	TOOLTIPS["whip"] = Tooltip("Whips", "Whips have decent range, and a 50% chance to afflict *RED*Whipped*GREY*, which deals 3 damage per turn and stacks.");
	TOOLTIPS["ranged"] = Tooltip("Ranged Weapons", "These weapons have much higher range than other weapons");

	TOOLTIPS["LEFTRIGHT"] = Tooltip("Attack Order", "If you have two weapons equipped, you'll attack with the weapon in your right hand first");


	for (auto person : PEOPLE) {
		std::string header = person.second.NAME;
		std::string body = person.second.DESCRIPTION;
		if (person.second.MERCHANT) {
			header += " *BLACK*|*YELLOW* Merchant";
			body += "\n\n*BLACK*Double-click to trade with " + person.second.NAME;
		}
		TOOLTIPS[person.first] = Tooltip(header, body);
	}
}

void initColors() {
	COLORS["amber"] = sf::Color(140, 105, 0);
	COLORS["ruby"] = sf::Color(170, 35, 35);
	COLORS["steel"] = sf::Color(110, 90, 130);
	COLORS["wood"] = sf::Color(120, 65, 35);
	COLORS["mint"] = sf::Color(135, 190, 140);
	COLORS["sand"] = sf::Color(190, 115, 70);
	COLORS["silver"] = sf::Color(165, 155, 185);
	COLORS["coffee"] = sf::Color(80, 60, 10);
	COLORS["pale"] = sf::Color(200, 200, 195);
	COLORS["navy"] = sf::Color(50, 70, 125);
	COLORS["pine"] = sf::Color(40, 110, 75);
	COLORS["indigo"] = sf::Color(25, 70, 75);
	COLORS["raindrop"] = sf::Color(70, 130, 125);
	COLORS["goblin"] = sf::Color(100, 150, 50);
	COLORS["dark"] = sf::Color(10, 20, 30);
	COLORS["background"] = sf::Color(0, 5, 15);

	COLORS["yellow"] = sf::Color(180, 140, 0);
	COLORS["red"] = sf::Color(220, 50, 45);
	COLORS["green"] = sf::Color(135, 155, 0);
	COLORS["brown"] = sf::Color(140, 75, 40);
	COLORS["pink"] = sf::Color(190, 55, 125);
	COLORS["purple"] = sf::Color(160, 90, 250);
	COLORS["blue"] = sf::Color(40, 125, 195);
	COLORS["teal"] = sf::Color(45, 155, 145);
	COLORS["cyan"] = sf::Color(45, 155, 145);
	COLORS["orange"] = sf::Color(190, 75, 40);
	COLORS["black"] = sf::Color(125, 125, 125);
	COLORS["grey"] = sf::Color(240, 240, 235);
	COLORS["white"] = sf::Color(255, 255, 255);


for (auto color : COLORS) {
	COLORS[caps(color.first)] = color.second;
	COLORS["*" + color.first + "*"] = color.second;
	COLORS["*" + caps(color.first) + "*"] = color.second;
}
}

void updateOrbPos(Scrollbar* bar, int index) {
	int origIndex = bar->index;
	int origOrbPos = bar->orbPos;
	int num = max(0, (bar->numThings - bar->visibleAtOnce));
	if (num > 0) {
		int height = (bar->height - 24);
		bar->orbPos = index * ((float)height / (float)num);
		if (bar->inverse) {
			float numVal = (float)num - (float)index;
			float mult = (float)height / (float)num;
			bar->orbPos = std::round(numVal * mult);
			if (origIndex == index && origOrbPos != bar->orbPos) {
				std::cout << "wtf" << std::endl;
			}
		}
	}
}


void updateBarIndex(Scrollbar* bar) {
	bar->orbPos = max(0, min(bar->height - 24, bar->orbPos));

	int num = max(0, (bar->numThings - bar->visibleAtOnce));
	if (num > 0) {
		float scale = (float)bar->orbPos / (float)(bar->height - 24);
		int index = ceil(scale * num);
		if (bar->inverse) {
			bar->index = num - index;
		}
		else {
			bar->index = index;
		}
	}
}

void updateScrollbars() {
	Scrollbar* scrolling = nullptr;
	for (int i = 0; i < scrollbars.size(); i++) {
		Scrollbar* bar = scrollbars[i];
		if (bar->visible) {
			bool change = false;
			if (UI.scrollDown || UI.scrollUp) {
				if (range(UI.mX, UI.mY, 0, bar->dY, bar->dX, bar->height)) {
					if (scrolling == nullptr || bar->dX < scrolling->dX) {
						scrolling = bar;
					}
				}
			}
			if (UI.mouseReleased) {
				bar->dragging = false;
			}
			else if (UI.mousePressed) {
				// Up Button
				if (range(UI.mX, UI.mY, bar->dX, bar->dY, 8, 4)) {
					int index = bar->index - 1;
					if (bar->inverse) {
						index = bar->index + 1;
					}
					updateOrbPos(bar, index);
				}
				// Down Button
				if (range(UI.mX, UI.mY, bar->dX, bar->dY + bar->height - 4, 8, 4)) {
					int index = bar->index + 1;
					if (bar->inverse) {
						index = bar->index - 1;
					}
					updateOrbPos(bar, index);
				}
				if (mRange(Box(bar->dX + 2, bar->dY + 7, 4, bar->height - 14))) {
					bar->dragging = true;
				}
			}
			if (bar->dragging) {
				bar->orbPos = max(0, min(bar->height - 24, UI.mY - (bar->dY + 8)));
			}
			updateBarIndex(bar);
		}
		bar->visible = false;
	}
	if (scrolling != nullptr) {
		int index = scrolling->index - 1;
		if ((UI.scrollDown && !scrolling->inverse) || (UI.scrollUp && scrolling->inverse)) {
			index = scrolling->index + 1;
		}
		updateOrbPos(scrolling, index);
		updateBarIndex(scrolling);
	}
}

Box DrawSection(int x, int y, int w, int h, std::string border = "grey", std::string inside = "dark") {
	fillRect(x, y, w, h, getColor(border));
	fillRect(x + 1, y + 1, w - 2, h - 2, getColor(inside));
	return Box(x, y, w, h);
}

Box DrawSymbol(SYMBOL symbol, int x, int y, std::string color) {
	return Draw(8 * symbol, 58, 7, 7, x, y, 1, getColor(color));
}

void DrawCharacter(int x, int y, std::vector<int> styles, std::vector<int> colors, int scale = 1, int opacity = 255) {
	std::vector<std::vector<std::string>> colorOptions = {
	{ "sand", "orange", "yellow", "amber", "brown", "wood", "pale", "silver", "mint", "green", "raindrop" },
	{ "sand", "orange", "brown", "wood", "yellow", "amber", "red", "pale", "silver", "steel" },
	{ "pale", "silver", "steel", "purple", "pink", "ruby", "orange", "yellow", "amber", "brown", "wood", "coffee", "teal", "blue", "navy", "indigo", "goblin" },
	{ "pale", "silver", "steel", "purple", "pink", "ruby", "orange", "yellow", "amber", "brown", "wood", "coffee", "teal", "blue", "navy", "indigo", "goblin" },
	{ "pale", "silver", "steel", "purple", "pink", "ruby", "orange", "yellow", "amber", "brown", "wood", "coffee", "teal", "blue", "navy", "indigo", "goblin" },
	{ "pale", "silver", "steel", "purple", "pink", "ruby", "orange", "yellow", "amber", "brown", "wood", "coffee", "teal", "blue", "navy", "indigo", "goblin" },
	{ "pale", "silver", "steel", "purple", "pink", "ruby", "orange", "yellow", "amber", "brown", "wood", "coffee", "teal", "blue", "navy", "indigo", "goblin" },
	{ "pale", "silver", "steel", "purple", "pink", "ruby", "orange", "yellow", "amber", "brown", "wood", "coffee", "teal", "blue", "navy", "indigo", "goblin" },
	{ "pale", "silver", "steel", "purple", "pink", "ruby", "orange", "yellow", "amber", "brown", "wood", "coffee", "teal", "blue", "navy", "indigo", "goblin" }
	};

	for (int i = 0; i < styles.size(); i++) {
		Draw(16 * i, 224 + styles[i] * 16, 16, 16, x, y, scale, getColor(colorOptions[i][colors[i]], opacity));
	}
}

void DrawCharacter(int x, int y, std::string look, int scale = 1, int opacity = 255) {
	if (look.size() != 18) {
		return;
	}
	std::vector<int> styles = {};
	std::vector<int> colors = {};
	for (int i = 0; i < look.size(); i += 2) {
		styles.push_back((int)(look[i] - 'a'));
		colors.push_back((int)(look[i + 1] - 'a'));
	}
	DrawCharacter(x, y, styles, colors, scale, opacity);
}