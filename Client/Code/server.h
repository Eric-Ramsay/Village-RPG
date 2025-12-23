#pragma once

void sendData(std::string type, std::string data) {
	std::string text = str(type) + data + (char)250;
	const int len = text.length();
	char* arr = new char[len + 1];
	for (int i = 0; i < len; i++) {
		arr[i] = text[i];
	}
	send(sock, arr, len, 0);
}


void connect() {
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	WSAStartup(ver, &data);

	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, IP.c_str(), &hint.sin_addr);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	int connection = SOCKET_ERROR;
	while (connection == SOCKET_ERROR) {
		connection = connect(sock, (sockaddr*)&hint, sizeof(hint));
		int error = WSAGetLastError();
		std::cout << "Error #" << error << " Retrying Connection. . ." << std::endl;
		Sleep(10);
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
			disconnects = 0;
			int index = nextMessage(messageBuffer);
			for (int i = 0; i < bytesReceived; i++) {
				if (buf[i] == (char)(250)) {
					messageBuffer[index].done = true;
					index = nextMessage(messageBuffer);
				}
				else {
					messageBuffer[index].data += buf[i];
				}
			}
			std::vector<Message> newMessages = {};
			for (int i = 0; i < messageBuffer.size(); i++) {
				if (messageBuffer[i].done) {
					SENDING = true;
					Message* m = new Message(messageBuffer[i].data);
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