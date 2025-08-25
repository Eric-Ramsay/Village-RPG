#pragma once

struct Player {
	std::string ID;
	std::string USERNAME = "";
	std::string PASSWORD = "";

	SOCKET socket;
	
	std::vector<Message> messages = {};

	bool connected = true;

	Player(SOCKET s) {
		socket = s;
	}
};

struct Account {
	std::string USERNAME;
	std::string PASSWORD;

	Account() {}

	Account(std::string user, std::string pass) {
		USERNAME = user;
		PASSWORD = pass;
	}
};