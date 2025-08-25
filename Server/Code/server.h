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

void sendCharacter(int playerIndex) {
	std::string id = players[playerIndex].ID;
	std::string data = serialize(CHARACTERS[id]);
	sendData("CHARACTER", data);
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