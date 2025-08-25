#pragma once
int SCREEN_X = 1;
int SCREEN_Y = 1;

std::deque<Message> messageBuffer = {};
bool SENDING = false;
Message* bufferStart = nullptr;
Message* bufferEnd = nullptr;
Message* firstProcess = nullptr;

bool connected = true;
std::string IP = "67.177.184.43";
sockaddr_in hint;
int port = 1234;
SOCKET sock;

std::vector<Location> LOCATIONS = {};
std::vector<Character> ENEMIES = {};

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

std::string USERNAME = "";
std::string PASSWORD = "";

