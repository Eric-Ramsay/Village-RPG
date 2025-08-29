#pragma once
std::string commandTravel(int playerIndex, Character& C, std::vector<std::string> words, int index) {
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

std::string commandLeave(int playerIndex, Character& C, std::vector<std::string> words) {
	std::string id = C.LOCATION;
	if (BATTLES.count(id) > 0) {
		if (BATTLES[id].round == 0) {
			C.LOCATION = BATTLES[id].zone;
			validateBattle(id);
			return "*YELLOW*You leave the battle . . .";
		}
	}
	return "*RED*You can't leave from here . . .";
}

void command(std::string input, int playerIndex) {
	std::string id = players[playerIndex].ID;
	std::string msg = "";
	std::vector<std::string> words = split(low(input));
	std::string keyword = words[0];
	words.erase(words.begin());
	if (keyword == "go" || keyword == "enter" || keyword == "travel") {
		msg = commandTravel(playerIndex, CHARACTERS[id], words, -1);
		sendStat(id, "LOCATION", CHARACTERS[id].LOCATION);
	}
	if (keyword == "delve") {
		msg = commandDelve(playerIndex, CHARACTERS[id], words);
		sendStat(id, "LOCATION", CHARACTERS[id].LOCATION);
	}
	if (keyword == "leave") {
		msg = commandLeave(playerIndex, CHARACTERS[id], words);
		sendStat(id, "LOCATION", CHARACTERS[id].LOCATION);
	}
	if (keyword == "move") {
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
	if (keyword == "hp") {
		if (words.size() > 0) {
			int num = std::stoi(words[0]);
			CHARACTERS[id].HP = num;
			sendStat(id, "HP", CHARACTERS[id].HP);
		}
	}
	std::cout << msg << std::endl;
	save(CHARACTERS[id]);
	if (msg != "") {
		sendData("TEXT", msg, { playerIndex });
	}
}
