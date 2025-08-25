#pragma once
bool fileExists(std::string name) {
	std::ifstream f(name.c_str());
	return f.good();
}

void parseChange(Account& account, std::string type, std::string data) {
	if (type == "USERNAME") {
		account.USERNAME = readStr(data);
	}
	if (type == "PASSWORD") {
		account.PASSWORD = readStr(data);
	}
}

void validateBattle(std::string id) {
	for (int i = 0; i < BATTLES[id].teams[0].size(); i++) {
		std::string character = BATTLES[id].teams[0][i];
		if (CHARACTERS.count(character) == 0 || CHARACTERS[character].LOCATION != BATTLES[id].id) {
			BATTLES[id].teams[0].erase(BATTLES[id].teams[0].begin() + i);
		}
	}
	if (BATTLES[id].teams[0].size() + BATTLES[id].teams[1].size() == 0) {
		std::string filePath = "./Saves/Battles/" + BATTLES[id].id + ".txt";
		std::remove(filePath.c_str());
		BATTLES.erase(id);
	}
}