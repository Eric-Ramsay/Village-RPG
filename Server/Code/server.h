#pragma once
void sendData(std::string type, std::string data, std::vector<int> sendList = {}) {
	SENDING = TRUE;
	Message* m = new Message(type, data, sendList);
	if (m->players.size() == 0) {
		for (int i = 0; i < players.size(); i++) {
			m->players.push_back(i);
		}
	}
	if (bufferStart == nullptr) {
		bufferStart = m;
	}
	else if (bufferStart->next == nullptr) {
		bufferStart->next = m;
		bufferEnd = m;
	}
	else {
		bufferEnd->next = m;
		bufferEnd = m;
	}
	SENDING = FALSE;
}

void sendCharacter(Character character, std::vector<int> sendList = {}) {
	std::string data = serialize(character);
	sendData("CHARACTER", data, sendList);
}

void sendStat(std::string id, std::string stat, std::string value, std::vector<int> sendList = {}) {
	sendData("STAT", id + "!!!" + stat + "!!!" + str(value), sendList);
}

void sendStat(std::string id, std::string stat, int value, std::vector<int> sendList = {}) {
	sendData("STAT", id + "!!!" + stat + "!!!" + str(value), sendList);
}

void sendStat(std::string id, std::string stat, std::vector<int> value, std::vector<int> sendList = {}) {
	sendData("STAT", id + "!!!" + stat + "!!!" + str(value), sendList);
}


void sendBattle(Battle battle, std::vector<int> sendList = {}) {
	std::string data = serialize(battle);
	sendData("BATTLE", data, sendList);
}

void updateBattle(Battle battle) {
	std::vector<int> indices = {};
	for (int i = 0; i < 2; i++) {
		for (std::string id : battle.teams[i]) {
			for (int i = 0; i < players.size(); i++) {
				if (players[i].ID == id) {
					indices.push_back(i);
				}
			}
		}
	}
	save(battle);
	sendBattle(battle, indices);
}

void removeCharacter(Character character) {
	sendData("REMOVE_CHARACTER", character.ID);
	if (character.TYPE == "player") {
		graveSave(character);
		std::remove(("./Saves/Characters/" + character.ID).c_str());
	}
	CHARACTERS.erase(character.ID);
}

void setStat(Character& C, std::string stat, std::string value) {
	parseChange(C, stat, str(value));
	sendData("STAT", C.ID + "!!!" + stat + "!!!" + str(value));
}

void setStat(Character& C, std::string stat, int value) {
	parseChange(C, stat, str(value));
	sendData("STAT", C.ID + "!!!" + stat + "!!!" + str(value));
}

void setStat(Character& C, std::string stat, std::vector<int> value) {
	parseChange(C, stat, str(value));
	sendData("STAT", C.ID + "!!!" + stat + "!!!" + str(value));
}