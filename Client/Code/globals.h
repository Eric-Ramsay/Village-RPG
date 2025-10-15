#pragma once

std::deque<Message> messageBuffer = {};
bool SENDING = false;
Message* bufferStart = nullptr;
Message* bufferEnd = nullptr;
Message* firstProcess = nullptr;

bool connected = true;
std::string IP = "10.0.0.11";
sockaddr_in hint;
int port = 1234;
SOCKET sock;

std::unordered_map<std::string, UI_Item> ITEMS = {};
std::unordered_map<std::string, NPC> PEOPLE = {};
std::unordered_map<std::string, Location> LOCATIONS = {};
std::unordered_map<std::string, Character> ENEMIES = {};

UIState UI;

std::vector<sf::Vertex> vertices;
int numVertices = 0;
int vertSize = 200000;

Battle BATTLE;

int WIDTH = 16 * 40;
int HEIGHT = 9 * 40;

std::string input = "";
int holdingBackspace = 0;

std::vector<std::string> logs = {"*BLACK*Connecting . . ."};

std::string ID = "";
std::unordered_map<std::string, Character> CHARACTERS = {};

std::string USERNAME = "eric";
std::string PASSWORD = "123";


bool blur = false;
bool scanLines = false;
bool quit = false;