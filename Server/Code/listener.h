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
					if (type == "REGISTER_USERNAME") {
						// Bug: If two users create accounts at the same-ish time w/ the same username, it doesn't check for that.
						if (ACCOUNTS.count(data) == 0) {
							players[i].USERNAME = data;
							sendData("ACCEPT_USERNAME", "", { i });
						}
						else {
							sendData("REJECT_USERNAME", "", { i });
						}
					}
					else if (type == "REGISTER_PASSWORD") {
						ACCOUNTS[players[i].USERNAME] = Account(players[i].USERNAME, data);
						save(ACCOUNTS[players[i].USERNAME]);
						sendData("ACCEPT_PASSWORD", "", { i });
					}
					else if (type == "LOGIN_USERNAME") {
						if (ACCOUNTS.count(data) > 0) {
							players[i].USERNAME = data;
							sendData("ACCEPT_USERNAME", "", { i });
						}
						else {
							sendData("REJECT_USERNAME", "", { i });
						}
					}
					else if (type == "LOGIN_PASSWORD") {
						if (ACCOUNTS[players[i].USERNAME].PASSWORD == data) {
							sendData("ACCEPT_PASSWORD", "", { i });
							std::string id = players[i].USERNAME;
							if (CHARACTERS.count(id) != 0) {
								players[i].ID = id;
							}
							for (auto character : CHARACTERS) {
								if (character.second.TYPE == "player" || (id != "" && character.second.LOCATION == CHARACTERS[id].LOCATION)) {
									std::string data = serialize(character.second);
									sendData("CHARACTER", data, { i });
								}
							}
							for (auto battle : BATTLES) {
								if (battle.second.id == CHARACTERS[id].LOCATION) {
									std::string data = serialize(battle.second);
									sendData("BATTLE", data, { i });
								}
							}
						}
						else {
							sendData("REJECT_PASSWORD", "", { i });
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