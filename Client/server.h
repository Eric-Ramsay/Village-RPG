#pragma once

std::deque<Message> messageBuffer = {};
bool SENDING = false;
Message* bufferStart = nullptr;
Message* bufferEnd = nullptr;
Message* firstProcess = nullptr;

bool connected = true;
std::string IP = "10.0.0.11";
//std::string IP = "70.136.29.184";
sockaddr_in hint;
int port = 1234;
int localPort = rand() % 10000;
SOCKET sock;

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
			/*if (type == "MAPDATA") {
				WIDTH = readInt(msg);
				HEIGHT = readInt(msg)
			}*/
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
			Player reset;
			player = reset;
			Connect();
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
	std::cout << "Connected!" << std::endl;
}

void serverInit() {
	Connect();
	std::thread listener(Listen);
	listener.detach();
	std::thread processor(processMessages);
	processor.detach();
}