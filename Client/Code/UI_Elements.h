#pragma once

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
	for (int i = 0; i < tab->tabs.size(); i++) {
		int len = measureText(tab->tabs[i]) + 8;
		tab->tabSizes[i] = len;
		tab->totalSize += len;
	}
}

void initTabs() {
	tabList.push_back(&playerMenu);
	tabList.push_back(&combatMenu);
	tabList.push_back(&viewMenu);

	for (int i = 0; i < tabList.size(); i++) {
		measureTab(tabList[i]);
	}
}

void initScrollbars() {
	scrollbars.push_back(&tradeBar);
	scrollbars.push_back(&logBar);
	logBar.orbPos = logBar.height - 24;
}

void initToolTips() {
	TOOLTIPS["VIT"] = Tooltip("Vitality", "*GREEN*+10*GREY* Max *RED*HP*GREY*\n\n*RED*HP*GREY* is a measure of how much *ORANGE*damage*GREY* you can take before your character *BLACK*dies");
	TOOLTIPS["END"] = Tooltip("Endurance", "*GREEN*+10*GREY* Max Stamina\n*GREEN*+1 *GREY*Stamina Regeneration\n\n*GREEN*Endurance*GREY* is used to refill your *YELLOW*AP*GREY* at the start of each turn");
	TOOLTIPS["DEX"] = Tooltip("Dexterity", "*GREEN*+3 AP*GREY* per turn\n\n*YELLOW*AP*GREY* is used to perform actions in your turn, like *ORANGE*attacking*GREY*, *BLUE*casting spells*GREY*, or *GREEN*moving");
	TOOLTIPS["MAG"] = Tooltip("Magic", "*BLUE*+2*GREY* max learned spells\n*BLUE*+1*GREY* spell casts per turn");
	TOOLTIPS["WEP"] = Tooltip("Weapon Handling", "*RED*+1 *GREY*Max Attack Damage\n*RED*+5%*GREY* total attack damage");
	TOOLTIPS["AVD"] = Tooltip("Avoidance", "*GREEN*+5% *GREY*Dodge Chance\n*GREEN*+5% *GREY*Flee Chance\n*YELLOW*+1 AP*GREY* per turn\n\nAvoidance is *ORANGE*capped*GREY* at *RED*10*GREY* points");
	TOOLTIPS["BUTTON_END"] = Tooltip("Ending your Turn", "Once you've done everything you want to do, you should end your turn.\n\nOnce everyone on your team has ended their turn, the enemies will take a turn.");
	TOOLTIPS["BUTTON_BRACE"] = Tooltip("Bracing", "It costs 6 Stamina to Brace. Bracing gives you a 50% chance to dodge the first damage that you would take next turn.");
	TOOLTIPS["BUTTON_GUARD"] = Tooltip("Guarding", "It costs 6 AP to Guard.");
	TOOLTIPS["BUTTON_FLEE"] = Tooltip("Fleeing", "It costs 3 AP to attempt to Flee. If you fail, you can't attack/cast for the remainder of the turn.");

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
	COLORS["pale"] = sf::Color(200, 200, 195);

	for (auto color : COLORS) {
		COLORS[caps(color.first)] = color.second;
		COLORS["*" + color.first + "*"] = color.second;
		COLORS["*" + caps(color.first) + "*"] = color.second;
	}
}

void updateOrbPos(Scrollbar* bar, int index) {
	int num = max(0, (bar->numThings - bar->visibleAtOnce));
	if (num > 0) {
		int height = (bar->height - 24);
		bar->orbPos = index * ((float)height / (float)num);
		if (bar->inverse) {
			bar->orbPos = (num - index) * ((float)height / (float)num);
		}
	}
}

void updateScrollbars() {
	for (int i = 0; i < scrollbars.size(); i++) {
		Scrollbar* bar = scrollbars[i];
		if (bar->visible) {
			bool change = false;
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
	}
}