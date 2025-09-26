#pragma once
std::string IP = "10.0.0.143";
int port = 1234;
std::deque<Message> outMessages = {};

Message* bufferStart = nullptr;
Message* bufferEnd = nullptr;
Message* processing = nullptr;
bool SENDING = false;

std::vector<Player> players = {};


SOCKET listening;
fd_set master;

std::vector<Location> LOCATIONS = {};
std::vector<NPC> PEOPLE = {};
std::vector<UI_Item> ITEMS = {};
std::vector<Character> ENEMIES = {};


std::unordered_map<std::string, Account> ACCOUNTS = {};
std::unordered_map<std::string, Character> CHARACTERS = {};
std::unordered_map<std::string, Battle> BATTLES = {};