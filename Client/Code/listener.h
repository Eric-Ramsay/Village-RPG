#pragma once

void process(std::string type, std::string data) {
	std::cout << type << std::endl;
	if (type == "TEXT") {
		std::string location = readStr(data);
		if (location == "" || (CHARACTERS.count(ID) > 0 && CHARACTERS[ID].LOCATION == location)) {
			std::vector < std::string> lines = splitLines(data, 340);
			for (std::string line : lines) {
				logs.push_back(line);
			}
		}
	}
	if (type == "READY") {
		CHARACTERS = {};
		GRAVES = {};
		GRAVEYARD = {};
		logs.push_back("*TEAL*Connected!");
		if (rememberAccount && USERNAME != "" && PASSWORD != "") {
			sendData("LOGIN_USERNAME", USERNAME);
			sendData("LOGIN_PASSWORD", PASSWORD);
		}
		else {
			UI.signInState = CHOOSE;
		}
		stateMessage(UI.signInState);
	}
	if (type == "REJECT_USERNAME") {
		if (UI.signInState == LOGIN_USERNAME) {
			logs.push_back("*RED*No matching username was found by the server. Try again.");
		}
		else if (UI.signInState == REGISTER_USERNAME) {
			logs.push_back("*RED*This username has already been taken. Try again.");
		}
		else {
			UI.signInState = CHOOSE;
		}
	}
	if (type == "ACCEPT_USERNAME") {
		if (UI.signInState == LOGIN_USERNAME) {
			UI.signInState = LOGIN_PASSWORD;
		}
		else {
			UI.signInState = REGISTER_PASSWORD;
		}
		stateMessage(UI.signInState);
	}
	if (type == "ACCEPT_PASSWORD") {
		logs.push_back("*GREEN*You've successfully signed in.");
		UI.signInState = COMPLETED;
	}
	if (type == "REJECT_PASSWORD") {
		logs.push_back("*RED*Password was incorrect. Try again.");
		if (UI.signInState == COMPLETED) {
			UI.signInState = CHOOSE;
			stateMessage(UI.signInState);
		}
	}
	if (type == "CHARACTER") {
		Character C;
		std::vector<std::string> strings = split(data, '\n');
		for (std::string str : strings) {
			std::string stat = readStr(str);
			parseChange(C, stat, str);
		}
		if (C.USER == USERNAME) {
			ID = C.ID;
		}
		if (C.ID == "") {
			std::cout << "Weird Character Received" << std::endl;
		}
		CHARACTERS[C.ID] = C;
	}
	if (type == "GRAVE") {
		Character C;
		std::vector<std::string> strings = split(data, '\n');
		for (std::string str : strings) {
			std::string stat = readStr(str);
			parseChange(C, stat, str);
		}
		if (C.ID == "" || C.NAME == "") {
			std::cout << "Weird Grave Received" << std::endl;
		}
		GRAVEYARD[C.ID] = C;
		GRAVES.push_back(&C);
		GRAVES = sortChars(GRAVES);
	}
	if (type == "REMOVE_CHARACTER") {
		CHARACTERS.erase(data);
		if (data == ID) {
			ID = "";
		}
		for (auto battle : BATTLES) {
			std::vector<std::string> team = {};
			for (std::string id : battle.second.characters) {
				if (id != data) {
					team.push_back(id);
				}
			}
			BATTLES[battle.first].characters = team;
		}
	}
	if (type == "STAT") {
		std::string id = readStr(data);
		std::string stat = readStr(data);
		int HP = CHARACTERS[id].HP;
		if (stat == "COORDINATES") {
			ANIMATIONS[id].move.startX = ANIMATIONS[id].xPos;
			ANIMATIONS[id].move.startY = ANIMATIONS[id].yPos;
			ANIMATIONS[id].move.duration = 30;
			ANIMATIONS[id].move.timePassed = 0;
		}
		parseChange(CHARACTERS[id], stat, data);
		if (stat == "HP") {
			std::string text = "*RED*" + to_str(CHARACTERS[id].HP - HP);
			if (CHARACTERS[id].HP > HP) {
				text = "*GREEN*+" + to_str(CHARACTERS[id].HP - HP);
			}
			ANIMATIONS[id].text.push_back(TextAnimation(text, 40));
		}
	}
	if (type == "STATS") {
		std::string id = readStr(data);
		std::vector<std::string> strings = split(data, '\n');
		for (std::string str : strings) {
			std::string type = readStr(str);
			parseChange(CHARACTERS[id], type, str);
		}
	}
	if (type == "BATTLE") {
		Battle battle;
		std::vector<std::string> strings = split(data, '\n');
		for (std::string str : strings) {
			std::string type = readStr(str);
			parseChange(battle, type, str);
		}
		BATTLES[battle.id] = battle;
	}
	if (type == "BATTLE_STAT") {
		std::string id = readStr(data);
		std::string stat = readStr(data);
		parseChange(BATTLES[id], stat, data);
	}
	if (type == "REMOVE_BATTLE") {
		BATTLES.erase(data);
	}
	if (type == "ATTACK") {
		std::string attackerId = readStr(data);
		std::string targetId = readStr(data);
		if (CHARACTERS.count(attackerId) > 0 && CHARACTERS.count(targetId) > 0) {
			int x = battleX + CHARACTERS[targetId].X * 16 + 8;
			int y = 20 + battleY + CHARACTERS[targetId].Y * 16 + 8;
			ANIMATIONS[attackerId].lines.push_back(LineAnimation(x, y, "RED"));
		}
	}
	if (!logBar.dragging) {
		updateOrbPos(&logBar, logBar.index);
	}
}

void processMessages() {
	while (firstProcess != nullptr) {
		static int numMessages = 0;
		std::string data = firstProcess->data;
		std::cout << std::endl;

		std::vector<std::string> strings = split(data, 249);
		for (std::string str : strings) {
			std::string type = readStr(str);
			process(type, str);
		}

		Message* temp = firstProcess;
		firstProcess = firstProcess->next;
		delete temp;
		updateMovement = true;
	}
	if (bufferStart != nullptr && !SENDING) {
		firstProcess = bufferStart;
		bufferStart = nullptr;
		bufferEnd = nullptr;
	}
}