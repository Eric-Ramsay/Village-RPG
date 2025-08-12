#pragma once

void sendData(std::string type, std::string data) {
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

void consoleInput() {
	for (;;) {
		Sleep(1);
		std::string text;
		std::getline(std::cin, text);
		sendData("TEXT", text);
	}
}

void processMessages() {
	for (;;) {
		Sleep(1);
		while (firstProcess != nullptr) {
			static int numMessages = 0;
			std::string data = firstProcess->data;
			std::string type = firstProcess->type;
			std::cout << type << " " << std::endl;
			if (type == "TEXT") {
				logs.push_back(data);
			}
			if (type == "READY") {
				char myhostname[256];
				int rc = gethostname(myhostname, sizeof myhostname);
				std::cout << myhostname << std::endl;
				ID = myhostname;
				sendData("LOG_IN", std::string(myhostname));
			}
			if (type == "CHARACTER") {
				Character C;
				std::vector<std::string> strings = split(data, '\n');
				for (std::string str : strings) {
					std::string stat = readStr(str);
					characterChange(C, stat, str);
				}
				CHARACTERS[C.ID] = C;
			}
			if (type == "STAT") {
				std::string id = readStr(data);
				std::string stat = readStr(data);
				characterChange(CHARACTERS[id], stat, data);
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

void connect() {
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
		connect();
	}
	std::cout << "Connected." << std::endl;
}

void listenToServer() {
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
			connect();
		}
	}
}

void serverInit() {
	std::cout << "Beginning Connection. . ." << std::endl;
	connect();
	std::thread thread1(listenToServer);
	std::thread thread2(processMessages);
	std::thread thread3(consoleInput);
	thread1.detach();
	thread2.detach();
	thread3.detach();
}