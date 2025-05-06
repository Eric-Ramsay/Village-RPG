#pragma once
#define NOMINMAX
#undef _WINSOCKAPI_
#define _WINSOCKAPI_

// Dependencies
#include <windows.h>
#include <winsock2.h>
#include <deque>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <vector> 
#include <algorithm>
#include <time.h> 
#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

#include "structures.h"
#include "init.h"
#include "server.h"

SOCKET listening;
fd_set master;

int nextMessage(std::vector<Message>& msgs) {
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

void Send() {
	for (;;) {
		Sleep(1);
		if (processing != nullptr) {
			// This gets a list of all clients which are currently available
			for (int i = 0; i < master.fd_count; i++) {
				if (master.fd_array[i] != listening) {
					std::vector<int> newList = {};
					for (int j : processing->players) {
						if (j < players.size()) {
							// The current client is one of the clients this message should be sent to
							if (master.fd_array[i] == players[j].socket) {
								std::string data = processing->type + "!" + processing->data + "stop!";
								std::vector<std::string> dataToSend = {};

								int numStrings = 1 + (int)data.length() / 8000;

								for (int n = 0; n < numStrings; n++) {
									dataToSend.push_back("");
								}

								for (int n = 0; n < data.length(); n++) {
									dataToSend[(int)(n / 8000)] += data[n];
								}

								for (int n = 0; n < dataToSend.size(); n++) {
									std::string str = dataToSend[n];
									if (n > 0) {
										//std::cout << "   ";
									}
									//std::cout << str.length() << " " << processing->type << std::endl;
									send(players[j].socket, str.c_str(), str.length(), 0);
								}
							}
							else if (j > 0 && players[j].connected) {
								newList.push_back(j);
							}
						}
					}
					// This keeps track of which players are yet to receive this message
					processing->players = newList;
				}
			}
			// If the message has no more players to deliver to then move on to the next message in the queue
			if (processing->players.size() == 0) {
				Message* temp = processing;
				processing = processing->next;
				delete temp;
			}
		}
		// If we're not busy, then see if there are any messages in the buffer to process
		else if (!SENDING && bufferStart != nullptr) {
			processing = bufferStart;
			bufferStart = nullptr;
			bufferEnd = nullptr;
		}
	}
}

void AddConnection() {
	SOCKADDR_IN client_info = { 0 };
	int addrsize = sizeof(client_info);

	SOCKET new_client = accept(listening, (struct sockaddr*)&client_info, &addrsize);
	FD_SET(new_client, &master);


	char ipBuf[64];
	int bufSize = sizeof(ipBuf);

	inet_ntop(AF_INET, &client_info.sin_addr, ipBuf, bufSize);

	std::string ip(ipBuf);

	int index = -1;
	for (int i = 0; i < players.size(); i++) {
		if (players[i].IP == ip) {
			std::cout << "Reconnecting Client with IP: " << ip << std::endl;
			index = i;
			players[i].socket = new_client;
			players[i].connected = true;
		}
	}
	if (index == -1) {
		std::cout << "Adding New Client with IP: " << ip << std::endl;
		index = players.size();
		players.push_back(Player(new_client, ip));
	}
	
	EVERYONE.push_back(index);
}

void Listen() {
	FD_ZERO(&master);
	FD_SET(listening, &master);
	for (;;) {
		Sleep(1);
		fd_set copy = master;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		for (int i = 0; i < socketCount; i++) {
			SOCKET current = copy.fd_array[i];
			if (current == listening) {
				AddConnection();
			}
			else {
				// Receive Message From Player
				for (int i = 1; i < players.size(); i++) {
					if (players[i].socket == current) {
						char buf[2048];
						ZeroMemory(buf, 2048);
						int bytesReceived = recv(players[i].socket, buf, 2048, 0);
						if (bytesReceived >= 1) {
							int index = nextMessage(players[i].messages);
							for (int j = 0; j < bytesReceived; j++) {
								if (buf[j] == -128) {
									players[i].messages[index].done = true;
									index = nextMessage(players[i].messages);
								}
								else {
									players[i].messages[index].data.push_back((char)(buf[j] + 127));
								}
							}
						}
						else {
							std::cout << "Player Disconnected" << std::endl;
							RemoveFromEveryone(i);
							players[i].connected = false;
							closesocket(current);
							FD_CLR(current, &master);
						}
					}
				}
			}
		}
		// Interpret Completed Messages from Clients
		for (int i = 1; i < players.size(); i++) {
			std::vector<Message> newMessages = {};
			for (int j = 0; j < players[i].messages.size(); j++) {
				if (players[i].messages[j].done) {
					if (players[i].messages[j].data.size() > 0) {
						std::string msg = players[i].messages[j].data;
						std::string type = players[i].messages[j].type;
						if (type == "") {

						}
					}
				}
				else {
					newMessages.push_back(players[i].messages[j]);
				}
			}
			players[i].messages = newMessages;
		}
	}
}

void Input() {
	for (;;) {
		Sleep(1);
		std::cout << "Enter Command: ";
		std::string text;
		std::getline(std::cin, text);
		sendData(Message("TEXT", text));
		std::cout << text << std::endl;
	}
}

int main() {
	std::cout << "Initializing Server. . ." << std::endl;
	srand(time(NULL));
	int num = rand() % 99999;

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	WSAStartup(ver, &wsData);

	listening = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, IP.c_str(), &hint.sin_addr);

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);

	std::thread thread1(Listen);
	std::thread thread2(Send);
	std::thread thread3(Input);
	thread1.detach();
	thread2.detach();
	thread3.detach();

	FD_ZERO(&master);
	FD_SET(listening, &master);

	std::cout << std::endl << "Server started!" << std::endl;

	for (;;) {
		Sleep(1000);
	}

	return 0;
}