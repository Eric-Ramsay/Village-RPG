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

UIState UI;

std::vector<sf::Vertex> vertices;
int numVertices = 0;
int vertSize = 200000;

float min(float a, float b) {
	if (a < b) {
		return a;
	}
	return b;
}

float max(float a, float b) {
	if (a < b) {
		return b;
	}
	return a;
}

Character C;

std::vector<Location> LOCATIONS = {};
std::vector<Location> BUILDINGS = {};

int WIDTH = 16 * 53;
int HEIGHT = 9 * 53;

std::string input = "";
int holdingBackspace = 0;

std::vector<std::string> logs = {};