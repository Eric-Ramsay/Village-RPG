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

std::vector<Location> LOCATIONS = {};
std::vector<Character> ENEMIES = {};


std::unordered_map<std::string, Character> CHARACTERS = {};
std::unordered_map<std::string, Battle> BATTLES = {};