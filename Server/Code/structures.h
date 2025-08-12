#pragma once

struct Player {
	SOCKET socket;
	std::string ID;
	
	std::vector<Message> messages = {};

	bool connected = true;

	Player(SOCKET s) {
		socket = s;
	}
};