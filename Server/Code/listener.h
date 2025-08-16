#pragma once
// Interpret Completed Messages from Clients
void ProcessMessages() {
	Sleep(1);
	for (int i = 0; i < players.size(); i++) {
		std::vector<Message> newMessages = {};
		for (int j = 0; j < players[i].messages.size(); j++) {
			if (players[i].messages[j].done) {
				if (players[i].messages[j].data.size() > 0) {
					std::string data = players[i].messages[j].data;
					std::string type = players[i].messages[j].type;
					std::cout << type << " " << data << std::endl;
					if (type == "LOG_IN") {
						std::string id = "";
						if (CHARACTERS.count(data) != 0) {
							players[i].ID = data;
							id = data;
						}
						for (auto character : CHARACTERS) {
							if (character.second.TYPE == "player" || (id != "" && character.second.LOCATION == CHARACTERS[id].LOCATION)) {
								std::string data = serialize(character.second);
								sendData("CHARACTER", data);
							}
						}
						for (auto battle: BATTLES) {
							if (battle.second.id == CHARACTERS[id].LOCATION) {
								std::string data = serialize(battle.second);
								sendData("BATTLE", data);
							}
						}
					}
					else if (players[i].ID != "") {
						if (type == "TEXT") {
							sendData(type, data, { i });
						}
						else if (type == "COMMAND") {
							command(data, i);
						}
					}
					else {
						sendData("TEXT", "*RED*You need to create a character first!");
					}
				}
			}
			else {
				newMessages.push_back(players[i].messages[j]);
			}
		}
		players[i].messages = newMessages;
	}
}