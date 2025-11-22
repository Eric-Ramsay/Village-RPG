#pragma once

void processMessages() {
	for (;;) {
		Sleep(1);
		while (firstProcess != nullptr) {
			static int numMessages = 0;
			std::string data = firstProcess->data;
			std::string type = firstProcess->type;
			std::cout << type << " " << std::endl;
			if (type == "TEXT") {
				std::vector < std::string> lines = splitLines(data);
				for (std::string line : lines) {
					logs.push_back(line);
				}
			}
			if (type == "READY") {
				logs.push_back("*TEAL*Connected!");
				if (USERNAME != "" && PASSWORD != "") {
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
			if (type == "REMOVE_CHARACTER") {
				CHARACTERS.erase(data);
				if (data == ID) {
					ID = "";
				}
			}
			if (type == "STAT") {
				std::string id = readStr(data);
				std::string stat = readStr(data);
				parseChange(CHARACTERS[id], stat, data);
			}
			if (type == "BATTLE") {
				std::vector<std::string> strings = split(data, '\n');
				for (std::string str : strings) {
					std::string type = readStr(str);
					parseChange(BATTLE, type, str);
				}
				combatMenu.tabs[1] = "LOOT (" + to_str(BATTLE.loot.size()) + ")";
				setTabString(combatMenu);
			}
			Message* temp = firstProcess;
			firstProcess = firstProcess->next;
			delete temp;
		}
		if (bufferStart != nullptr && !SENDING) {
			firstProcess = bufferStart;
			bufferStart = nullptr;
			bufferEnd = nullptr;
		}
	}
}