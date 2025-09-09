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
					if (!players[i].connected) {
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
								players[i].connected = true;
								for (auto character : CHARACTERS) {
									if (character.second.USER == players[i].USERNAME) {
										players[i].ID = character.first;
										std::string location = character.second.LOCATION;
										if (BATTLES.count(location) > 0) {
											std::string data = serialize(BATTLES[location]);
											sendData("BATTLE", data, { i });
										}
									}
									sendCharacter(character.second, { i });
								}
							}
							else {
								sendData("REJECT_PASSWORD", "", { i });
							}
						}
					}
					else {
						if (type == "TEXT") {
							sendData(type, data, { i });
						}
						else if (type == "COMMAND") {
							command(data, i);
						}
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