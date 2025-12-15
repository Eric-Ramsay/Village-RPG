#pragma once

std::string padNum(int num) {
	if (num < 10) {
		return "0" + to_str(num);
	}

	return to_str(num);
}

void DrawGraves() {
	int x = 5;
	int y = 5;
	Print("*RED*Graveyard", x, y);

	std::vector<Character> graveList = {};
	if (UI.graveSearch == "") {
		graveList = GRAVES;
	}
	else {
		std::string filter = low(UI.graveSearch);
		for (Character C : GRAVES) {
			if (includes(low(C.NAME), filter) || includes(low(C.DEATH), filter)	) {
				graveList.push_back(C);
			}
		}
	}
	
	graveBar.visible = true;
	graveBar.visibleAtOnce = 8;
	graveBar.numThings = graveList.size();
	graveBar.dY = y + 34;
	graveBar.dX = x + 250;
	graveBar.height = graveBar.visibleAtOnce * 30;
	DrawScrollbar(graveBar);

	DrawSection(x, y + 10, 225, 15);
	Print(UI.graveSearch + '_', x + 4, y + 14, 145);

	int startIndex = graveBar.index;
	int maxIndex = graveBar.index + graveBar.visibleAtOnce;
	if (maxIndex > graveList.size()) {
		maxIndex = graveList.size();
	}
	for (int i = startIndex; i < maxIndex; i++) {
		int yPos = y + 34 + (i - startIndex) * 30;
		Character C = graveList[i];
		Box box = DrawSection(x, yPos, 225, 31);
		if (mRange(box)) {
			UI.viewedPlayer = C.ID;
		}
		DrawCharacter(x, yPos + 7, C.LOOK);
		Print("*PINK*" + padNum(i + 1) + "*GREY*) *GREEN*" + C.NAME, x + 18, yPos + 5);
		Print("*PINK*Level " + to_str(C.LEVEL), x + 178, yPos + 5);
		Print("*BLACK*Killed by *RED*" + to_str(C.DEATH), x + 18, yPos + 18);
	}

	if (graveList.size() == 0) {
		Print("*BLACK*You don't see any graves . . .", x, y + 30);
	}

	if (GRAVEYARD.count(UI.viewedPlayer) > 0) {
		Character C = GRAVEYARD[UI.viewedPlayer];
		int xPos = x + 260 + 90;
		int yPos = y + 10 + 16 * 4;
		DrawCharacter(xPos - 8 * 4, y, C.LOOK, 4);
		CPrint("*GREEN*" + C.NAME, xPos, yPos);
		std::string loc = split(C.LOCATION, '.')[0];
		CPrint("*RED*" + C.DEATH, xPos, yPos + 12);
		CPrint("*PURPLE*" + loc, xPos, yPos + 24);

		Print("*PINK*Damage Dealt:", xPos - 55, yPos + 48);
		Print("0", xPos + 55, yPos + 48);

		Print("*ORANGE*Damage Mitigated:", xPos - 55, yPos + 60);
		Print("0", xPos + 55, yPos + 60);

		Print("*RED*Healing:", xPos - 55, yPos + 72);
		Print("0", xPos + 55, yPos + 72);

		Print("*YELLOW*Gold Earned:", xPos - 55, yPos + 84);
		Print("0", xPos + 55, yPos + 84);

		Print("*BLUE*Battles Won:", xPos - 55, yPos + 96);
		Print("0", xPos + 55, yPos + 96);
	}


	Box box = DrawButton("Go Back", x, y + 310);
	if (UI.mouseReleased && mRange(box)) {
		UI.viewingGraves = false;
	}

}

void DrawHaircut() {
	Character C = CHARACTERS[ID];
	int x = 5;
	int y = 10;
	int w = 400;
	int h = 240;
	std::string instructions = "Use the text box and the color pickers below to enter a description for your character.";
	if (C.GOLD < 15) {
		instructions += "\n\n*RED*You don't have enough money for a haircut!";
	}
	Print(instructions, x, y, w);

	std::vector<std::string> colors = {
		"GREY",
		"BLACK",
		"RED",
		"YELLOW",
		"GREEN",
		"BLUE",
		"TEAL",
		"PINK",
		"PURPLE",
		"ORANGE",
		"BROWN"
	};

	for (int i = 0; i < colors.size(); i++) {
		std::string color = "WHITE";
		if (colors[i] == "GREY") {
			color = "PINK";
		}
		if (UI.color == colors[i]) {
			Draw(128, 48, 7, 7, x + 46, y + 71 + i * 10, 1, getColor("YELLOW"));
		}
		Box box = fillRect(x + 55, y + 70 + i * 10, 9, 9, getColor(color));
		fillRect(x + 56, y + 71 + i * 10, 7, 7, getColor(colors[i]));
		if (UI.mouseReleased && mRange(box)) {
			UI.color = colors[i];
			int index = UI.description.size() - 1;
			if (index > -1 && UI.description[index].text == "") {
				UI.description[index].color = "*" + UI.color + "*";
			}
			else {
				UI.description.push_back(TextSegment("*" + UI.color + "*", ""));
			}
		}
	}

	CPrint("*GREEN*Example View", x + 70 + 189 / 2, y + 59);
	fillRect(x + 70, y + 69, 189, 112, getColor("GREY"));
	fillRect(x + 71, y + 70, 187, 110, sf::Color(10, 20, 30));
	std::string text = "";
	for (TextSegment seg : UI.description) {
		text += seg.color;
		text += seg.text;
	}
	Print(replace(text, "\r", "\n") + '_', x + 73, y + 72, 185);


	Box box = CenterButton("Go Back", x + 65 + 189/4, y + 190);
	if (mRange(box)) {
		if (UI.mouseReleased) {
			UI.hairCut = false;
		}
	}
	box = CenterButton("Confirm - *YELLOW*15G", x + 65 + (189 * 3)/4, y + 190);
	if (mRange(box)) {
		if (UI.mouseReleased) {
			if (C.GOLD < 15) {
				logs.push_back("*RED*You don't have enough money!");
			}
			else if (text == "") {
				logs.push_back("*RED*You must input a description first!");
			}
			else if (C.DESCRIPTION == text) {
				logs.push_back("*RED*You can't choose the exact same description!");
			}
			else {
				sendData("COMMAND", "haircut " + cleanText(text));
				UI.description = {};
			}
		}
	}
}

void DrawTrade() {
	int x = 5;
	int y = 10;
	if (CHARACTERS.count(ID) == 0) {
		return;
	}
	Character C = getCharacter(ID);
	NPC npc = getNPC(C.TRADING);
	std::string text = npc.DESCRIPTION;
	if (npc.CONVERSATIONS.size() > 0) {
		if (npc.index < 0 || npc.CONVERSATIONS.size() < npc.index) {
			npc.index = 0;
		}
		text += "\n\n" + npc.CONVERSATIONS[npc.index];
	}
	Print(text, x, y, 420);

	y = 110;
	Print("*YELLOW*" + to_str(npc.ITEMS.size()) + " Items for Sale", x, y);

	y = 120;
	tradeBar.visible = true;
	tradeBar.visibleAtOnce = 10;
	tradeBar.numThings = npc.ITEMS.size();
	tradeBar.dY = y;
	tradeBar.dX = x + 250;
	tradeBar.height = tradeBar.visibleAtOnce * 10;
	DrawScrollbar(tradeBar);

	int startIndex = tradeBar.index;
	int maxIndex = tradeBar.index + tradeBar.visibleAtOnce;
	if (maxIndex > npc.ITEMS.size()) {
		maxIndex = npc.ITEMS.size();
	}
	for (int i = startIndex; i < maxIndex; i++) {
		UI_Item item = getItem(npc.ITEMS[i]);
		Box box = Print("*PINK*" + padNum(i + 1) + "*GREY*) " + pretty(item.id), x, y + 10 * (i - startIndex), 420);
		if (mRange(box)) {
			if (UI.doubleClicked) {
				sendData("COMMAND", "buy " + item.id);
			}
			UI.view = 1;
			UI.viewedItem = Item(item.id);
		}
		Print("*YELLOW*" + to_str(item.cost), x + 220, y + 10 * (i - startIndex), 420);
	}

	Box box = DrawButton("Stop Trading", x, y + 105);
	if (mRange(box)) {
		if (UI.mouseReleased) {
			sendData("COMMAND", "leave");
		}
	}
}

void DrawViewUI() {
	Character C = getCharacter(ID);
	int x = 380;
	int y = 216;
	int w = 240;
	int h = 147;
	if (UI.view == 1) {
		// Viewing an Item
		Item item = UI.viewedItem;
		UI_Item baseItem = getItem(item.id);
		std::string handleTip = "*BLACK*Right Click to Sell.";

		std::string equipStr = "";
		if (C.LEFT == item.index || C.RIGHT == item.index) {
			equipStr = "*BLACK* | *RED*";
			if (C.LEFT == item.index) {
				equipStr += "L";
			}
			if (C.RIGHT == item.index) {
				equipStr += "R";
			}
		}

		if (baseItem.type == "weapon") {
			handleTip += " Double-click to ";
			if (equipStr != "") {
				handleTip += "unequip.";
			}
			else {
				handleTip += "equip.";
			}
			std::string text = "*ORANGE*" + pretty(item.id) + equipStr;
			text += "*BLACK* | *TEAL*" + pretty(baseItem.subclass) + "*BLACK* | *PINK*" + to_str(1 + baseItem.twoHanded) + "H" + "*BLACK* | *YELLOW*" + baseItem.cost + "G";
			Print(text, x, y);

			DrawTabs(viewMenu, x + w/2, y + 11);

			if (viewMenu.index == 0) {
				std::string description = baseItem.description + "\n\n";
				description += "Attacks *PINK*" + to_str(w_attacks(C, item)) + "x*GREY* per turn at a cost of *GREEN*" + to_str(w_AP(C, item)) + " AP *GREY*per attack. ";
				description += "Deals *RED*" + to_str(w_min(C, item)) + "-" + to_str(w_max(C, item)) + "*GREY* damage within *GREEN*" + to_str(w_range(C, item)) + " *GREY*tiles";
				if (w_pen(C, item) > 0) {
					description += ", ignoring *RED*" + to_str(w_pen(C, item)) + "% *GREY*of enemy armor.";
				}
				Print(description, x, y + 30, w);
			}
			else {
				Print("*BLUE*Runes *BLACK*" + to_str(item.runes.size()) + "/" + maxRunes(getCharacter(ID), item), x, y + 30);
			}
		}

		CPrint(handleTip, x + w / 2, 320, w);
	}
	else if (UI.view == 2) {
		// Viewing a Character
		if (CHARACTERS.count(UI.viewedEnemy)) {
			Character C = CHARACTERS[UI.viewedEnemy];
			std::string color = "*RED*";
			Print(color + C.NAME, x, y);
			if (UI.viewedEffect != "") {
				Print("*PINK*" + pretty(UI.viewedEffect), x, 340);
				std::string desc = "blah blah blah";
				Print(desc, x + 75, 340);
			}
		}
	}
	else {
		if (UI.tooltip != "") {
			Tooltip tooltip = TOOLTIPS[UI.tooltip];
			Print("*TEAL*" + tooltip.title, x, y);
			Print(tooltip.description, x, y + 15, w);
		}
	}
	if (UI.view != 0) {
		if (!UI.viewLocked) {
			CPrint("*BLACK*Press CTRL to lock view", x + w / 2, 340, w);
		}
		else {
			Print("*YELLOW*\7", x + w - 10, y);
		}
	}
}

void DrawCharacterUI(std::string id) {
	int x = 450;
	int y = 3;
	int w = 185;
	int h = 208;
	Character C;
	if (CHARACTERS.count(id) == 0) {
		if (GRAVEYARD.count(id) == 0) {
			return;
		}
		C = GRAVEYARD[id];
		C.HP = 0;
	}
	else {
		C = getCharacter(id);
	}

	std::string accentOne = "*PINK*";
	std::string nameColor = "*GREEN*";
	if (id != ID) {
		accentOne = "*PURPLE*";
		nameColor = "*BLUE*";
	}
	Print(nameColor + pretty(C.NAME), x, y);
	Print("*YELLOW*Gold: " + to_str(C.GOLD), x + w - 45, y);

	Print(accentOne + "Level " + to_str(C.LEVEL), x, y + 13);
	Print("*YELLOW*" + to_str(C.XP) + "*GREY*/*GREEN*" + to_str(100 * C.LEVEL) + "*GREY* XP", x + 54, y + 13);
	if (UI.topLocked) {
		Print("*YELLOW*\7", x + w - 55, y + 13);
	}
	Print("*PURPLE*SP: *PINK*" + to_str(C.SP), x + w - 35, y + 13);

	std::vector<std::string> barStats = { "VIT", "DEF", "END", "DEX"};
	std::vector<int> vals = { C.HP, C.ARMOR, C.STAMINA, C.AP };
	std::vector<int> maxVals = { MaxHP(C), MaxArmor(C), MaxStamina(C), MaxAP(C) };
	std::vector<std::string> colors = { "*RED*", "*NAVY*", "*GREEN*", "*YELLOW*"};
	std::vector<std::string> symbols = { "*RED*\2", "*NAVY*\4", "*GREEN*\5", "*YELLOW*\3"};

	for (int i = 0; i < barStats.size(); i++) {
		Box box = Print(symbols[i] + " " + DrawBar(vals[i], maxVals[i], 19, colors[i]), x, y + 25 + 10 * i);
		if (mRange(box)) {
			UI.tooltip = barStats[i];
		}
		if (C.SP > 0 && id == ID) {
			box = Print("*PURPLE*\6", x + (w - 5), y + 25 + 10 * i);
			if (mRange(box)) {
				UI.tooltip = barStats[i];
				if (UI.mouseReleased) {
					sendData("COMMAND", "LEVEL " + barStats[i]);
				}
			}
		}
	}

	std::vector<std::string> stats = { "WEP", "AVD" };
	std::vector<std::string> statStr = { "*TEAL*WEP: " + to_str(C.STATS[WEP]), "*TEAL*AVD: " + to_str(C.STATS[AVD]) };

	for (int i = 0; i < stats.size(); i++) {
		Box box = Print(statStr[i], x + i * 63, y + 66);
		if (mRange(box)) {
			UI.tooltip = stats[i];
		}
		if (C.SP > 0) {
			box = Print("*PURPLE*\6", x + 41 + (i * 63), y + 66);
			if (mRange(box)) {
				UI.tooltip = stats[i];
				if (UI.mouseReleased) {
					sendData("COMMAND", "LEVEL " + stats[i]);
				}
			}
		}
	}

	Print("*BLACK*|", x + 55, y + 66);

	DrawTabs(playerMenu, x + w / 2, y + 77);

	y = y + 96;

	if (playerMenu.index == 0) {
		int itemCount = 0;
		for (auto item : C.INVENTORY) {
			int xPos = x;
			int yPos = y + 10 * itemCount;
			itemCount++;
			std::string color = "";
			std::string command = "EQUIP";
			if (item.second.equipped) {
				color = "*TEAL*";
				command = "REMOVE";
			}
			std::string str = accentOne + padNum(itemCount) + "*GREY*) " + color + pretty(item.second.id);
			Print(str, xPos, yPos);
			int len = measureText(str);
			if (range(UI.mX, UI.mY, xPos, yPos, len, 9)) {
				if (UI.doubleClicked) {
					sendData("COMMAND", command + " " + item.first);
				}
				else if (UI.rightReleased) {
					sendData("COMMAND", "sell " + item.first);
				}
				if (!UI.viewLocked) {
					UI.view = 1;
					UI.viewedItem = item.second;
				}
			}

			UI_Item baseItem = getItem(item.second.id);
			if (baseItem.type == "weapon" || baseItem.type == "staff") {
				xPos += 100;
				std::string postText = "*RED*x" + to_str(baseItem.attacks - item.second.attacks);
				Print(postText, xPos, yPos);

				xPos += 25;
				postText = "*GREEN*" + to_str(baseItem.AP);
				Print(postText, xPos, yPos);
			}
		}
		int num = (10 + (5 * C.BACKPACK)) - itemCount;
		for (int i = itemCount; i < itemCount + num; i++) {
			Print(accentOne + padNum(i + 1) + "*GREY*) *BLACK*---", x, y + 10 * i);
		}
	}
	else if (playerMenu.index == 1) {
		CPrint("*PINK*Buffs", x + w / 2, y);
		CPrint("*RED*Debuffs", x + w / 2, y + 55);

		for (int i = 0; i < C.EFFECTS.size(); i++) {
			CPrint(C.EFFECTS[i].id, x + w / 2, y + 10 * i);
		}

	}
	else if (playerMenu.index == 2) {
		CPrint("*BLUE*Spell List", x + w / 2, y);

	}
	else if (playerMenu.index == 3) {
		if (C.DESCRIPTION == "") {
			if (C.ID == ID) {
				CPrint("*BLACK*You can go to the Barber to give your character a description.", x + w / 2, y, w);
			}
			else {
				CPrint("*BLACK*This character doesn't have a description.", x + w / 2, y, w);
			}
		}
		else {
			Print(C.DESCRIPTION, x, y, w);
		}
	}
	else {
		CPrint("*BLUE*Traits", x + w / 2, y);
	}
}


enum CHAR_SELECTOR {
	SKIN,
	HAIR,
	SHIRT,
	SLEEVES,
	ACCENT_1,
	PANTS,
	ACCENT_2,
	BELT,
	SHOES
};

void DrawCharCreation() {
	int x = 5;
	int y = 5;

	std::vector<int> maxStyles = {
		1, // Skin
		8, // Hair
		2, // Shirt
		4, // Sleeves
		6, // Accent_1
		5, // Pants
		3, // Accent_2
		2, // Belt
		1, // Shoes
	};

	std::vector<std::vector<std::string>> colors = {
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

	Box box = DrawButton("Randomize", x + 75, y);
	if (UI.mouseReleased) {
		if (mRange(box)) {
			UI.randomize = true;
		}
	}

	box = DrawButton("Confirm", x + 75, y + 40);
	if (UI.mouseReleased) {
		if (mRange(box)) {
			std::string code = "";
			for (int i = 0; i < UI.colors.size(); i++) {
				code += (char)(UI.styles[i] + 'a');
				code += (char)(UI.colors[i] + 'a');
			}
			sendData("COMMAND", "Character " + UI.charName + "=" + code);
		}
	}

	fillRect(x + 296, y + 46, 150, 15, getColor("GREY"));
	fillRect(x + 297, y + 47, 148, 13, sf::Color(10, 20, 30));
	Print(UI.charName + '_', x + 300, y + 50, 145);

	if (UI.randomize) {
		UI.randomize = false;
		for (int i = 0; i < UI.colors.size(); i++) {
			UI.colors[i] = rand() % colors[i].size();
		}
		for (int i = 0; i < UI.styles.size(); i++) {
			UI.styles[i] = rand() % maxStyles[i];
		}
		if (UI.styles[PANTS] != 4) {
			UI.styles[ACCENT_2] %= 2;
		}
		if (colors[SKIN][UI.colors[SKIN]] == colors[HAIR][UI.colors[HAIR]]) {
			UI.colors[HAIR]++;
			UI.colors[HAIR] %= colors[HAIR].size();
		}
	}

	DrawCharacter(x, y, UI.styles, UI.colors, 4);
	std::string code = "";
	for (int i = 0; i < UI.colors.size(); i++) {
		code += (char)(UI.styles[i] + 'a');
		code += (char)(UI.colors[i] + 'a');
	}
	DrawCharacter(x + 150, y, code, 1);

	std::vector<std::string> styleTexts = { "Hair", "Shirt", "Sleeves", "Shirt Accent", "Pants", "Pants Accent", "Belt"};
	std::vector<std::string> colorTexts = { "Skin", "Hair", "Shirt", "Sleeves", "Shirt Accent", "Pants", "Pants Accent", "Belt", "Shoes"};
	
	x = 50;
	y = 70;
	CPrint("*GREEN*Styles", x, y);
	y += 10;
	for (int i = 0; i < styleTexts.size(); i++) {
		CPrint(styleTexts[i], x, y + 20 * i);
		CPrint(to_str(1 + UI.styles[i + HAIR]), x, y + 10 + 20 * i);
		Box left = Draw(137, 48, 4, 7, x - 20, y + 10 + 20 * i);
		Box right = Draw(131, 48, 4, 7, x + 16, y + 10 + 20 * i);
		if (UI.mouseReleased) {
			int num = UI.styles[i + HAIR];
			int max = maxStyles[i + HAIR];
			if (mRange(left)) {
				num--;
				if (num < 0) {
					num += max;
				}
			}
			if (mRange(right)) {
				num = (1 + num) % max;
			}
			UI.styles[i + HAIR] = num;
			if (UI.styles[PANTS] != 4) {
				UI.styles[ACCENT_2] %= 2;
			}
		}
	}

	x = 250;
	y = 70;
	CPrint("*GREEN*Colors", x, y);
	y += 10;
	for (int i = 0; i < colorTexts.size(); i++) {
		std::string color = "";
		if (UI.styles[i] == 0) {
			if (i == SLEEVES || i == HAIR || i == BELT || i == ACCENT_1 || i == ACCENT_2) {
				color = "*RED*";
			}
		}
		CPrint(color + colorTexts[i], x, y + 20 * i);
		CPrint(to_str(1 + UI.colors[i]), x, y + 10 + 20 * i);
		Box left = Draw(137, 48, 4, 7, x - 20, y + 10 + 20 * i);
		Box right = Draw(131, 48, 4, 7, x + 16, y + 10 + 20 * i);
		if (UI.mouseReleased) {
			int num = UI.colors[i];
			int max = colors[i].size();
			if (mRange(left)) {
				num--;
				if (num < 0) {
					num += max;
				}
				if (i == HAIR && colors[UI.colors[SKIN]] == colors[num]) {
					num--;
					if (num < 0) {
						num += max;
					}
				}
			}
			if (mRange(right)) {
				num = (1 + num) % max;
			}
			UI.colors[i] = num;
			if (colors[SKIN][UI.colors[SKIN]] == colors[HAIR][UI.colors[HAIR]]) {
				UI.colors[HAIR]++;
				UI.colors[HAIR] %= colors[HAIR].size();
			}
		}
	}

}

void DrawBattle(Battle battle) {
	int x = 0;
	int y = 0;

	static std::vector<std::vector<int>> movementCosts;
	if (updateMovement) {
		updateMovement = false;
		movementCosts = moveCosts(getCharacter(ID), BATTLE);
	}

	std::vector<std::vector<std::string>> characters = {};
	std::vector<std::vector<Hazard>> hazards = {};

	for (int i = 0; i < 12; i++) {
		characters.push_back({});
		hazards.push_back({});
		for (int j = 0; j < 12; j++) {
			characters[i].push_back("");
			hazards[i].push_back(Hazard(0, j, i));
		}
	}

	for (int i = 0; i < battle.hazards.size(); i++) {
		hazards[battle.hazards[i].y][battle.hazards[i].x] = battle.hazards[i];
	}

	for (std::string id : battle.characters) {
		Character C = getCharacter(id);
		characters[C.Y][C.X] = id;
	}

	Print("*RED*" + battle.zone, x, y + 1);
	DrawTabs(combatMenu, 323, y + 10);

	if (battle.round > 0) {
		std::vector<std::string> texts = {
		"END",
		"BRACE",
		"GUARD",
		"FLEE"
		};
		int w = 16 * 12;
		for (int i = 0; i < 4; i++) {
			int len = (measureText(texts[i]) + 8);
			int x1 = ((1 + 2 * i) * w) / 8 - len / 2;
			int y1 = 212;
			if (range(UI.mX, UI.mY, x1 - 4, y1 - 4, len + 8, 15)) {
				UI.tooltip = "BUTTON_" + texts[i];
				if (UI.mouseReleased) {
					if (i == 0) {
						sendData("COMMAND", "end");
					}
					else if (i == 1) {
						sendData("COMMAND", "brace");
					}
					else if (i == 2) {
						UI.guarding = true;
					}
					else if (i == 3) {
						sendData("COMMAND", "flee");
					}
				}
			}
			DrawButton(texts[i], x1, y1);
		}
	}
	else {
		int w = 16 * 12;
		std::string text = "Start the Battle!";
		int len = measureText(text);
		int x1 = w / 4 - len / 2;
		int y1 = 212;
		Box box = DrawButton(text, x1, y1, "*PINK*");
		if (mRange(box)) {
			if (UI.mouseReleased) {
				sendData("COMMAND", "start");
			}
		}
		text = "Go Back";
		len = measureText(text);
		x1 = 3 * (w / 4) - len / 4;
		box = DrawButton(text, x1, y1, "*PINK*");
		if (mRange(box)) {
			if (UI.mouseReleased) {
				sendData("COMMAND", "leave");
			}
		}
	}

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			Hazard hazard = hazards[i][j];
			Terrain terrain = TERRAIN[hazard.index];
			int xPos = (x + j * 16);
			int yPos = y + 11 + (i * 16);
			if (battle.round > 0 && movementCosts[i][j] <= getCharacter(ID).AP) {
				Draw(32, 80, 16, 16, xPos, yPos, 1, sf::Color(135, 155, 0));
			}
			if (characters[i][j] == "") {
				Draw(terrain.sX, terrain.sY, 16, 16, xPos, yPos);
			}
			else {
				Character C = CHARACTERS[characters[i][j]];
				if (C.LOOK != "") {
					DrawCharacter(xPos, yPos, C.LOOK);
				}
				else {
					int sX = C.SX;
					int sY = C.SY;
					Draw(sX, sY, 16, 16, xPos, yPos);
				}
				std::string borderColor = "RED";
				if (C.TEAM == 0) {
					borderColor = "BLUE";
				}
				Draw(80, 144, 16, 16, xPos, yPos, 1, getColor(borderColor));
			}
			if (range(UI.mX, UI.mY, xPos, yPos, 16, 16)) {
				if (UI.rightPressed) {
					sendData("COMMAND", "MOVE " + str(j) + str(i));
				}
				else if (characters[i][j] != "") {
					if (UI.doubleClicked) {
						sendData("COMMAND", "ATTACK " + characters[i][j]);
					}
					else if (!UI.viewLocked) {
						UI.view = 2;
						UI.viewedEnemy = characters[i][j];
					}
				}
			}
		}
	}

	y += 10;

	if (combatMenu.index == 0) {
		Print("*PINK*Enemies", x + 200, y + 10);
		Print("*YELLOW*Allies", x + 200, y + 107);
		for (int i = 0; i < battle.characters.size(); i++) {
			int xPos = x + 200;
			int yPos = y + 20 + (i * 10);
			Character C = getCharacter(battle.characters[i]);
			std::string tag = "*RED*E" + to_str(i + 1);
			if (C.TEAM == 0) {
				tag = "*GREEN*P" + to_str(i + 1);
				if (C.TYPE != "player") {
					tag = "*BLUE*P" + to_str(i + 1);
				}
				yPos += 97;
			}
			Box box = Print(tag + " *GREY*" + C.NAME, xPos, yPos);

			if (!UI.viewLocked && mRange(box)) {
				if (C.TYPE == "player") {
					UI.viewedPlayer = C.ID;
				}
				else if (!UI.viewLocked && mRange(box)) {
					UI.view = 2;
					UI.viewedEnemy = C.ID;
				}
			}

			xPos = x + 300;
			Print(DrawBar(C.HP, MaxHP(C), 8, "*RED*"), xPos, yPos);
		}
	}
	else {
		lootBar.visible = true;
		lootBar.visibleAtOnce = 17;
		lootBar.numThings = battle.loot.size();
		lootBar.dY = y + 29;
		lootBar.dX = x + 420;
		lootBar.height = lootBar.visibleAtOnce * 10;
		DrawScrollbar(lootBar);

		int startIndex = lootBar.index;
		int maxIndex = lootBar.index + lootBar.visibleAtOnce;
		if (maxIndex > lootBar.numThings) {
			maxIndex = lootBar.numThings;
		}
		std::string helpMsg = "*YELLOW*Double click to take items";
		if (battle.loot.size() == 0) {
			helpMsg = "*BLACK*There's no loot here . . .";
		}
		CPrint(helpMsg, 323, y + 18);
		std::vector<Item> items = {};
		for (auto item : battle.loot) {
			items.push_back(item.second);
		}
		for (int i = startIndex; i < maxIndex; i++) {
			Item item = items[i];
			UI_Item baseItem = getItem(item.id);
			std::string text = "*PINK*" + padNum(i) + "*GREY*) *TEAL*" + pretty(item.id);
			int xPos = (x + 200);
			int yPos = (y + 29 + 10 * (i - startIndex));
			int w = measureText(text);
			int h = 9;
			Print(text, xPos, yPos);
			Print("*YELLOW*" + to_str(baseItem.cost), xPos + 200, yPos);
			// Check if player clicks on any of these items
			if (range(UI.mX, UI.mY, xPos, yPos, w, h)) {
				if (UI.doubleClicked) {
					sendData("COMMAND", "TAKE " + item.index);
				}
				else if (!UI.viewLocked) {
					UI.view = 1;
					UI.viewedItem = item;
				}
			}
		}
	}
}

void DrawRoom() {
	int x = 5;
	int y = 5;
	std::string msg = "";
	Location room = getLocation(getCharacter(ID).LOCATION);
	if (room.parent != "") {
		msg = "*RED*" + room.parent + "*GREY* - *BLUE*" + room.id + "*GREY*\n";
	}
	else {
		msg = "*RED*" + room.id + "*GREY*\n";
	}

	msg += room.description + "\n\n";

	std::vector<std::string> names = {};
	for (std::string npc : room.people) {
		std::string color = "*GREEN*";
		NPC character = getNPC(npc);
		if (character.MERCHANT) {
			color = "*YELLOW*";
		}
		names.push_back(color + pretty(npc));
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
	
	Box descBox = Print(msg, 5, 5, 420);

	y += descBox.h + 10;
	
	msg = "";

	if (room.buildings.size() > 0) {
		y = 122;
		Print("*BLUE*Buildings", x, y);
		msg += "*BLUE*Buildings\n";
		for (int i = 0; i < room.buildings.size(); i++) {
			Box box = DrawButton(room.buildings[i], x, y + 10, "YELLOW");
			if (mRange(box)) {
				if (UI.mouseReleased) {
					sendData("COMMAND", "GO " + room.buildings[i]);
				}
			}
			x += box.w + 5;
		}
	}

	x = 300;
	y = 150;
	Print("*GREEN*Players", x, y);
	
	int n = 0;
	for (auto character : CHARACTERS) {
		Character C = character.second;
		if (C.ID != ID && C.LOCATION == room.id) {
			Box box = Print("*PURPLE*" + pretty(C.NAME), x, y + 10 + n++ * 10);
			if (mRange(box)) {
				if (!UI.topLocked) {
					UI.viewedPlayer = C.ID;
				}
			}
		}
	}
	if (n == 0) {
		Print("*BLACK*No one is here.", x, y + 10);
	}

	x = 5;
	y = 160;
	std::vector <std::string> directions = {
		"NORTH",
		"WEST",
		"EAST",
		"SOUTH"
	};
	for (int i = 0; i < 4; i++) {
		for (Connection con : room.connections) {
			if (con.direction == low(directions[i])) {
				std::string color = "*BLUE*";
				if (LOCATIONS[low(con.location)].dungeon) {
					color = "*RED*";
				}
				Box box = DrawButton(con.location, x, y + 10, color);
				if (mRange(box)) {
					if (UI.mouseReleased) {
						sendData("COMMAND", "GO " + con.location);
					}
				}
				Print("*GREEN*" + directions[i], x, y);
				x += box.w + 5;
			}
		}
	}
	if (low(room.id) == "tavern") {
		y = 170;
		x = 5;
		Box box = DrawButton("Rest - *YELLOW*5 Gold", x, y, "PINK");
		if (mRange(box)) {
			if (UI.mouseReleased) {
				sendData("COMMAND", "rest");
			}
		}
	}
	if (low(room.id) == "barber") {
		y = 170;
		x = 5;
		Box box = DrawButton("Haircut - *YELLOW*15 Gold", x, y, "PINK");
		if (mRange(box)) {
			if (UI.mouseReleased) {
				UI.hairCut = true;
				playerMenu.index = 3;
			}
		}
	}
	if (low(room.id) == "graveyard") {
		y = 140;
		x = 5;
		Box box = DrawButton("View Graves", x, y, "PINK");
		if (mRange(box)) {
			if (UI.mouseReleased) {
				UI.viewingGraves = true;
			}
		}
	}
	
	if (room.dungeon || room.parent != "") {
		y = 205;
		x = 5;
		std::string command = "DELVE";
		std::string color = "RED";
		if (room.parent != "") {
			command = "LEAVE";
			color = "ORANGE";
		}
		Box box = DrawButton(command, x, y, color);
		if (mRange(box)) {
			if (UI.mouseReleased) {
				sendData("COMMAND", command);
			}
		}
	}
}


void DrawLogs() {
	int y = 244;
	int x = 347;

	logBar.visible = true;
	logBar.dX = x;
	logBar.dY = y;
	logBar.numThings = min(20, logs.size());
	logBar.visibleAtOnce = 7;
	DrawScrollbar(logBar);

	int startIndex = logBar.index;
	int maxIndex = logBar.index + logBar.visibleAtOnce;
	if (maxIndex > logs.size()) {
		maxIndex = logs.size();
	}
	for (int i = 1; i <= 7; i++) {
		if (logs.size() >= (i + logBar.index)) {
			Print(logs[logs.size() - (i + logBar.index)], 5, HEIGHT - (11 + i * 15), 340);
		}
	}

	if (logBar.index == 0) {
		Print("> " + input + "_", 5, HEIGHT - 11, 350);
	}
	else {
		Print("*BLACK*Viewing old logs, scroll down to see latest. . .", 5, HEIGHT - 11, 340);
	}
}

void DrawUI() {
	UI.creatingCharacter = false;
	if (CHARACTERS.count(ID) == 0) {
		UI.creatingCharacter = true;
		DrawCharCreation();
	}
	else {
		if (UI.hairCut) {
			DrawHaircut();
		}
		else if (UI.viewingGraves) {
			DrawGraves();
		}
		else if (getCharacter(ID).LOCATION == BATTLE.id) {
			DrawBattle(BATTLE);
		}
		else if (getCharacter(ID).TRADING != "") {
			DrawTrade();
		}
		else {
			DrawRoom();
		}
		DrawCharacterUI(UI.viewedPlayer);
		DrawViewUI();
		if (!UI.viewingGraves) {
			DrawLogs();
		}
	}
}