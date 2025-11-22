#pragma once

std::string padNum(int num) {
	if (num < 10) {
		return "0" + to_str(num);
	}

	return to_str(num);
}

void setTabString(Tab& tab) {
	tab.tabString = "";
	for (int i = 0; i < tab.tabs.size(); i++) {
		if (tab.index == i) {
			tab.tabString += tab.onColor;
		}
		else {
			tab.tabString += tab.offColor;
		}
		tab.tabString += " " + tab.tabs[i] + " ";
		int len = measureText(" " + tab.tabs[i] + " ");
		tab.tabSizes[i] = len;
		//if (i < tab.tabs.size() - 1) {
		//	tab.tabSizes[i] += 1;
		//	tab.tabString += tab.breakColor + "|";
		//}
	}
}

void handleClick(Tab& tab, int mX, int mY) {
	if (!tab.visible) {
		return;
	}
	int y = tab.y - 3;
	if (mY >= y && mY <= y + 13) {
		int len = measureText(tab.tabString);
		int x = tab.x - (len / 2);
		if (mX >= x && mX <= x + len) {
			for (int i = 0; i < tab.tabSizes.size(); i++) {
				x += tab.tabSizes[i];
				if (mX < x) {
					tab.index = i;
					setTabString(tab);
					return;
				}
			}
		}
		
	}
}

void DrawTabs(Tab& tab, int x1, int y1) {
	tab.x = x1;
	tab.y = y1;
	tab.visible = true;
	int len = measureText(tab.tabString);
	int x = tab.x - (len / 2);
	int y = tab.y - 3;
	std::vector<sf::Color> colors = { sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Cyan };
	for (int i = 0; i < tab.tabSizes.size(); i++) {
		int w = tab.tabSizes[i];
		std::string color = tab.breakColor;
		if (i == tab.index) {
			color = tab.indexColor;
		}
		fillRect(x, y, w, 13, getColor(color));
		fillRect(x + 1, y + 1, w - 2, 11, sf::Color::Black);
		x += w;
	}
	CPrint(tab.tabString, tab.x, tab.y);
}

void DrawTrade() {
	if (CHARACTERS.count(ID) == 0) {
		return;
	}
	Character C = CHARACTERS[ID];
	NPC npc = getNPC(C.TRADING);
	std::string text = npc.DESCRIPTION;
	if (npc.CONVERSATIONS.size() > 0) {
		if (npc.index < 0 || npc.CONVERSATIONS.size() < npc.index) {
			npc.index = 0;
		}
		text += "\n\n" + npc.CONVERSATIONS[npc.index];
	}
	int y = 10 + Print(text, 10, 10, 420);
	//Print("*YELLOW*Items for Sale", 10, y);
	for (int i = 0; i < npc.ITEMS.size(); i++) {
		UI_Item item = getItem(npc.ITEMS[i]);
		Print("*PINK*" + padNum(i + 1) + "*GREY*) " + pretty(item.id), 10, y + 10 * i, 420);
		Print("*YELLOW*" + to_str(item.cost), 150, y + 10 * i, 420);
	}
}

void DrawCharacterUI() {
	int x = 460;
	int y = 5;
	int w = 160;
	int h = 208;
	if (CHARACTERS.count(ID) == 0) {
		return;
	}
	Character C = CHARACTERS[ID];
	Print("*GREEN*" + pretty(C.NAME) + " *BLACK*| *PINK*Level " + C.LEVEL + "*BLACK* | *YELLOW*" + C.XP + "*GREY*/*GREEN*" + 100 * C.LEVEL + "*GREY* XP", x, y);
	Print(DrawBar(C.HP, MaxHP(C), 20, "*RED*"), x, y + 10);
	Print(DrawBar(C.STAMINA, MaxStamina(C), 20, "*GREEN*"), x, y + 20);
	Print("*TEAL*\4 *GREY*" + to_str(C.ARMOR[0]), x, y + 30);
	Print("*TEAL*\5 *GREY*" + to_str(C.ARMOR[1]), x + 30, y + 30);
	Print("*GREEN*AP: " + to_str(C.AP), x + 60, y + 30);
	Print("*PURPLE*SP: *GREY*" + to_str(C.SP), x, y + 40);
	std::vector<std::string> stats = {
		"VIT", "END", "DEX", "MAG", "WEP", "AVD"
	};
	for (int i = 0; i < stats.size(); i++) {
		int xPos = x;
		int yPos = y + 50 + 10 * i;
		if (i > 2) {
			xPos += 80;
			yPos -= 30;
		}
		Print("*TEAL*" + stats[i] + " - *GREY*" + C.STATS[i], xPos, yPos);
		if (C.SP > 0) {
			Print("*TEAL*\6", xPos + 50, yPos);
		}
	}

	DrawTabs(playerMenu, x + w / 2, 87);
	

	if (playerMenu.index == 0) {
		Print("*YELLOW*" + to_str(C.GOLD) + " Gold", x, y + 94);
		int itemCount = 0;
		for (auto item : C.INVENTORY) {
			itemCount++;
			std::string color = "";
			if (item.second.equipped) {
				color = "*TEAL*";
			}
			Print("*PINK*" + padNum(itemCount) + "*GREY*) " + color + pretty(item.second.id), x, y + 94 + 10 * itemCount);
		}
		int num = (5 + (5 * C.BACKPACK)) - itemCount;
		for (int i = itemCount; i < itemCount + num; i++) {
			Print("*PINK*" + padNum(i + 1) + "*GREY*) *BLACK*---", x, y + 104 + 10 * i);
		}
	}
	else if (playerMenu.index == 1) {

	}
	else {

	}
}

void DrawBattle() {
	std::string msg = "";
	int x = 0;
	int y = 0;
	Print("*RED*" + BATTLE.zone, x, y + 1);

	DrawTabs(combatMenu, 323, y + 10);

	std::vector<std::vector<int>> movementCosts = moveCosts(CHARACTERS[ID], BATTLE);
	std::vector<std::vector<std::string>> tiles(12, std::vector<std::string>(12, "blank_tile"));

	tiles[0][2] = "water_tile";
	tiles[1][2] = "water_tile";
	tiles[2][2] = "water_tile";
	tiles[2][3] = "water_tile";
	tiles[2][4] = "water_tile";
	tiles[3][4] = "water_tile";
	tiles[4][4] = "water_tile";
	tiles[5][4] = "water_tile";
	tiles[5][3] = "water_tile";
	tiles[6][3] = "water_tile";
	tiles[6][2] = "water_tile";
	tiles[6][1] = "water_tile";
	tiles[6][0] = "water_tile";

	for (int i = 0; i < BATTLE.teams[1].size(); i++) {
		Character C = CHARACTERS[BATTLE.teams[1][i]];
		tiles[C.Y][C.X] = "*RED*" + to_str(i + 1);
	}
	for (int i = 0; i < BATTLE.teams[0].size(); i++) {
		Character C = CHARACTERS[BATTLE.teams[0][i]];
		tiles[C.Y][C.X] = "*GREEN*" + to_str(i + 1);
	}

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			if (BATTLE.round > 0 && movementCosts[i][j] <= CHARACTERS[ID].AP) {
				Draw(32, 80, 16, 16, (x + j * 16), y + 11 + (i * 16), 1, sf::Color(135, 155, 0));
			}
			if (tiles[i][j] == "blank_tile") {
				Draw(0, 80, 16, 16, (x + j * 16), y + 11 + (i * 16));
			}
			else if (tiles[i][j] == "water_tile") {
				Draw(16, 80, 16, 16, (x + j * 16), y + 11 + (i * 16));
			}
			else {
				CPrint(tiles[i][j], x + 12 + (j * 16), y + 15 + (i * 16));
			}
		}
	}

	y += 10;

	if (combatMenu.index == 0) {
		Print("*PINK*Enemies", x + 200, y + 10);
		for (int i = 0; i < BATTLE.teams[1].size(); i++) {
			Character C = CHARACTERS[BATTLE.teams[1][i]];
			Print("*RED*E" + to_str(i + 1) + " *GREY*" + C.NAME, x + 200, y + 20 + (i * 10));
			Print(DrawBar(C.HP, MaxHP(C), 8, "*RED*"), x + 300, y + 20 + (i * 10));
		}

		Print("*YELLOW*Allies", x + 200, y + 107);
		for (int i = 0; i < BATTLE.teams[0].size(); i++) {
			Character C = CHARACTERS[BATTLE.teams[0][i]];
			std::string pColor = "*GREEN*";
			if (C.TYPE != "player") {
				pColor = "*BLUE*";
			}
			std::string nColor = "*GREY*";
			if (BATTLE.round > 0 && C.ENDED) {
				nColor = "*BLACK*";
			}
			Print(pColor + "P" + to_str(i + 1) + " " + nColor + C.NAME, x + 200, y + 117 + (i * 10));
		}
	}
	else {
		int itemCount = 0;
		for (auto item : BATTLE.loot) {
			UI_Item baseItem = getItem(item.second.id);
			itemCount++;
			std::string text = "*PINK*" + padNum(itemCount) + "*GREY*) *TEAL*" + pretty(item.second.id);
			int xPos = (x + 200);
			int yPos = (y + 1 + 10 * itemCount);
			int w = measureText(text);
			int h = 9;
			Print(text, xPos, yPos);
			Print("*YELLOW*" + to_str(baseItem.cost), xPos + 200, yPos);
			// Check if player clicks on any of these items
			if (UI.mousePressed) {
				if (UI.mX >= xPos && UI.mX <= xPos + w && UI.mY >= yPos && UI.mY <= yPos + h) {
					sendData("COMMAND", "TAKE " + item.second.index);
				}
			}
		}
	}
}

void DrawRoom() {
	std::string msg = "";
	Location room = getLocation(CHARACTERS[ID].LOCATION);
	if (room.parent != "") {
		msg = "*RED*" + room.parent + "*GREY* - *BLUE*" + room.id + "*GREY*\n";
	}
	else {
		msg = "*RED*" + room.id + "*GREY*\n";
	}

	msg += room.description + "\n\n";

	if (room.buildings.size() > 0) {
		msg += "*BLUE*Buildings\n";
		for (std::string building : room.buildings) {
			msg += "*YELLOW*" + building + "\n";
		}
		msg += "\n";
	}

	for (Connection con : room.connections) {
		msg += "*GREEN*" + con.direction + "*GREY* - *BLUE*" + con.location + "\n";
	}
	std::vector<std::string> names = {};
	for (std::string npc : room.people) {
		std::string color = "*GREEN*";
		NPC character = getNPC(npc);
		if (character.MERCHANT) {
			color = "*YELLOW*";
		}
		names.push_back(color + pretty(npc));
	}
	for (auto character : CHARACTERS) {
		Character C = character.second;
		if (C.ID != CHARACTERS[ID].ID && C.LOCATION == room.id) {
			names.push_back("*BLUE*" + pretty(C.NAME));
		}
	}
	int num = names.size();
	if (num > 0) {
		for (int i = 0; i < num; i++) {
			msg += names[i] + "*GREY*";
			if (num > 1 && i < num - 1) {
				if (num > 2) {
					msg += ",";
				}
				if (i == (num - 2)) {
					msg += " and";
				}
				msg += " ";
			}
		}
		if (num > 1) {
			msg += " are here.\n";
		}
		else {
			msg += " is here.\n";
		}
	}
	
	Print(msg, 5, 5, 420);
}

void DrawUI() {
	if (CHARACTERS.count(ID) == 0) {
		Print("No Character!", 50, 50);
	}
	else {
		DrawCharacterUI();
		if (CHARACTERS[ID].LOCATION == BATTLE.id) {
			DrawBattle();
		}
		else if (CHARACTERS[ID].TRADING != "") {
			DrawTrade();
		}
		else {
			DrawRoom();
		}
	}
}