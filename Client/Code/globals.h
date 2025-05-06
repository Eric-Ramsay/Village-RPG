#pragma once

std::deque<Message> messageBuffer = {};
bool SENDING = false;
Message* bufferStart = nullptr;
Message* bufferEnd = nullptr;
Message* firstProcess = nullptr;

bool connected = true;
std::string IP = "10.0.0.12";
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