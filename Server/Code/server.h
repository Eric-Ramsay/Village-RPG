#pragma once
std::string IP = "10.0.0.12";
int port = 1234;
std::deque<Message> outMessages = {};

Message* bufferStart = nullptr;
Message* bufferEnd = nullptr;
Message* processing = nullptr;
bool SENDING = false;

std::vector<Player> players = {};

void SendData(std::string type, std::string data, std::vector<int> sendList = {}) {
	SENDING = TRUE;
	Message* m = new Message(type, data, sendList);
	if (m->players.size() == 0) {
		for (int i = 0; i < players.size(); i++) {
			m->players.push_back(i);
		}
	}
	if (bufferStart == nullptr) {
		bufferStart = m;
	}
	else if (bufferStart->next == nullptr) {
		bufferStart->next = m;
		bufferEnd = m;
	}
	else {
		bufferEnd->next = m;
		bufferEnd = m;
	}
	SENDING = FALSE;
}

std::string str(int num) {
	return std::to_string(num) + "!";
}