#pragma once

std::string padNum(int num) {
	if (num < 10) {
		return "0" + to_str(num);
	}

	return to_str(num);
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
	if (CHARACTERS.count(ID) == 0) {
		return;
	}
	Character C = CHARACTERS[ID];
	Print("*GREEN*" + pretty(C.NAME) + " *BLACK*| *PINK*Level " + C.LEVEL + "*BLACK* | *YELLOW*" + C.XP + "*GREY*/*GREEN*" + 100 * C.LEVEL + "*GREY* XP", x, y);
	Print(DrawBar(C.HP, MaxHP(C), 20, "*RED*"), x, y + 10);
	Print(DrawBar(MaxStamina(C), MaxStamina(C), 20, "*GREEN*"), x, y + 20);
	Print("*TEAL*\4 *GREY*" + to_str(C.ARMOR[0]), x, y + 30);
	Print("*TEAL*\5 *GREY*" + to_str(C.ARMOR[1]), x, y + 40);
	Print("*GREEN*AP: 7 ", x + 30, y + 30);
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
	}
	Print("*YELLOW*Inventory *GREY*- *YELLOW*" + to_str(C.GOLD) + " Gold", x, y + 80);
	
	int itemCount = 0;
	for (auto item : C.INVENTORY) {
		itemCount++;
		std::string color = "";
		if (item.second.equipped) {
			color = "*TEAL*";
		}
		Print("*PINK*" + padNum(itemCount) + "*GREY*) " + color + pretty(item.second.key), x, y + 80 + 10 * itemCount);
	}
	int num = (5 + (5 * C.BACKPACK)) - itemCount;
	for (int i = itemCount; i < itemCount + num; i++) {
		Print("*PINK*" + padNum(i + 1) + "*GREY*) *BLACK*---", x, y + 90 + 10 * i);
	}
}

void DrawBattle() {
	std::string msg = "";
	Print("*RED*" + BATTLE.zone, 1, 1);

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

	Print("*PINK*Enemies", 200, 18);
	for (int i = 0; i < BATTLE.teams[1].size(); i++) {
		Character C = CHARACTERS[BATTLE.teams[1][i]];
		Print("*RED*E" + to_str(i + 1) + " *GREY*" + C.NAME, 200, 28 + (i * 10));
		tiles[C.Y][C.X] = "*RED*" + to_str(i + 1);
	}

	Print("*YELLOW*Allies", 200, 107);
	for (int i = 0; i < BATTLE.teams[0].size(); i++) {
		Character C = CHARACTERS[BATTLE.teams[0][i]];
		std::string pColor = "*GREEN*";
		if (C.TYPE != "player") {
			pColor = "*BLUE*";
		}
		std::string nColor = "*GREY*";
		if (C.ENDED) {
			nColor = "*BLACK*";
		}
		Print(pColor + "P" + to_str(i + 1) + " " + nColor + C.NAME, 200, 117 + (i * 10));
		tiles[C.Y][C.X] = "*GREEN*" + to_str(i + 1);
	}

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			if (movementCosts[i][j] <= CHARACTERS[ID].AP) {
				Draw(32, 80, 16, 16, (j * 16), 11 + (i * 16), 1, sf::Color(135, 155, 0));
			}
			if (tiles[i][j] == "blank_tile") {
				Draw(0, 80, 16, 16, (j * 16), 11 + (i * 16));
			}
			else if (tiles[i][j] == "water_tile") {
				Draw(16, 80, 16, 16, (j * 16), 11 + (i * 16));
			}
			else {
				CPrint(tiles[i][j], 12 + (j * 16), 15 + (i * 16));
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