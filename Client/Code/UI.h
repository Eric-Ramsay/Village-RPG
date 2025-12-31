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
	
	int startIndex;
	int maxIndex;
	setScrollBar(graveBar, x + 250, y + 34, graveList.size(), startIndex, maxIndex);

	DrawSection(x, y + 10, 225, 15);
	Print(UI.graveSearch + '_', x + 4, y + 14, 145);

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
	int w = 420;
	
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
	std::unordered_map<std::string, std::vector<std::string>> itemTypes = {};
	for (std::string itemId : npc.ITEMS) {
		UI_Item item = getItem(itemId);
		itemTypes[item.type].push_back(item.id);
	}

	tradeMenu.tabs = {};
	for (auto type : itemTypes) {
		tradeMenu.tabs.push_back(pretty(type.first));
	}
	measureTab(&tradeMenu);
	DrawTabs(tradeMenu, x + w / 2, y);

	std::string tab = low(tradeMenu.tabs[tradeMenu.index]);

	y = 130;
	int startIndex;
	int maxIndex;
	setScrollBar(tradeBar, x + w - 30, y, itemTypes[tab].size(), startIndex, maxIndex);
	for (int i = startIndex; i < maxIndex; i++) {
		UI_Item item = getItem(itemTypes[tab][i]);
		Box box = Print("*PINK*" + padNum(i + 1) + "*GREY*) " + item.name, x, y + 10 * (i - startIndex), 420);
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
	int x = 420;
	int y = 206;
	int w = 214;
	int h = 157;
	if (UI.view == 1) {
		// Viewing an Item
		Item item = UI.viewedItem;
		UI_Item baseItem = getItem(item.id);
		bool owned = C.INVENTORY.count(item.index) > 0;

		std::string handleTip = "*BLACK*";
		if (owned) {
			handleTip + "Right Click to sell.";
		}

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
			handleTip += "Double-click to ";
			if (!owned) {
				handleTip += "take.";
			}
			else if (equipStr != "") {
				handleTip += "unequip";
			}
			else {
				handleTip += "equip";
			}

			std::string text = "*ORANGE*" + pretty(baseItem.name) + equipStr;
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

		CPrint(handleTip, x + w / 2, 335, w);
	}
	else if (UI.view == 2) {
		// Viewing a Character
		if (CHARACTERS.count(UI.viewedEnemy)) {
			Character C = CHARACTERS[UI.viewedEnemy];
			std::string color = "*RED*";
			Print(color + C.NAME, x, y);
		}
	}
	else if (UI.view == 3) {
		Hazard hazard = UI.viewedHazard;
		Terrain terrain = TERRAIN[hazard.index];
		Box titleBox = Print("*BLUE*" + terrain.name, x, y);
		Draw(terrain.sX, terrain.sY, 16, 16, x + titleBox.w + 5, y - 7);

		std::string description = "";
		if (hazard.duration < 999) {
			description += "*RED*" + to_str(hazard.duration) + " Turns*GREY*\n\n";
		}
		if (terrain.moveCost == 999) {
			description += "*BLACK*This tile is impassable.\n";
		}
		else {
			description += "*GREY*It costs *YELLOW*" + to_str(terrain.moveCost) + " AP*GREY* to cross this tile.\n";
		}

		if (terrain.hp > 0) {
			description += "*GREY*Can be removed if hit for *ORANGE*" + to_str(terrain.hp) + "+*GREY* damage.\n";
		}

		if (terrain.damage > 0) {
			description += "Deals *ORANGE*" + to_str(terrain.damage) + "*GREY* damage per turn.\n";
		}

		Box box = Print(description, x, y + 12);

		if (terrain.effects.size() > 0) {
			int yPos = y + 24 + box.h;
			Print("*PINK*Effects", x, yPos);
			CPrint("*YELLOW*Turns", x + 2 * w / 5, yPos);
			CPrint("*PURPLE*Stacks", x + 2 * w / 3, yPos);
			int num = 1;
			for (Effect effect : terrain.effects) {
				UI_Effect baseEffect = getEffect(effect.id);
				int yPos = y + 24 + box.h + 11 * num;
				num++;
				std::string color = "*RED*";
				if (baseEffect.type == "buff") {
					color = "*GREEN*";
				}
				Box effectBox = Print(color + pretty(effect.id), x, yPos);
				if (mRange(effectBox)) {
					UI.viewedEffect = effect.id;
				}
				CPrint("*YELLOW*" + to_str(effect.turns), x + 2 * w / 5, yPos);
				CPrint("*PURPLE*" + to_str(effect.stacks), x + 2 * w / 3, yPos);
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
			CPrint("*BLACK*Press CTRL to lock view", x + w / 2, 349, w);
		}
		else {
			Print("*YELLOW*\7", x + w - 10, y);
		}
	}
	if (UI.viewedEffect != "") {
		CPrint("*TEAL*" + EFFECTS[UI.viewedEffect].desc, x + w / 2, 349, w);
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
	Print("*YELLOW*Gold: " + to_str(C.GOLD), x + w - 50, y);

	Print(accentOne + "Level " + to_str(C.LEVEL), x, y + 13);
	Print("*YELLOW*" + to_str(C.XP) + "*GREY*/*GREEN*" + to_str(100 * C.LEVEL) + "*GREY* XP", x + 57, y + 13);
	if (UI.topLocked) {
		Print("*YELLOW*\7", x + w - 55, y + 13);
	}
	Print("*PURPLE*SP: *PINK*" + to_str(C.SP), x + w - 35, y + 13);

	std::vector<std::string> barStats = { "VIT", "END", "DEX"};
	std::vector<int> vals = { C.HP, C.STAMINA, C.AP };
	std::vector<int> maxVals = { MaxHP(C), MaxStamina(C), MaxAP(C) };
	std::vector<std::string> colors = { "*RED*", "*GREEN*", "*YELLOW*"};
	std::vector<std::string> symbols = { "*RED*\2", "*GREEN*\5", "*YELLOW*\3"};

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

	std::vector<std::string> stats = { "WEP", "AVD", "DEF"};
	std::vector<std::string> statStr = { "*TEAL*WEP*PALE* " + to_str(C.STATS[WEP]), "*TEAL*AVD*PALE* " + to_str(C.STATS[AVD]) };

	std::vector<int> statX = { x + 41, x + 99, x + 176 };

	Box box;
	for (int i = 0; i < stats.size(); i++) {
		if (C.SP > 0) {
			box = DrawSymbol(SP, statX[i], y + 56, "PURPLE");
			if (mRange(box)) {
				UI.tooltip = stats[i];
				if (UI.mouseReleased) {
					sendData("COMMAND", "LEVEL " + stats[i]);
				}
			}
		}
		if (i < statStr.size()) {
			box = Print(statStr[i], x + 1 + (58 * i), y + 56);
			if (mRange(box)) {
				UI.tooltip = stats[i];
			}
		}
	}

	Print("*BLACK*|", x + 53, y + 56);
	Print("*BLACK*|", x + 111, y + 56);

	DrawSymbol(DEFENSE, x + 117, y + 56, "NAVY");
	DrawSymbol(ARMOR, x + 146, y + 56, "NAVY");
	Print("*PALE*" + to_str(Armor(C)), x + 131, y + 56);
	Print("*PALE*" + to_str(Defense(C)), x + 160, y + 56);
	if (mRange(Box(x + 117, y + 56, 58, 7))) {
		UI.tooltip = "DEF";
	}

	DrawTabs(playerMenu, x + w / 2, y + 67);

	y += 86;

	if (playerMenu.index == 0) {
		int itemCount = 0;
		for (auto item : C.INVENTORY) {
			UI_Item baseItem = getItem(item.second.id);
			int xPos = x;
			int yPos = y + 10 * itemCount;
			itemCount++;
			std::string color = "";
			std::string command = "EQUIP";
			if (item.second.equipped) {
				color = "*TEAL*";
				command = "REMOVE";
			}
			std::string str = accentOne + padNum(itemCount) + "*GREY*) " + color + baseItem.name;
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
		if (C.EFFECTS.size() == 0) {
			CPrint("*BLACK*You have no active effects.\n\nTip: You can hover an effect's name to see what it does.", x + w / 2, y, w);
		}
		else {
			Print("*PINK*Effects", x, y);
			CPrint("*YELLOW*Turns", x + 2 * w / 5, y);
			CPrint("*PURPLE*Stacks", x + 2 * w / 3, y);

			int numEffects = 0;
			for (int i = 0; i < 2; i++) {
				for (Effect effect : C.EFFECTS) {
					UI_Effect baseEffect = getEffect(effect.id);
					if ((i == 0 && baseEffect.type == "buff") || (i == 1 && baseEffect.type == "debuff")) {
						numEffects++;
						int yPos = y + 10 * numEffects;
						std::string color = "*GREEN*";
						if (baseEffect.type == "debuff") {
							color = "*RED*";
						}
						Box effectBox = Print(color + pretty(effect.id), x, yPos);
						if (mRange(effectBox)) {
							UI.viewedEffect = effect.id;
						}
						CPrint("*YELLOW*" + to_str(effect.turns), x + 2 * w / 5, yPos);
						CPrint("*PURPLE*" + to_str(effect.stacks), x + 2 * w / 3, yPos);
					}
				}
			}
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
			UI.colors[i] = random(colors[i].size());
		}
		for (int i = 0; i < UI.styles.size(); i++) {
			UI.styles[i] = random(maxStyles[i]);
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

void DrawCharSection(Character C, int x, int y, std::string tag) {
	Box box = DrawSection(x, y, 191, 24, "BLACK");
	if (!UI.viewLocked && mRange(box)) {
		if (C.TYPE == "player") {
			UI.viewedPlayer = C.ID;
		}
		else if (!UI.viewLocked && mRange(box)) {
			UI.view = 2;
			UI.viewedEnemy = C.ID;
		}
	}
	Print(tag + " *GREY*" + C.NAME, x + 3, y + 3);
	DrawSymbol(HEART, x + 3, y + 14, "RED");
	Print(DrawBar(C.HP, MaxHP(C), 8, "*RED*"), x + 13, y + 14);
	DrawSymbol(BUFF, x + 145, y + 3, "GREEN");
	DrawSymbol(DEBUFF, x + 168, y + 3, "RED");
	DrawSymbol(DEFENSE, x + 145, y + 14, "NAVY");
	DrawSymbol(ARMOR, x + 168, y + 14, "NAVY");
	int buffs = 0;
	int debuffs = 0;
	for (Effect e : C.EFFECTS) {
		UI_Effect effect = getEffect(e.id);
		if (effect.type == "buff") {
			buffs++;
		}
		else {
			debuffs++;
		}
	}
	Print("*PALE*" + to_str(buffs), x + 154, y + 3);
	Print("*PALE*" + to_str(debuffs), x + 177, y + 3);
	Print("*PALE*" + to_str(Defense(C)), x + 154, y + 14);
	Print("*PALE*" + to_str(Armor(C)), x + 177, y + 14);
}

void DrawBattle(Battle battle) {
	int x = battleX;
	int y = battleY;
	int w = 16 * 12;
	std::vector<std::string> enemies = {};
	std::vector<std::string> allies = {};

	static std::vector<std::vector<int>> movementCosts;
	if (updateMovement) {
		updateMovement = false;
		movementCosts = moveCosts(getCharacter(ID), battle);
	}

	combatMenu.tabs[1] = "LOOT (" + to_str(battle.loot.size()) + ")";
	measureTab(&combatMenu);

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

	int allyHp = 0;

	int enemyHp = 0;

	for (std::string id : battle.characters) {
		if (CHARACTERS.count(id) > 0) {
			Character C = CHARACTERS[id];
			characters[C.Y][C.X] = id;
			if (CHARACTERS[id].TEAM == 0) {
				allies.push_back(id);
				allyHp += C.HP;
			}
			else {
				enemies.push_back(id);
				enemyHp += C.HP;
			}
		}
	}

	CPrint("*RED*" + battle.zone, x + 8 * 12, y + 1);
	CPrint(DrawBar(allyHp, allyHp + enemyHp, 30, "*GREEN*", false, "*GREY*", "*RED*", false), x + w / 2, y + 10);

	y += 20;

	if (battle.round > 0) {
		std::vector<std::string> texts = {
		"END",
		"BRACE",
		"GUARD",
		"FLEE"
		};
		for (int i = 0; i < 4; i++) {
			int len = (measureText(texts[i]) + 8);
			int x1 = x  + ((1 + 2 * i) * w) / 8 - len / 2;
			int y1 = y + 16 * 12 + 10;
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
		int x1 = x + w / 4 - len / 2;
		int y1 = y + 16 * 12 + 10;
		Box box = DrawButton(text, x1, y1, "*PINK*");
		if (mRange(box)) {
			if (UI.mouseReleased) {
				sendData("COMMAND", "start");
			}
		}
		text = "Go Back";
		len = measureText(text);
		x1 = x + 3 * (w / 4) - len / 4;
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
			int yPos = y + (i * 16);
			if (battle.round > 0 && movementCosts[i][j] <= getCharacter(ID).AP) {
				Draw(32, 80, 16, 16, xPos, yPos, 1, sf::Color(135, 155, 0));
			}
			//CPrint(to_str(movementCosts[i][j]), xPos + 8, yPos + 4);
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
				else if (!UI.viewLocked && hazard.index > 0) {
					UI.viewedHazard = hazard;
					UI.view = 3;
				}
			}
		}
	}

	x = battleMenuX;
	y = battleMenuY;

	DrawTabs(combatMenu, battleX/2, y);

	if (combatMenu.index == 0) {
		y += 19;
		Print("*YELLOW*Allies", x, y);
		Print("*PINK*Enemies", x, y + 90);

		int startIndex;
		int maxIndex;
		setScrollBar(enemyBar, x + 200, y + 100, enemies.size(), startIndex, maxIndex);
		for (int i = startIndex; i < maxIndex; i++) {
			Character C = getCharacter(enemies[i]);
			int yPos = y + 100 + (i - startIndex) * 25;
			std::string tag = "*RED*E" + to_str(i + 1);
			DrawCharSection(C, x, yPos, tag);
		}

		setScrollBar(allyBar, x + 200, y + 10, allies.size(), startIndex, maxIndex);
		for (int i = startIndex; i < maxIndex; i++) {
			Character C = getCharacter(allies[i]);
			int yPos = y + 10 + (i - startIndex) * 25;
			std::string tag = "*GREEN*P" + to_str(i + 1);
			if (C.TYPE != "player") {
				tag = "*BLUE*P" + to_str(i + 1);
			}
			DrawCharSection(C, x, yPos, tag);
		}
	}
	else {
		int startIndex;
		int maxIndex;
		setScrollBar(lootBar, x + 200, y + 29, battle.loot.size(), startIndex, maxIndex);

		std::string helpMsg = "*YELLOW*Double click to take items";
		if (battle.loot.size() == 0) {
			helpMsg = "*BLACK*There's no loot here . . .";
		}
		CPrint(helpMsg, x + 100, y + 18);
		std::vector<Item> items = {};
		for (auto item : battle.loot) {
			items.push_back(item.second);
		}
		for (int i = startIndex; i < maxIndex; i++) {
			Item item = items[i];
			UI_Item baseItem = getItem(item.id);
			std::string text = "*PINK*" + padNum((i + 1)) + "*GREY*) *TEAL*" + pretty(baseItem.name);
			int xPos = x;
			int yPos = (y + 29 + 10 * (i - startIndex));
			int w = measureText(text);
			int h = 9;
			Print(text, xPos, yPos);
			Print("*YELLOW*" + to_str(baseItem.cost), xPos + 180, yPos);
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

	for (int i = 0; i < animations.size(); i++) {
		if (animations[i].type == "text") {
			int xPos = animations[i].position.x;
			int yPos = animations[i].position.y;
			CPrint(animations[i].color + animations[i].text, xPos, yPos, WIDTH, 1, animations[i].opacity);
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
	int y = 258;
	int x = 347;

	int startIndex;
	int maxIndex;
	setScrollBar(logBar, x, y, logs.size(), startIndex, maxIndex);

	for (int i = 1 + startIndex; i <= maxIndex; i++) {
		Print(logs[logs.size() - i], 5, HEIGHT - (11 + (i - startIndex) * 15), 340);
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
		else if (BATTLES.count(CHARACTERS[ID].LOCATION) > 0) {
			DrawBattle(BATTLES[CHARACTERS[ID].LOCATION]);
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