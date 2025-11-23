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

void sendToIds(std::string type, std::string data, std::vector<std::string> sendList) {
	std::vector<int> list = {};
	for (std::string id : sendList) {
		for (int i = 0; i < players.size(); i++) {
			if (players[i].ID == id) {
				list.push_back(i);
				break;
			}
		}
	}
	sendData(type, data, list);
}

void sendCharacter(Character character, std::vector<int> sendList = {}) {
	std::string data = serialize(character);
	if (character.ID == "") {
		std::cout << "Weird Charcter Detected" << std::endl;
	}
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

void sendItem(std::string id, Item item, std::vector<int> sendList = {}) {
	if (item.id == "") {
		std::cout << "Weird Item detected" << std::endl;
	}
	sendData("STAT", id + "!!!ITEM!!!" + serialize(item), sendList);
}

void sendText(std::string text, std::vector<int> sendList = {}) {
	if (text != "") {
		sendData("TEXT", text, sendList);
	}
}

/*void sendInventory(std::string id, std::unordered_map<Item> inventory, std::vector<int> sendList = {}) {
	sendData("STAT", id + "!!!INVENTORY!!!" + serialize(inventory), sendList);
}*/

void sendStat(std::string id, std::string stat, std::vector<Item> value, std::vector<int> sendList = {}) {
	sendData("STAT", id + "!!!" + stat + "!!!" + serialize(value), sendList);
}

void sendBattle(Battle battle, std::vector<int> sendList = {}) {
	std::string data = serialize(battle);
	sendData("BATTLE", data, sendList);
}

std::vector<int> battleIndices(Battle battle) {
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
	return indices;
}

void updateBattle(Battle battle) {
	save(battle);
	sendBattle(battle, battleIndices(battle));
}

void removeLoot(Battle& battle, std::string index) {
	if (battle.loot.count(index) > 0) {
		battle.loot.erase(index);
		save(battle);
		sendData("BATTLE", "REMOVE_ITEM!!!" + index, battleIndices(battle));
	}
}

void removeCharacter(Character character) {
	sendData("REMOVE_CHARACTER", character.ID);
	if (character.TYPE == "player") {
		graveSave(character);
		std::remove(("./Saves/Characters/" + character.ID + ".txt").c_str());
	}
	else {
		std::remove(("./Saves/Characters/Enemies/" + character.ID + ".txt").c_str());
	}
	for (int i = 0; i < players.size(); i++) {
		if (players[i].ID == character.ID) {
			players[i].ID = "";
		}
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