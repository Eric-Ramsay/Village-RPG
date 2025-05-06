#pragma once

int readInt(std::deque<char>& msg) {
	int total = 0;
	char c = 48;
	while (msg.size() > 0 && c != '!') {
		total *= 10;
		total += (int)(c - 48);
		c = msg.front();
		msg.pop_front();
	}
	return total;
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

void processMessages() {
	for (;;) {
		Sleep(1);
		while (firstProcess != nullptr) {
			static int numMessages = 0;
			std::string msg = firstProcess->data;
			std::string type = firstProcess->type;
			if (type == "TEXT") {
				DATA = msg;
			}
			if (type == "GOLD") {
				//player.gold = readInt(message);
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
	char myhostname[256];
	int rc = gethostname(myhostname, sizeof myhostname);
	std::cout << myhostname << std::endl;
}

bool endsWith(std::string stringOne, std::string stringTwo) {
	if (stringOne.size() < stringTwo.size()) {
		return false;
	}
	for (int i = 0; i < stringTwo.size(); i++) {
		if (stringTwo[stringTwo.size() - i] != stringOne[stringOne.size() - i]) {
			return false;
		}
	}
	return true;
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
				messageBuffer[index].data.push_back(buf[i]);
				if (endsWith(messageBuffer[index].data, "stop!")) {
					std::string type = "";
					std::string data = "";
					bool setType = true;
					for (int j = 0; j < messageBuffer[index].data.size() - 5; j++) {
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
	std::thread listener(Listen);
	listener.detach();
	std::thread processor(processMessages);
	processor.detach();
	std::cout << "Connection Complete!" << std::endl;
}