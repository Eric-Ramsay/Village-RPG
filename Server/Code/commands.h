#pragma once
std::string commandTravel(int playerIndex, Character& C, std::vector<std::string> words) {
	if (words.size() == 0 || words[0].size() == 0) {
		return "*RED*Command not understood. . . ";
	}
	if (BATTLES.count(C.LOCATION) > 0) {
		return "*RED*You must leave this battle first!\n";
	}
	std::string direction = "";
	Location location = getLocation(C.LOCATION);
	if (words[0] == "to") {
		words.erase(words.begin());
	}
	Location destination = getLocation(join(words));
	if (destination.id == "error") {
		if (words[0] == "the") {
			words.erase(words.begin());
			destination = getLocation(join(words));
		}
	}
	if (destination.id != "error") {
		C.TRADING = "";
		sendStat(C.ID, "TRADING", "", { playerIndex });
		C.LOCATION = destination.id;
		return "You travel to the " + destination.id;
	}
	if (words[0][0] == 'n') {
		direction = "north";
	}
	if (words[0][0] == 's') {
		direction = "south";
	}
	if (words[0][0] == 'w') {
		direction = "west";
	}
	if (words[0][0] == 'e') {
		direction = "east";
	}
	if (direction != "") {
		for (Connection con : location.connections) {
			if (low(con.direction) == direction) {
				C.TRADING = "";
				sendStat(C.ID, "TRADING", "", { playerIndex });
				C.LOCATION = con.location;
				return "You travel " + direction + ".";
			}
		}
	}
	return "*RED*You can't go that way . . .";
}

std::string commandDelve(int playerIndex, Character& C, std::vector<std::string> words) {
	if (BATTLES.count(C.LOCATION) > 0) {
		return "*RED*You're already in a battle!";
	}
	std::string id = "";
	for (auto battle : BATTLES) {
		if (C.LOCATION == battle.second.zone && battle.second.round == 0) {
			id = battle.second.id;
		}
	}
	if (id == "") {
		id = C.LOCATION + to_str(rand() % 9999);
		BATTLES[id] = Battle(id, C.LOCATION);
	}
	C.LOCATION = id;
	BATTLES[id].teams[0].push_back(C.ID);
	save(BATTLES[id]);
	sendBattle(BATTLES[id], { playerIndex });
	return "*YELLOW*You delve deeper. . .";
}

std::string commandStart(int playerIndex, Character& C) {
	if (BATTLES.count(C.LOCATION) == 0) {
		return "*RED*You must be in combat!";
	}

	if (BATTLES[C.LOCATION].round > 0) {
		return "*RED*The battle already started!";
	}

	startBattle(BATTLES[C.LOCATION]);
	return "";
}

std::string commandAttack(int playerIndex, Character& C, std::vector<std::string> words) {
	std::string args = low(join(words));
	if (BATTLES.count(C.LOCATION) == 0) {
		return "*RED*You must be in combat to attack.";
	}
	std::string wepId = C.LEFT;
	// Determine Weapon to Attack with
	if (C.LEFT == "" && C.RIGHT == "") {
		return "*RED*You must equip a weapon!";
	}
	if (C.INVENTORY[C.LEFT].attacks == 0 && C.INVENTORY[C.RIGHT].attacks == 0) {
		return "*RED*You can't make any more attacks!";
	}
	UI_Item left = getItem(C.LEFT);
	UI_Item right = getItem(C.RIGHT);
	if (C.LEFT == "" || C.INVENTORY[C.LEFT].attacks == 0 || left.AP > C.AP) {
		wepId = C.RIGHT;
	}
	else {
		if (C.INVENTORY[C.RIGHT].attacks > 0 && right.range > left.range) {
			wepId = C.RIGHT;
		}
	}

	UI_Item item = getItem(C.INVENTORY[wepId].key);
	if (item.AP > C.AP) {
		return "*RED*You don't have enough AP!";
	}

	int x = C.X;
	int y = C.Y;
	Battle battle = BATTLES[C.LOCATION];
	std::string target = "";
	bool found = false;
	for (std::string enemyId : battle.teams[1]) {
		Character enemy = CHARACTERS[enemyId];
		int eX = enemy.X;
		int eY = enemy.Y;
		if (low(enemy.NAME) == args) {
			found = true;
			if (atkDist(x, y, eX, eY) <= item.range) {
				target = enemyId;
				break;
			}
		}
	}
	if (target == "") {
		if (found) {
			return "*RED*That enemy is too far to attack.";
		}
		return "*RED*Couldn't find enemy '*GREY*" + args + "*RED*'";
	}

	setStat(C, "AP", C.AP - item.AP);
	C.INVENTORY[wepId].attacks--;
	sendItem(C.ID, C.INVENTORY[wepId]);
	std::string msg = dealDamage(item.attack, C.ID, target, battle.teams[0], battle.teams[1]).msg + "\n";
	msg += handleCombat(battle.id);
	return msg;
}

std::string commandLeave(int playerIndex, Character& C, std::vector<std::string> words) {
	if (C.TRADING != "") {
		C.TRADING = "";
		sendStat(C.ID, "TRADING", "", { playerIndex });
		return "";
	}
	std::string id = C.LOCATION;
	if (BATTLES.count(id) > 0) {
		if (BATTLES[id].round == 0) {
			C.LOCATION = BATTLES[id].zone;
			validateBattle(id);
			return "*YELLOW*You leave the battle . . .";
		}
	}
	Location loc = getLocation(C.LOCATION);
	if (loc.parent != "") {
		C.LOCATION = loc.parent;
		return "You travel to the " + loc.parent;
	}
	return "*RED*You can't leave from here . . .";
}

std::string commandRemove(int playerIndex, Character& C, std::vector<std::string> words) {
	std::string args = low(join(words));
	std::vector<std::string> ids = findItem(args, C.INVENTORY);
	for (std::string id : ids) {
		C.INVENTORY[id].equipped = false;
		sendStat(C.ID, "DEQUIP", id);
		return "You unequip the *GREEN*" + pretty(C.INVENTORY[id].key);
	}
	return "*RED*Unable to equip '" + args + "'";
}

std::string commandEquip(int playerIndex, Character& C, std::vector<std::string> words) {
	std::string msg = "";
	std::string args = low(join(words));
	std::vector<std::string> ids = findItem(args, C.INVENTORY);
	for (std::string id : ids) {
		if (!C.INVENTORY[id].equipped) {
			UI_Item item = getItem(C.INVENTORY[id].key);
			std::string type = item.type;
			if (type == "armor") {
				for (auto compItem : C.INVENTORY) {
					std::string compType = getItem(compItem.second.key).type;
					if (type == "armor" && compType == "armor") {
						C.INVENTORY[compItem.first].equipped = false;
					}
				}
			}
			else if (type == "weapon" || type == "staff") {
				bool swapLeft = item.twoHanded || (C.INVENTORY.count(C.LEFT) > 0 && C.INVENTORY.count(C.RIGHT) > 0);
				bool swapRight = item.twoHanded || !swapLeft;
				if (swapLeft) {
					if (C.INVENTORY.count(C.LEFT) > 0) {
						C.INVENTORY[C.LEFT].equipped = false;
						sendStat(C.ID, "DEQUIP", C.LEFT);
					}
					setStat(C, "LEFT", id);
				}
				if (swapRight) {
					if (C.INVENTORY.count(C.RIGHT) > 0) {
						C.INVENTORY[C.RIGHT].equipped = false;
						sendStat(C.ID, "DEQUIP", C.RIGHT);
					}
					setStat(C, "RIGHT", id);
				}
				C.INVENTORY[id].equipped = true;
				sendStat(C.ID, "EQUIP", id);
				msg = "You equip the *GREEN*" + item.id;
				break;
			}
			else {
				return "*RED*You can't equip that.";
			}
		}
	}
	return msg;
}

std::string commandBuy(int playerIndex, Character& C, std::vector<std::string> words) {
	if (C.TRADING == "") {
		return "*RED*You must trade with a merchant first.";
	}
	std::string args = low(join(words));
	if (C.BACKPACK && args == "backpack") {
		return "*RED*You already have a backpack.";
	}

	NPC npc = getNPC(C.TRADING);
	bool found = false;
	for (std::string word : npc.ITEMS) {
		if (word == args) {
			found = true;
			break;
		}
	}
	if (!found) {
		return "*RED*This merchant isn't selling that item.";
	}
	UI_Item item = getItem(args);
	if (C.GOLD < item.cost) {
		return "*RED*You don't have enough gold to buy that.";
	}

	if (canTake(C, args)) {
		Item newItem(args);
		C.INVENTORY[newItem.index] = newItem;
		C.GOLD -= item.cost;
		sendItem(C.ID, newItem);
		sendStat(C.ID, "GOLD", C.GOLD);
		return "*GREEN*You buy the *RED*" + pretty(args) + "*GREEN* for *YELLOW*" + item.cost + " gold*GREEN*. You have *YELLOW*" + C.GOLD + " gold*GREEN* left.\n";
	}
	return "*RED*You don't have room in your inventory to buy that!\n";
}

std::string createCharacter(int playerIndex, std::string name) {
	std::time_t t = std::time(0);
	Character newCharacter;
	std::string id = players[playerIndex].USERNAME + " " + name + " " + to_str(t) + to_str(rand() % 99);

	newCharacter.GOLD = 30;
	newCharacter.HP = 30;
	newCharacter.ID = id;
	newCharacter.NAME = name;
	newCharacter.USER = players[playerIndex].USERNAME;

	players[playerIndex].ID = id;
	CHARACTERS[id] = newCharacter;
	
	sendCharacter(newCharacter);
	return id;
}

void commandTrade(int playerIndex, Character& C, std::string id) {
	NPC npc = getNPC(id);
	if (low(npc.NAME) == low(id)) {
		C.TRADING = npc.NAME;
		sendStat(C.ID, "TRADING", npc.NAME, { playerIndex });
	}
}

void command(std::string input, int playerIndex) {
	std::string id = players[playerIndex].ID;
	std::string msg = "";
	std::vector<std::string> words = split(low(input));
	std::string keyword = words[0];
	words.erase(words.begin());
	if (keyword == "character") {
		if (words.size() == 0) {
			msg = "*RED*Please enter a character name!";
		}
		else {
			id = createCharacter(playerIndex, words[0]);
			players[playerIndex].ID = id;
		}
	}
	else if (keyword == "go" || keyword == "enter" || keyword == "travel") {
		msg = commandTravel(playerIndex, CHARACTERS[id], words);
		sendStat(id, "LOCATION", CHARACTERS[id].LOCATION);
	}
	else if (keyword == "delve") {
		msg = commandDelve(playerIndex, CHARACTERS[id], words);
		sendStat(id, "LOCATION", CHARACTERS[id].LOCATION);
	}
	else if (keyword == "start") {
		msg = commandStart(playerIndex, CHARACTERS[id]);
	}
	else if (keyword == "equip") {
		msg = commandEquip(playerIndex, CHARACTERS[id], words);
	}
	else if (keyword == "remove" || keyword == "dequip") {
		msg = commandRemove(playerIndex, CHARACTERS[id], words);
	}
	else if (keyword == "attack") {
		msg = commandAttack(playerIndex, CHARACTERS[id], words);
	}
	else if (keyword == "trade") {
		std::string trading = CHARACTERS[id].TRADING;
		if (words.size() > 0) {
			commandTrade(playerIndex, CHARACTERS[id], words[0]);
		}
		if (trading == CHARACTERS[id].TRADING) {
			msg = "*RED*Command not understood.";
		}
	}
	else if (keyword == "buy") {
		msg = commandBuy(playerIndex, CHARACTERS[id], words);
	}
	else if (keyword == "end") {
		if (BATTLES.count(CHARACTERS[id].LOCATION) > 0) {
			setStat(CHARACTERS[id], "ENDED", true);
			msg = handleCombat(CHARACTERS[id].LOCATION);
		}
		else {
			msg = "*RED*You must be in combat. Did you mean to *YELLOW*quit*RED* the game?";
		}
	}
	else if (keyword == "leave" || keyword == "back" || keyword == "stop") {
		msg = commandLeave(playerIndex, CHARACTERS[id], words);
		sendStat(id, "LOCATION", CHARACTERS[id].LOCATION);
	}
	else if (keyword == "move") {
		int x = readInt(words[0]);
		int y = readInt(words[0]);
		if (x >= 0 && x < 12) {
			CHARACTERS[id].X = x;
			sendStat(id, "X", CHARACTERS[id].X);
		}
		if (y >= 0 && y < 12) {
			CHARACTERS[id].Y = y;
			sendStat(id, "Y", CHARACTERS[id].Y);
		}
	}
	else if (keyword == "hp") {
		if (words.size() > 0) {
			int num = std::stoi(words[0]);
			CHARACTERS[id].HP = num;
			sendStat(id, "HP", CHARACTERS[id].HP);
		}
	}
	else if (keyword == "gold") {
		CHARACTERS[id].GOLD += 25;
		sendStat(id, "GOLD", CHARACTERS[id].GOLD);
	}
	else if (keyword == "suicide") {
		msg = "*RED*You've lost the will to go on. . .";
		removeCharacter(CHARACTERS[id]);
	}
	std::cout << msg << std::endl;
	if (CHARACTERS.count(id) > 0) {
		save(CHARACTERS[id]);
	}
	if (msg != "") {
		sendData("TEXT", msg, { playerIndex });
	}
}
