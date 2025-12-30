#pragma once

struct Player {
	std::string ID = "";
	std::string USERNAME = "";
	std::string PASSWORD = "";

	bool connected = false;

	SOCKET socket;
	
	std::vector<Message> messages = {};

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

struct DamageResult {
	std::string changes = "";
	int damage = 0;
	DamageResult(std::string c = "", int dmg = 0) {
		changes = c;
		damage = dmg;
	}
};

struct TargetResult {
	std::string closest = "";
	std::string farthest = "";
	std::string strongest = "";
	std::string weakest = "";
	std::vector<std::string> targets = {};

	TargetResult() {}
};