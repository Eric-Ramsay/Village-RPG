#pragma once

void process(std::string type, std::string data) {
	if (type == "TEXT") {
		std::vector < std::string> lines = splitLines(data, 340);
		for (std::string line : lines) {
			logs.push_back(line);
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
		for (int i = 0; i < 2; i++) {
			std::vector<std::string> team = {};
			for (std::string id : BATTLE.teams[i]) {
				if (id != data) {
					team.push_back(id);
				}
			}
			BATTLE.teams[i] = team;
		}


	}
	if (type == "STAT") {
		std::string id = readStr(data);
		std::string stat = readStr(data);
		parseChange(CHARACTERS[id], stat, data);
	}
	if (type == "STATS") {
		std::string id = readStr(data);
		std::vector<std::string> strings = split(data, '\n');
		for (std::string str : strings) {
			std::string type = readStr(str);
			parseChange(CHARACTERS[id], type, str);
		}
	}
	if (type == "BUNDLE") {
		std::vector<std::string> strings = split(data, '\t');
		for (std::string str : strings) {
			std::string type = readStr(str);
			process(type, str);
		}
	}
	if (type == "BATTLE") {
		std::vector<std::string> strings = split(data, '\n');
		for (std::string str : strings) {
			std::string type = readStr(str);
			parseChange(BATTLE, type, str);
		}
		combatMenu.tabs[1] = "LOOT (" + to_str(BATTLE.loot.size()) + ")";
		measureTab(&combatMenu);
	}
	if (!logBar.dragging) {
		updateOrbPos(&logBar, logBar.index);
	}
}

void processMessages() {
	for (;;) {
		Sleep(1);
		while (firstProcess != nullptr) {
			static int numMessages = 0;
			std::string data = firstProcess->data;
			std::string type = firstProcess->type;
			std::cout << type << " " << std::endl;

			process(type, data);

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
}