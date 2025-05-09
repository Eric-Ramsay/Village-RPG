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
#include <fstream>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

#include "structures.h"
#include "functions.h"
#include "init.h"
#include "data.h"
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
								std::string data = processing->type + "!" + processing->data + "!STOP!";
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

// Interpret Completed Messages from Clients
void ProcessMessages() {
	Sleep(1);
	for (int i = 0; i < players.size(); i++) {
		std::vector<Message> newMessages = {};
		for (int j = 0; j < players[i].messages.size(); j++) {
			if (players[i].messages[j].done) {
				if (players[i].messages[j].data.size() > 0) {
					std::string data = players[i].messages[j].data;
					std::string type = players[i].messages[j].type;
					std::cout << type << " " << data << std::endl;
					if (type == "LOG_IN") {
						if (fileExists("Saves/" + data + ".txt")) {
							players[i].character = loadCharacter(data);
							SendCharacter(i);
						}
						// Loop through the saved characters and return the one that belongs to them
					}
					else if (type == "TEXT") {
						SendData(type, data, { i });
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

void Listen() {
	FD_ZERO(&master);
	FD_SET(listening, &master);
	for (;;) {
		Sleep(1);
		fd_set copy = master;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		if (socketCount > 1) {
			std::cout << socketCount << std::endl;
		}
		for (int i = 0; i < socketCount; i++) {
			SOCKET current = copy.fd_array[i];
			if (current == listening) {
				std::cout << "Connecting Player" << std::endl;
				SOCKET new_client = accept(listening, nullptr, nullptr);
				if (new_client == SOCKET_ERROR) {
					int error = WSAGetLastError();
					std::cout << "Error #" << error << std::endl;
				}
				FD_SET(new_client, &master);
				FD_SET(new_client, &copy);

				players.push_back(Player(new_client));
				SendData("READY", "", { (int)players.size() - 1 });
			}
			else {
				// Receive Message From Player
				for (int i = 0; i < players.size(); i++) {
					if (players[i].socket == current) {
						char buf[2048];
						ZeroMemory(buf, 2048);
						int bytesReceived = recv(players[i].socket, buf, 2048, 0);
						if (bytesReceived >= 1) {
							int index = nextMessage(players[i].messages);
							std::string buffer = players[i].messages[index].data;
							for (int j = 0; j < bytesReceived; j++) {
								buffer += buf[j];
								if (endsWith(buffer, "!STOP!")) {
									std::string type = "";
									std::string data = "";
									bool setType = true;
									for (int j = 0; j < buffer.size() - 6; j++) {
										char c = buffer[j];
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
									players[i].messages[index].data = data;
									players[i].messages[index].type = type;
									players[i].messages[index].done = true;
									index = nextMessage(players[i].messages);
								}
							}
						}
						else {
							std::cout << "Player Disconnected" << std::endl;
							players.erase(players.begin() + i);
							closesocket(current);
							FD_CLR(current, &master);
						}
					}
				}
			}
		}
		ProcessMessages();
	}
}

void Input() {
	for (;;) {
		Sleep(1);
		std::string text;
		std::getline(std::cin, text);
		SendData("TEXT", text);
	}
}

int main() {
	Character Bob;
	Item weapon("Axe", "axe", "An axe", 2, 1, 50, 75, 2, 8, 4, 6, 1);
	Item armor("Chainmail", "Some old chainmail.", 50, 6, 25, 4, 2);
	Bob.INVENTORY.push_back(&weapon);
	Bob.INVENTORY.push_back(&armor);
	Bob.NAME = "Bob";
	Bob.ID = "Bob1234";
	Bob.DESCRIPTION = "Bob is an Ape";
	Bob.LEVEL = 10;
	Bob.XP = 150;
	Bob.SP = 3;

	saveChar(Bob);
	Bob = loadChar("Bob1234");
	//std::cout << ((Weapon*)Bob.INVENTORY[0])->min << std::endl;
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