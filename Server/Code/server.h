#pragma once

std::vector<int> EVERYONE = {};

struct Message {
	std::vector<int> players = {};
	std::string type = "";
	std::string data = "";
	Message* next = nullptr;
	bool done = false;

	Message(std::string t = "", std::string d = "", std::vector<int> p = EVERYONE) {
		type = t;
		data = d;
		players = p;
	}

	template<typename T>
	Message& operator &=(const T& rhs)
	{
		this.data += to_str(rhs);
		return *this;
	}
};

struct Player {
	SOCKET socket;
	std::string IP;

	Character character;
	std::vector<Message> messages = {};

	bool connected = true;

	Player(SOCKET s, std::string ip) {
		socket = s;
		IP = ip;
	}
};


std::string IP = "10.0.0.12";
int port = 1234;
std::deque<Message> outMessages = {};

Message* bufferStart = nullptr;
Message* bufferEnd = nullptr;
Message* processing = nullptr;
bool SENDING = false;

std::vector<Player> players = {};

void sendData(Message msg) {
	SENDING = TRUE;
	Message* m = new Message(msg.type, msg.data, msg.players);
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

void RemoveFromEveryone(int value) {
	std::vector<int> newList = {};
	for (int p : EVERYONE) {
		if (p != value) {
			newList.push_back(p);
		}
	}
	EVERYONE = newList;
}