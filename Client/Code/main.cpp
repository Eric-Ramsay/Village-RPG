#define NOMINMAX
#define MINIAUDIO_IMPLEMENTATION
#define WIN32_LEAN_AND_MEAN
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <algorithm>
#include <vector>
#include <windows.h>
#include <deque>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <WS2tcpip.h>
#include <thread>
#include <unordered_map>
#pragma comment (lib, "ws2_32.lib")

namespace Gdiplus {
	using std::min;
	using std::max;
};

#include <SFML\Graphics.hpp>

#include "miniaudio.h"
#include "structures.h"
#include "..\..\Shared\sharedStructures.h"
#include "globals.h"
#include "..\..\Shared\textFunctions.h"
#include "..\..\Shared\sharedFunctions.h"
#include "utilities.h"
#include "audio.h"
#include "server.h"
#include "listener.h"
#include "draw.h"
#include "text.h"

#include "..\..\Shared\init.h"
#include "UI.h"


int main()
{
	srand(time(NULL));
	std::cout << rand() % 100 << std::endl;
	initLocations();

	sf::RenderWindow window;
	sf::RenderTexture texture;
	
	std::cout << "Beginning Connection. . ." << std::endl;
	connect();
	std::thread thread1(listenToServer);
	std::thread thread2(processMessages);
	std::thread thread3(consoleInput);
	thread1.detach();
	thread2.detach();
	thread3.detach();

	float FPS = 0;
	bool updateFPS = true;
	auto clock = sf::Clock{};
	float elapsedTime = 0;
	float targetTime = .04f;

	UI.W = sf::VideoMode::getDesktopMode().width;
	UI.H = sf::VideoMode::getDesktopMode().height;
	window.create(sf::VideoMode(UI.W, UI.H), "Village RPG", sf::Style::Fullscreen);

	window.setKeyRepeatEnabled(false);
	window.setVerticalSyncEnabled(UI.vSync);
	srand(time(NULL));
	//ma_engine_init(NULL, &audio);
	static sf::Texture sprites = createTexture("./Sprites/sprites.png");
	texture.create(WIDTH, HEIGHT);
	vertices.resize(vertSize);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::LostFocus) {
				UI.inGame = false;
			}
			else if (event.type == sf::Event::GainedFocus) {
				UI.inGame = true;
			}
			else if (event.type == sf::Event::KeyPressed) {
				int c = 0;
				int keyCode = (int)event.key.code;
				if (keyCode >= sf::Keyboard::A && keyCode <= sf::Keyboard::Z) {
					char add = 'a';
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
						add = 'A';
					}
					c = add + (keyCode - (int)sf::Keyboard::A);
				}
				else if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9) {
					c = (char)(event.key.code - sf::Keyboard::Num0 + '0');
				}
				else if (event.key.code == sf::Keyboard::Space) {
					c = (char)(event.key.code - sf::Keyboard::Space + ' ');
				}
				else if (event.key.code == sf::Keyboard::Backspace) {
					if (input.length() > 0) {
						input.pop_back();
					}
				}
				else if (event.key.code == sf::Keyboard::Right) {
					sendData("COMMAND", "MOVE " + str(CHARACTERS[ID].X + 1) + str(CHARACTERS[ID].Y));
				}
				else if (event.key.code == sf::Keyboard::Left) {
					sendData("COMMAND", "MOVE " + str(CHARACTERS[ID].X - 1) + str(CHARACTERS[ID].Y));
				}
				else if (event.key.code == sf::Keyboard::Up) {
					sendData("COMMAND", "MOVE " + str(CHARACTERS[ID].X) + str(CHARACTERS[ID].Y - 1));
				}
				else if (event.key.code == sf::Keyboard::Down) {
					sendData("COMMAND", "MOVE " + str(CHARACTERS[ID].X) + str(CHARACTERS[ID].Y + 1));
				}
				if (c != 0) {
					input += (char)c;
				}
				else if (event.key.code == sf::Keyboard::Enter) {
					replace(input, "  ", " ");
					sendData("COMMAND", input);
					logs.push_back(input);
					input = "";
				}
			}
		}
		if (UI.inGame) {
			if (updateFPS) {
				FPS = 1.f / clock.getElapsedTime().asSeconds();
				updateFPS = false;
			}
			elapsedTime += clock.getElapsedTime().asSeconds();
			clock.restart();
			if (elapsedTime >= targetTime) {
				elapsedTime -= targetTime;
				UI.timer++;
				if (UI.timer % 15 == 0) {
					UI.blink = !UI.blink;
				}
				if (UI.timer % 200 == 0) {
					UI.timer = 0;
				}
				updateFPS = true;
				if (input.length() > 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
					if (holdingBackspace > 5) {
						input.pop_back();
					}
					else {
						holdingBackspace++;
					}
				}
				else {
					holdingBackspace = 0;
				}
			}

			sf::Sprite sprite;
			sprite.setPosition(0, 0);
			texture.clear(sf::Color(0, 0, 10));
			SCREEN_X = texture.getSize().x;
			SCREEN_Y = texture.getSize().y;

			//PUT ALL YOUR DRAWING CODE HERE!
			/*Print("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 50, 100, 1);
			Print(low("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), 50, 125, 1);
			Print("0123456789", 50, 150, 1);
			Print(". , ! ?", 50, 175, 1);
			//Draw(0, 434, 13, 13, player.x, player.y, 2);

			Print("*RED*the *YELLOW*quick *GREEN*brown *BLUE*fox *ORANGE*jumps *PURPLE*over *GREY*the *TEAL*lazy *BROWN*dog. . .", 400, 100, 1);
			Print("sphinx of black quartz, hear my vow!", 400, 125, 1);
			Print("Do Androids Dream of Electric Sheep?", 400, 150, 1);
			Print(C.NAME, 10, 10);
			Print(C.DESCRIPTION, 10, 30);
			Print(C.ID, 10, 50);
			Print(C.TYPE, 10, 70);
			Print(C.LOCATION, 10, 90);
			Print(C.BUILDING, 10, 110);
			Print(to_str(C.LEVEL), 10, 130);
			Print(to_str(C.XP), 10, 150);
			Print(to_str(C.SP), 10, 170);
			Print("Gold " + to_str(C.GOLD), 10, 190);
			Print(to_str(C.HP), 10, 210);
			Print(to_str(C.AP), 10, 230);
			Print(to_str(C.BACKPACK), 10, 250);
			Print(to_str(C.HITS[0]), 10, 270);
			Print(to_str(C.HITS[1]), 10, 290);
			Print(to_str(C.MISSES[0]), 10, 310);
			Print(to_str(C.MISSES[1]), 10, 330);
			*/

			//fillRect(0, 0, 320, 360, sf::Color::Red);

			//fillRect(319, 0, 2, 360, sf::Color::Red);

			//fillRect(638, 0, 2, 360, sf::Color::Red);

			DrawUI();


			for (int i = 1; i <= 3; i++) {
				if (logs.size() >= i) {
					Print(logs[logs.size() - i], 10, HEIGHT - (50 + i * 15));
				}
			}
			Print("> " + input + "_", 10, HEIGHT - 50);

			if (numVertices > vertSize) {
				vertSize = numVertices + 1000;
				vertices.resize(vertSize);
				std::cout << "Resizing to store " << vertSize << " elements. . ." << std::endl;
			}

			if (numVertices > 0) {
				texture.draw(&vertices[0], numVertices, sf::Quads, &sprites);
				numVertices = 0;
			}
			texture.display();
			sprite.setTexture(texture.getTexture());
			sprite.setScale((UI.W) / texture.getSize().x, UI.H / texture.getSize().y);
			window.draw(sprite);


			//Print(to_str((int)FPS), 0, 0, 8);

			if (numVertices > 0) {
				window.draw(&vertices[0], numVertices, sf::Quads, &sprites);
				numVertices = 0;
			}
			window.display();
		}
	}
	closesocket(sock);
	WSACleanup();
	//ma_engine_uninit(&audio);
}
