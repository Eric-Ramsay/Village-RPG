#pragma once
std::string commandTravel(int playerIndex, Character& C, std::vector<std::string> words, int index) {
	if (words.size() == 0 || words[0].size() == 0) {
		return "*RED*Command not understood. . . ";
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
	for (int i = 0; i < battles.size(); i++) {
		if (C.LOCATION == battles[i].id) {
			return "*RED*You're already in a battle!";
		}
		if (C.LOCATION == battles[i].zone && battles[i].round == 0) {
			battles[i].teams[0].push_back(C.ID);
			sendBattle(battles[i], { playerIndex })
			return "*YELLOW*You delve deeper. . .";
		}
	}
	return "";
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
	}
	std::cout << msg << std::endl;
	savePlayer(CHARACTERS[id]);
	sendData("TEXT", msg, { playerIndex });
}
