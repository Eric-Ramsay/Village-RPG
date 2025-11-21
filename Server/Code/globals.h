#pragma once
std::string IP = "10.0.0.11";
int port = 1234;
std::deque<Message> outMessages = {};

Message* bufferStart = nullptr;
Message* bufferEnd = nullptr;
Message* processing = nullptr;
bool SENDING = false;

std::vector<Player> players = {};


SOCKET listening;
fd_set master;


std::unordered_map<std::string, Account> ACCOUNTS = {};
std::unordered_map<std::string, Character> CHARACTERS = {};
std::unordered_map<std::string, Battle> BATTLES = {};

std::unordered_map<std::string, UI_Item> ITEMS = {};
std::unordered_map<std::string, NPC> PEOPLE = {};
std::unordered_map<std::string, Location> LOCATIONS = {};
std::unordered_map<std::string, Character> ENEMIES = {};

std::vector<std::vector<Drop>> zoneLoot = { {}, {}, {}, {} };
std::vector<UI_Item> genericLoot = {};

std::vector<std::string> zoneNames = {
	"Haunted Crypts",
	"Acrid Swamp",
	"Wilted Woods"
};