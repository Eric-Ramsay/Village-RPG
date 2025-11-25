#pragma once
std::string commandTravel(int playerIndex, Character& C, std::vector<std::string> words) {
	if (words.size() == 0 || words[0].size() == 0) {
		return "*RED*Command not understood. . . ";
	}
	std::string battleId = "";
	if (BATTLES.count(C.LOCATION) > 0) {
		if (BATTLES[C.LOCATION].round > 0) {
			return "*RED*You must flee the battle first!\n";
		}
		battleId = C.LOCATION;
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
		setStat(C, "LOCATION", destination.id);
		if (battleId != "") {
			validateBattle(battleId);
		}
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
				setStat(C, "LOCATION", con.location);
				if (battleId != "") {
					validateBattle(battleId);
				}
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
	setStat(C, "LOCATION", id);
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
	std::string args = join(words);
	if (BATTLES.count(C.LOCATION) == 0) {
		return "*RED*You must be in combat to attack.";
	}
	// Determine Weapon to Attack with
	if (C.RIGHT != "" && C.INVENTORY.count(C.RIGHT) == 0) {
		C.RIGHT = "";
	}
	if (C.LEFT != "" && C.INVENTORY.count(C.LEFT) == 0) {
		C.LEFT = "";
	}
	if (C.LEFT == "" && C.RIGHT == "") {
		return "*RED*You must equip a weapon!";
	}

	std::string leftItem = C.LEFT;
	std::string rightItem = C.RIGHT;
	
	if (leftItem == "") {
		leftItem = C.RIGHT;
	}
	if (rightItem == "") {
		rightItem = C.LEFT;
	}

	if (C.INVENTORY[leftItem].attacks == 0 && C.INVENTORY[rightItem].attacks == 0) {
		return "*RED*You can't make any more attacks!";
	}

	std::string wepId = leftItem;

	UI_Item left = getItem(C.INVENTORY[leftItem].id);
	UI_Item right = getItem(C.INVENTORY[rightItem].id);
	if (C.LEFT == "" || C.INVENTORY[leftItem].attacks == 0 || left.AP > C.AP) {
		wepId = rightItem;
	}
	else if (C.INVENTORY[rightItem].attacks > 0 && right.range > left.range) {
		wepId = rightItem;
	}

	UI_Item item = getItem(C.INVENTORY[wepId].id);
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
		if (low(enemy.ID) == args || low(enemy.NAME) == args) {
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

	std::string msg = dealDamage(item.attack, C.ID, target, battle.teams[0], battle.teams[1]).msg;
	sendText(msg, battleIndices(battle));

	handleCombat(battle.id);
	return "";
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
			setStat(C, "LOCATION", BATTLES[id].zone);
			validateBattle(id);
			return "*YELLOW*You leave the battle . . .";
		}
	}
	Location loc = getLocation(C.LOCATION);
	if (loc.parent != "") {
		setStat(C, "LOCATION", loc.parent);
		return "You travel to the " + loc.parent;
	}
	return "*RED*You can't leave from here . . .";
}

std::string commandRemove(int playerIndex, Character& C, std::vector<std::string> words) {
	std::string args = join(words);
	std::vector<std::string> ids = findItem(args, C.INVENTORY);
	std::string changes = str(C.ID);
	for (std::string id : ids) {
		C.INVENTORY[id].equipped = false;
		changes += addLine("DEQUIP", id);
		if (C.LEFT == id) {
			C.LEFT = "";
			changes += addLine("LEFT", "");
		}
		if (C.RIGHT == id) {
			C.RIGHT = "";
			changes += addLine("RIGHT", "");
		}
		sendData("STATS", changes);
		return "You unequip the *GREEN*" + pretty(C.INVENTORY[id].id);
	}
	return "*RED*Unable to equip '" + args + "'";
}

std::string commandEquip(int playerIndex, Character& C, std::vector<std::string> words) {
	std::string args = join(words);
	std::vector<std::string> ids = findItem(args, C.INVENTORY);
	for (std::string id : ids) {
		if (!C.INVENTORY[id].equipped) {
			UI_Item item = getItem(C.INVENTORY[id].id);
			std::string type = item.type;
			if (type == "armor") {
				for (auto compItem : C.INVENTORY) {
					std::string compType = getItem(compItem.second.id).type;
					if (type == "armor" && compType == "armor") {
						C.INVENTORY[compItem.first].equipped = false;
					}
				}
			}
			else if (type == "weapon" || type == "staff") {
				std::string changes = str(C.ID);
				bool twoHanded = item.twoHanded;
				if (C.INVENTORY.count(C.LEFT) > 0) {
					UI_Item right = getItem(C.INVENTORY[C.LEFT].id);
					if (right.twoHanded) {
						twoHanded = true;
					}
				}
				if (C.INVENTORY.count(C.RIGHT) > 0) {
					UI_Item right = getItem(C.INVENTORY[C.RIGHT].id);
					if (right.twoHanded) {
						twoHanded = true;
					}
				}

				if (twoHanded) {
					if (C.INVENTORY.count(C.LEFT) > 0) {
						C.INVENTORY[C.LEFT].equipped = false;
						changes += addLine("DEQUIP", C.LEFT);
					}
					if (C.INVENTORY.count(C.RIGHT) > 0) {
						C.INVENTORY[C.RIGHT].equipped = false;
						changes += addLine("DEQUIP", C.RIGHT);
					}
					C.LEFT = id;
					C.RIGHT = id;
				}
				else {
					if (C.RIGHT == "") {
						C.RIGHT = id;
					}
					else {
						if (C.INVENTORY.count(C.LEFT) > 0) {
							C.INVENTORY[C.LEFT].equipped = false;
						}
						C.LEFT = id;
					}
				}
				C.INVENTORY[id].equipped = true;
				changes += addLine("LEFT", C.LEFT);
				changes += addLine("RIGHT", C.RIGHT);
				changes += addLine("EQUIP", id);
				sendData("STATS", changes);
				return "You equip the *GREEN*" + item.id;
			}
			else {
				return "*RED*You can't equip that.";
			}
		}
	}
	return "*RED*Item couldn't be found!";
}

std::string commandBuy(int playerIndex, Character& C, std::vector<std::string> words) {
	if (C.TRADING == "") {
		return "*RED*You must trade with a merchant first.";
	}
	std::string args = join(words);
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
	newCharacter.AP = 6;
	newCharacter.STAMINA = 20;
	newCharacter.HP = 30;
	newCharacter.ID = id;
	newCharacter.NAME = name;
	newCharacter.USER = players[playerIndex].USERNAME;

	Item dagger("dagger");
	dagger.equipped = true;
	newCharacter.LEFT = dagger.index;
	newCharacter.INVENTORY[dagger.index] = dagger;

	players[playerIndex].ID = id;
	CHARACTERS[id] = newCharacter;
	
	sendCharacter(newCharacter);
	return id;
}

std::string commandTake(int playerIndex, Character& C, std::vector<std::string> words) {
	std::string args = join(words);
	if (BATTLES.count(C.LOCATION) == 0) {
		return "*RED*You must be in combat to attack.";
	}

	Battle* battle = &BATTLES[C.LOCATION];
	std::string index = "";
	std::string id = "";
	if (battle->loot.count(args) > 0) {
		id = battle->loot[args].id;
		index = args;
	}
	else {
		for (auto item : battle->loot) {
			if (low(item.second.id) == low(args)) {
				id = args;
				index = item.first;
			}
		}
	}
	if (index == "") {
		return "*RED*Couldn't find item '" + args + "'";
	}
	if (!canTake(C, battle->loot[index].id)) {
		return "*RED*You don't have room in your inventory for that.";
	}

	C.INVENTORY[index] = battle->loot[index];
	removeLoot(*battle, index);

	sendItem(C.ID, C.INVENTORY[index]);

	return "";
}

void commandTrade(int playerIndex, Character& C, std::string id) {
	NPC npc = getNPC(id);
	if (low(npc.NAME) == low(id)) {
		C.TRADING = npc.NAME;
		sendStat(C.ID, "TRADING", npc.NAME, { playerIndex });
	}
}

std::string commandLevel(int playerIndex, Character& C, std::string stat) {
	if (C.SP == 0) {
		return "*RED*You don't have any SP";
	}
	std::vector<std::string> stats = {
		"VIT", "END", "DEX", "MAG", "WEP", "AVD"
	};
	int index = -1;
	for (int i = 0; i < stats.size(); i++) {
		if (low(stats[i]) == low(stat)) {
			index = i;
			break;
		}
	}
	if (C.STATS[index] == 99 || (index == AVD && C.STATS[AVD] >= 10)) {
		return "*RED*You can't increase this stat any further.\n";
	}
	std::string changes = str(C.ID);

	C.STATS[index]++;
	changes += addLine("STATS", C.STATS);
	C.SP--;
	changes += addLine("SP", C.SP);
	if (index == VIT) {
		C.HP += 10;
		changes += addLine("HP", C.HP);
	}
	if (index == END) {
		C.STAMINA += 10;
		changes += addLine("STAMINA", C.STAMINA);
	}
	if (index == DEX) {
		C.AP += 3;
		changes += addLine("AP", C.AP);
	}

	sendData("STATS", changes);
	return "*TEAL*You increased your *BLUE*" + stats[index] + "*TEAL* stat to *GREY*" + C.STATS[index] + "*TEAL*";
}

std::string commandSleep(int playerIndex, Character& C) {
	if (C.HP >= MaxHP(C)) {
		return "You're already full HP.";
	}
	std::string msg = "";
	if (low(C.LOCATION) == "tavern") {
		if (C.GOLD >= 5) {
			std::vector<std::string> dialogues = {
				"You gamble a little in common room, but come out even in the end.",
				"You dream about giant slugs.",
				"Penelope brings you a mug of water as she shows you to your room, and sets it at your bedside.",
				"You wake up to the sound of laughter from the floor below you, and you feel refreshed.",
				"A dog is barking somewhere in the village.",
				"You stumble on your way up the stairs.",
				"The bed is soft and warm.",
				"You dream of swimming in a cold river on a hot summer day.",
				"The wind shakes the shutters to your room.",
				"You hear rain tapping on the roof above you."
			};
			setStat(C, "GOLD", C.GOLD - 5);
			setStat(C, "HP", MaxHP(C));
			return "*GREEN*You pay five gold to sleep at the tavern. *GREY*\n" + dialogues[rand() % dialogues.size()] + "\n";
		}
		return "*RED*You can't afford to rent a room here!\n";
	}
	return "*RED*You have to be at the tavern to sleep.\n";
}

void command(std::string input, int playerIndex) {
	std::string id = players[playerIndex].ID;
	std::string msg = "";
	std::vector<std::string> words = split(low(input));
	std::string keyword = words[0];
	words.erase(words.begin());

	if (keyword == "character") {
		if (players[playerIndex].ID == "") {
			if (words.size() == 0) {
				msg = "*RED*Please enter a character name!";
			}
			else {
				id = createCharacter(playerIndex, words[0]);
				players[playerIndex].ID = id;
			}
		}
	}

	if (players[playerIndex].ID == "") {
		sendText("*RED*You need to make a character!", { playerIndex });
		return;
	}

	
	if (keyword == "go" || keyword == "enter" || keyword == "travel") {
		msg = commandTravel(playerIndex, CHARACTERS[id], words);
	}
	else if (keyword == "delve") {
		msg = commandDelve(playerIndex, CHARACTERS[id], words);
	}
	else if (keyword == "level") {
		msg = commandLevel(playerIndex, CHARACTERS[id], words[0]);
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
			handleCombat(CHARACTERS[id].LOCATION);
		}
		else {
			msg = "*RED*You must be in combat. Did you mean to *YELLOW*quit*RED* the game?";
		}
	}
	else if (keyword == "leave" || keyword == "back" || keyword == "stop") {
		msg = commandLeave(playerIndex, CHARACTERS[id], words);
	}
	else if (keyword == "move") {
		if (BATTLES.count(CHARACTERS[id].LOCATION) > 0) {
			int x = readInt(words[0]);
			int y = readInt(words[0]);
			if (x >= 0 && y >= 0 && x < 12 && y < 12) {
				std::vector<std::vector<int>> movementCosts = moveCosts(CHARACTERS[id], BATTLES[CHARACTERS[id].LOCATION]);
				if (movementCosts[y][x] <= CHARACTERS[id].AP) {
					if (BATTLES[CHARACTERS[id].LOCATION].round != 0) {
						setStat(CHARACTERS[id], "AP", CHARACTERS[id].AP - movementCosts[y][x]);
					}
					CHARACTERS[id].X = x;
					CHARACTERS[id].Y = y;
					std::string changes = str(id);
					changes += addLine("X", x);
					changes += addLine("Y", y);
					sendData("STATS", changes);
				}
			}
		}
	}
	else if (keyword == "hp") {
		if (words.size() > 0) {
			int num = std::stoi(words[0]);
			CHARACTERS[id].HP = num;
			sendStat(id, "HP", CHARACTERS[id].HP);
		}
	}
	else if (keyword == "take") {
		msg = commandTake(playerIndex, CHARACTERS[id], words);
	}
	else if (keyword == "rest" || keyword == "sleep") {
		msg = commandSleep(playerIndex, CHARACTERS[id]);
	}
	else if (keyword == "gold") {
		CHARACTERS[id].GOLD += 25;
		sendStat(id, "GOLD", CHARACTERS[id].GOLD);
	}
	else if (keyword == "suicide") {
		msg = "*RED*You've lost the will to go on. . .";
		std::string loc = CHARACTERS[id].LOCATION;
		removeCharacter(CHARACTERS[id]);
		if (BATTLES.count(loc) > 0) {
			validateBattle(loc);
		}
	}
	std::cout << msg << std::endl;
	if (CHARACTERS.count(id) > 0) {
		save(CHARACTERS[id]);
	}
	sendText(msg, { playerIndex });
}
