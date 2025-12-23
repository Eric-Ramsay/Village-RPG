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
		if (C.ID == "" || C.NAME == "") {
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
		GRAVES.push_back(C);
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
		parseChange(CHARACTERS[id], stat, data);
		if (stat == "HP") {
			Animation anim;
			anim.type = "text";	
			anim.position.x = CHARACTERS[id].X * 16 + 8;
			anim.position.y = CHARACTERS[id].Y * 16 + 4;
			anim.endPos.x = CHARACTERS[id].X * 16 + 8;
			anim.endPos.y = CHARACTERS[id].Y * 16 - 8;
			anim.endOpacity = 0;
			anim.beginFade = 20;
			anim.duration = 40;
			if (CHARACTERS[id].HP > HP) {
				anim.text = "+" + to_str(CHARACTERS[id].HP - HP);
				anim.color = "*GREEN*";
				animations.push_back(anim);
			}
			else if (CHARACTERS[id].HP < HP) {
				anim.text = to_str(CHARACTERS[id].HP - HP);
				anim.color = "*RED*";
				animations.push_back(anim);
			}
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
		if (CHARACTERS.count(ID) > 0 && CHARACTERS[ID].LOCATION == battle.id) {
			combatMenu.tabs[1] = "LOOT (" + to_str(BATTLES[battle.id].loot.size()) + ")";
			measureTab(&combatMenu);
		}
	}
	if (type == "BATTLE_CHANGE") {
		std::string id = readStr(data);
		std::string stat = readStr(data);
		parseChange(BATTLES[id], stat, data);
	}
	if (type == "REMOVE_BATTLE") {
		BATTLES.erase(data);
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