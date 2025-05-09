#pragma once

void SendData(std::string type, std::string data) {
	std::string str = type + "!" + data + "!STOP!";
	const int len = str.length();
	char* arr = new char[len + 1];
	for (int i = 0; i < len; i++) {
		arr[i] = str[i];
	}
	send(sock, arr, len, 0);
}


int nextMessage(std::deque<Message>& msgs) {
	int msgIndex = 0;
	while (msgIndex < msgs.size() && msgs[msgIndex].done) {
		msgIndex++;
	}
	if (msgIndex >= msgs.size()) {
		Message m;
		msgs.push_back(m);
	}
	return msgIndex;
}

void Input() {
	for (;;) {
		Sleep(1);
		std::string text;
		std::getline(std::cin, text);
		SendData("TEXT", text);
	}
}

void parseItem(Item* item, std::string data) {
	while (data.length() > 0) {
		std::string type = readStr(data);
		if (type == "TYPE") {
			item->type = readStr(data);
		}
		if (type == "NAME") {
			item->name = readStr(data);
		}
		if (type == "DESCRIPTION") {
			item->description = readStr(data);
		}
		if (type == "VALUE") {
			item->value = readInt(data);
		}
		if (type == "EQUIPPED") {
			item->equipped = (bool)readInt(data);
		}
		if (type == "RARE") {
			item->rare = (bool)readInt(data);
		}
		if (item->type == "weapon") {
			if (type == "SUBCLASS") {
				item->subclass = readStr(data);
			}
			if (type == "ATTACKS") {
				item->attacks = readInt(data);
			}
			if (type == "MAX_ATTACKS") {
				item->maxAttacks = readInt(data);
			}
			if (type == "CHANCE") {
				item->chance = readInt(data);
			}
			if (type == "MIN") {
				item->min = readInt(data);
			}
			if (type == "MAX") {
				item->max = readInt(data);
			}
			if (type == "AP") {
				item->AP = readInt(data);
			}
			if (type == "RANGE") {
				item->range = readInt(data);
			}
			if (type == "HANDS") {
				item->hands = readInt(data);
			}
		}
		else if (item->type == "armor") {
			if (type == "AP") {
				item->AP = readInt(data);
			}
			if (type == "STAMINA") {
				item->stamina = readInt(data);
			}
			if (type == "PHYSICAL_DEFENSE") {
				item->physical = readInt(data);
			}
			if (type == "MAGICAL_DEFENSE") {
				item->magical = readInt(data);
			}
		}
	}
}


void CharacterChange(Character& character, std::string type, std::string data) {
	if (type == "NAME") {
		character.NAME = readStr(data);
	}
	if (type == "ID") {
		character.ID = readStr(data);
	}
	if (type == "DESCRIPTION") {
		character.DESCRIPTION = readStr(data);
	}
	if (type == "COLOR") {
		character.COLOR = readStr(data);
	}
	if (type == "LOCATION") {
		character.LOCATION = readStr(data);
	}
	if (type == "BUILDING") {
		character.BUILDING = readStr(data);
	}
	if (type == "LEVEL") {
		character.LEVEL = readInt(data);
	}
	if (type == "XP") {
		character.XP = readInt(data);
	}
	if (type == "SP") {
		character.SP = readInt(data);
	}
	if (type == "GOLD") {
		character.GOLD = readInt(data);
	}
	if (type == "HP") {
		character.HP = readInt(data);
	}
	if (type == "BACKPACK") {
		character.BACKPACK = (bool)readInt(data);
	}
	if (type == "HITS") {
		character.HITS[0] = readInt(data);
		character.HITS[1] = readInt(data);
	}
	if (type == "MISSES") {
		character.MISSES[0] = readInt(data);
		character.MISSES[1] = readInt(data);
	}
	if (type == "ROW") {
		character.ROW = readInt(data);
	}
	if (type == "ROW_PREFERENCE") {
		character.ROW_PREFERENCE = readInt(data);
	}
	if (type == "LEFT") {
		character.LEFT = readInt(data);
	}
	if (type == "RIGHT") {
		character.RIGHT = readInt(data);
	}
	if (type == "ATTACKS") {
		character.ATTACKS = readInt(data);
	}
	if (type == "CASTS") {
		character.CASTS = readInt(data);
	}
	if (type == "ITEM") {
		Item* item = new Item();
		character.INVENTORY.push_back(item);
		parseItem(item, data);
	}
}

void ProcessMessages() {
	for (;;) {
		Sleep(1);
		while (firstProcess != nullptr) {
			static int numMessages = 0;
			std::string data = firstProcess->data;
			std::string type = firstProcess->type;
			std::cout << type << " " << data << std::endl;
			if (type == "TEXT") {
				std::cout << data << std::endl;
			}
			if (type == "READY") {
				char myhostname[256];
				int rc = gethostname(myhostname, sizeof myhostname);
				std::cout << myhostname << std::endl;
				SendData("LOG_IN", std::string(myhostname));
			}
			if (type == "CHARACTER") {
				std::vector<std::string> strings = split(data, '\n');
				for (std::string str : strings) {
					std::string type = readStr(str);
					CharacterChange(C, type, str);
				}
			}
			Message* temp = firstProcess;
			firstProcess = firstProcess->next;
			delete temp;
		}
		if (bufferStart != nullptr && !SENDING) {
			firstProcess = bufferStart;
			bufferStart = nullptr;
			bufferEnd = nullptr;
		}
	}
}

void Connect() {
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	WSAStartup(ver, &data);

	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, IP.c_str(), &hint.sin_addr);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	int connection = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connection == SOCKET_ERROR) {
		int error = WSAGetLastError();
		std::cout << "Error #" << error << " Retrying Connection. . ." << std::endl;
		Connect();
	}
	std::cout << "Connected." << std::endl;
}

void Listen() {
	char buf[8192];
	ZeroMemory(buf, 8192);
	int disconnects = 0;
	for (;;) {
		Sleep(1);
		int bytesReceived = recv(sock, buf, 8192, 0);
		if (bytesReceived > 1) {
			std::string text = "";
			disconnects = 0;
			int index = nextMessage(messageBuffer);
			for (int i = 0; i < bytesReceived; i++) {
				messageBuffer[index].data += buf[i];
				if (endsWith(messageBuffer[index].data, "!STOP!")) {
					std::string type = "";
					std::string data = "";
					bool setType = true;
					for (int j = 0; j < messageBuffer[index].data.size() - 6; j++) {
						char c = messageBuffer[index].data[j];
						if (setType) {
							if (c == '!') {
								setType = false;
							}
							else {
								type += c;
							}
						}
						else {
							data += c;
						}
					}
					messageBuffer[index].data = data;
					messageBuffer[index].type = type;
					messageBuffer[index].done = true;
					index = nextMessage(messageBuffer);
				}
			}
			std::deque<Message> newMessages = {};
			for (int i = 0; i < messageBuffer.size(); i++) {
				if (messageBuffer[i].done) {
					SENDING = true;
					Message* m = new Message(messageBuffer[i].type, messageBuffer[i].data);
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
					SENDING = false;
				}
				else {
					newMessages.push_back(messageBuffer[i]);
				}
			}
			messageBuffer = newMessages;
			ZeroMemory(buf, 8192);
		}
		else {
			std::cout << "Disconnect Signal. . ." << std::endl;
			disconnects++;
		}
		if (disconnects >= 3) {
			std::cout << "Server Lost. . ." << std::endl;
			closesocket(sock);
			WSACleanup();
			connected = false;
			Connect();
		}
	}
}

void serverInit() {
	std::cout << "Beginning Connection. . ." << std::endl;
	Connect();
	std::thread thread1(Listen);
	std::thread thread2(ProcessMessages);
	std::thread thread3(Input);
	thread1.detach();
	thread2.detach();
	thread3.detach();
}