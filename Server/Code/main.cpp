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
#include <unordered_map>
#include <filesystem>
#include <ctime>
#pragma comment (lib, "ws2_32.lib")

#include "..\..\Shared\sharedStructures.h"
#include "structures.h"
#include "globals.h"
#include "..\..\Shared\textFunctions.h"
#include "functions.h"
#include "..\..\Shared\sharedFunctions.h"
#include "..\..\Shared\init.h"
#include "data.h"
#include "server.h"
#include "AI.h"
#include "battle.h"
#include "commands.h"
#include "listener.h"

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
							else if (j > 0) {
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

void Listen() {
	srand(time(NULL));
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
				sendData("READY", "", { (int)players.size() - 1 });
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
		sendData("TEXT", text);
	}
}

int main() {
	std::cout << "Initializing Server. . ." << std::endl;
	srand(time(NULL));
	rand() % 9999;

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

	initLocations();

	// Load all Characters
	for (const auto& entry : std::filesystem::directory_iterator("./Saves/Characters/")) {
		std::string path = entry.path().string();
		if (path != "./Saves/Characters/Graveyard") {
			Character character = load<Character>(path);
			if (character.ID != "") {
				CHARACTERS[character.ID] = character;
			}
		}
	}

	for (const auto& entry : std::filesystem::directory_iterator("./Saves/Battles/")) {
		Battle battle = load<Battle>(entry.path().string());
		BATTLES[battle.id] = battle;
		validateBattle(battle.id);
	}

	for (const auto& entry : std::filesystem::directory_iterator("./Saves/Accounts/")) {
		Account account = load<Account>(entry.path().string());
		ACCOUNTS[account.USERNAME] = account;
	}

	std::cout << std::endl << "Server started!" << std::endl;

	for (;;) {
		Sleep(1000);
	}

	return 0;
}