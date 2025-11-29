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
#include "..\..\Shared\textFunctions.h"
#include "..\..\Shared\sharedStructures.h"
#include "structures.h"
#include "globals.h"
#include "..\..\Shared\sharedFunctions.h"
#include "utilities.h"
#include "audio.h"
#include "draw.h"
#include "server.h"
#include "text.h"

#include "..\..\Shared\init.h"
#include "UI.h"
#include "listener.h"


void handleLoginInput(std::string input) {
	if (UI.signInState == CHOOSE) {
		if (input == "log in") {
			UI.signInState = LOGIN_USERNAME;
		}
		else if (input == "register") {
			UI.signInState = REGISTER_USERNAME;
		}
		stateMessage(UI.signInState);
	}
	else if (UI.signInState == REGISTER_USERNAME) {
		sendData("REGISTER_USERNAME", input);
	}
	else if (UI.signInState == REGISTER_PASSWORD) {
		sendData("REGISTER_PASSWORD", input);
	}
	else if (UI.signInState == LOGIN_USERNAME) {
		USERNAME = input;
		sendData("LOGIN_USERNAME", input);
	}
	else if (UI.signInState == LOGIN_PASSWORD) {
		PASSWORD = input;
		sendData("LOGIN_PASSWORD", input);
	}
}

void initTabs() {
	tabList.push_back(&playerMenu);
	tabList.push_back(&combatMenu);
	tabList.push_back(&viewMenu);

	for (int i = 0; i < tabList.size(); i++) {
		setTabString(*tabList[i]);
	}
}

void initToolTips() {
	TOOLTIPS["VIT"] = Tooltip("Vitality", "*GREEN*+10*GREY* Max *RED*HP*GREY*\n\n*RED*HP*GREY* is a measure of how much *ORANGE*damage*GREY* you can take before your character *BLACK*dies");
	TOOLTIPS["END"] = Tooltip("Endurance", "*GREEN*+10*GREY* Max Stamina\n*GREEN*+1 *GREY*Stamina Regeneration\n\n*GREEN*Endurance*GREY* is used to refill your *YELLOW*AP*GREY* at the start of each turn");
	TOOLTIPS["DEX"] = Tooltip("Dexterity", "*GREEN*+3 AP*GREY* per turn\n\n*YELLOW*AP*GREY* is used to perform actions in your turn, like *ORANGE*attacking*GREY*, *BLUE*casting spells*GREY*, or *GREEN*moving");
	TOOLTIPS["MAG"] = Tooltip("Magic", "*BLUE*+2*GREY* max learned spells\n*BLUE*+1*GREY* spell casts per turn");
	TOOLTIPS["WEP"] = Tooltip("Weapon Handling", "*RED*+1 *GREY*Max Attack Damage\n*RED*+5%*GREY* total attack damage");
	TOOLTIPS["AVD"] = Tooltip("Avoidance", "*GREEN*+5% *GREY*Dodge Chance\n*GREEN*+5% *GREY*Flee Chance\n*YELLOW*+1 AP*GREY* per turn\n\nAvoidance is *ORANGE*capped*GREY* at *RED*10*GREY* points");
	TOOLTIPS["BUTTON_END"] = Tooltip("Ending your Turn", "Once you've done everything you want to do, you should end your turn.\n\nOnce everyone on your team has ended their turn, the enemies will take a turn.");
	TOOLTIPS["BUTTON_BRACE"] = Tooltip("Bracing", "It costs 6 Stamina to Brace. Bracing gives you a 50% chance to dodge the first damage that you would take next turn.");
	TOOLTIPS["BUTTON_GUARD"] = Tooltip("Guarding", "It costs 6 AP to Guard.");
	TOOLTIPS["BUTTON_FLEE"] = Tooltip("Fleeing", "It costs 3 AP to attempt to Flee. If you fail, you can't attack/cast for the remainder of the turn.");

	for (auto person : PEOPLE) {
		std::string header = person.second.NAME;
		std::string body = person.second.DESCRIPTION;
		if (person.second.MERCHANT) {
			header += " *BLACK*|*YELLOW* Merchant";
			body += "\n\n*BLACK*Double-click to trade with " + person.second.NAME;
		}
		TOOLTIPS[person.first] = Tooltip(header, body);
	}
}

void initColors() {
	COLORS["yellow"] = sf::Color(180, 140, 0);
	COLORS["red"] = sf::Color(220, 50, 45);
	COLORS["green"] = sf::Color(135, 155, 0);
	COLORS["brown"] = sf::Color(140, 75, 40);
	COLORS["pink"] = sf::Color(190, 55, 125);
	COLORS["purple"] = sf::Color(160, 90, 250);
	COLORS["blue"] = sf::Color(40, 125, 195);
	COLORS["teal"] = sf::Color(45, 155, 145);
	COLORS["cyan"] = sf::Color(45, 155, 145);
	COLORS["orange"] = sf::Color(190, 75, 40);
	COLORS["black"] = sf::Color(125, 125, 125);
	COLORS["grey"] = sf::Color(245, 245, 240);
	COLORS["white"] = sf::Color(255, 255, 255);

	for (auto color : COLORS) {
		COLORS[caps(color.first)] = color.second;
		COLORS["*" + color.first + "*"] = color.second;
		COLORS["*" + caps(color.first) + "*"] = color.second;
	}
}

void updateTabs(int mX, int mY) {
	for (int i = 0; i < tabList.size(); i++) {
		handleClick(*tabList[i], mX, mY);
	}
}

void hideTabs() {
	for (int i = 0; i < tabList.size(); i++) {
		tabList[i]->visible = false;
	}
}

int main()
{
	initColors();
	initTabs();

	srand(time(NULL));
	std::cout << rand() % 100 << std::endl;
	init();
	initToolTips();

	sf::RenderWindow window;
	sf::RenderTexture texture;
	
	std::cout << "Beginning Connection. . ." << std::endl;
	connect();
	std::thread thread1(listenToServer);
	std::thread thread2(processMessages);
	thread1.detach();
	thread2.detach();

	float FPS = 0;
	bool updateFPS = true;
	auto clock = sf::Clock{};
	float timeSinceLastClick = 0;
	float elapsedTime = 0;
	float targetTime = .04f;


	UI.W = sf::VideoMode::getDesktopMode().width;
	UI.H = sf::VideoMode::getDesktopMode().height;
	window.create(sf::VideoMode(UI.W, UI.H), "Village RPG", sf::Style::Fullscreen);

	window.setMouseCursorVisible(false);
	window.setKeyRepeatEnabled(false);
	window.setVerticalSyncEnabled(true);
	srand(time(NULL));
	//ma_engine_init(NULL, &audio);
	static sf::Texture sprites = createTexture("./Sprites/sprites.png");
	texture.create(WIDTH, HEIGHT);
	vertices.resize(vertSize);

	sf::Shader shader;
	shader.loadFromFile("./Code/shader.frag", sf::Shader::Fragment);

	int xScale = UI.W / WIDTH;
	int yScale = UI.H / HEIGHT;

	

	while (!quit && window.isOpen()) {
		sf::Event event;
		UI.mouseReleased = false;
		UI.mousePressed = false;
		UI.rightPressed = false;
		UI.doubleClicked = false;
		while (window.pollEvent(event)) {
			UI.mX = sf::Mouse::getPosition().x / xScale;
			UI.mY = sf::Mouse::getPosition().y / yScale;
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::LostFocus) {
				UI.inGame = false;
			}
			else if (event.type == sf::Event::GainedFocus) {
				UI.inGame = true;
			}
			else if (UI.inGame) {
				if (event.type == sf::Event::MouseButtonPressed) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						UI.mousePressed = true;
						UI.mouseHeld = true;
					}
					else if (event.mouseButton.button == sf::Mouse::Right) {
						UI.rightPressed = true;
						UI.rightHeld = true;
					}
				}
				else if (event.type == sf::Event::MouseButtonReleased) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						UI.mouseHeld = false;
						UI.mouseReleased = true;
						if (timeSinceLastClick < .5) {
							UI.doubleClicked = true;
							timeSinceLastClick += 1;
						}
						else {
							timeSinceLastClick = 0;
						}
						updateTabs(UI.mX, UI.mY);
					}
					else if (event.mouseButton.button == sf::Mouse::Right) {
						UI.rightReleased = true;
						UI.rightHeld = false;
					}
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
					else if (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::RControl) {
						if (UI.view > 0) {
							UI.viewLocked = !UI.viewLocked;
						}
					}
					else if (event.key.code == sf::Keyboard::Escape) {
						if (UI.signInState < COMPLETED && UI.signInState > CHOOSE) {
							if (UI.signInState == LOGIN_USERNAME) {
								UI.signInState = CHOOSE;
							}
							else {
								UI.signInState = (LOGIN)((int)UI.signInState - 1);
							}
							stateMessage(UI.signInState);
						}
					}
					if (c != 0 && input.size() < 50) {
						input += (char)c;
					}
					else if (event.key.code == sf::Keyboard::Enter) {
						replace(input, "  ", " ");
						logs.push_back("*BLACK*> " + input);

						if (input == "scan") {
							scanLines = !scanLines;
						}
						else if (input == "blur") {
							blur = !blur;
						}
						else if (input == "trade") {
							if (PEOPLE.count(low(input)) > 0) {
								PEOPLE[low(input)].index++;
							}
						}
						else if (input == "quit") {
							quit = true;
						}
						else if (UI.signInState == COMPLETED) {
							sendData("COMMAND", input);
						}
						else {
							handleLoginInput(input);
						}
						input = "";
					}
				}
			}
		}
		Sleep(1);
		if (UI.inGame) {
			if (updateFPS) {
				FPS = 1.f / clock.getElapsedTime().asSeconds();
				updateFPS = false;
			}
			elapsedTime += clock.getElapsedTime().asSeconds();
			timeSinceLastClick += clock.getElapsedTime().asSeconds();
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
			texture.clear(sf::Color(0, 5, 15));

			if (UI.signInState == COMPLETED) {
				hideTabs();
				if (!UI.viewLocked) {
					UI.view = 0;
				}
				UI.tooltip = "";
				DrawUI();
			}
			for (int i = 1; i <= 7; i++) {
				if (logs.size() >= i) {
					Print(logs[logs.size() - i], 10, HEIGHT - (11 + i * 15), 350);
				}
			}
			Print(to_str((int)FPS) + " " + input + "_", 10, HEIGHT - 11, 350);

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
			float xScale = (float)UI.W / texture.getSize().x;
			float yScale = (float)UI.H / texture.getSize().y;
			sprite.setScale(xScale, yScale);

			shader.setUniform("texture", texture.getTexture());
			shader.setUniform("pixel_width", 1.0f / texture.getSize().x);
			shader.setUniform("pixel_height", 1.0f / texture.getSize().y);
			shader.setUniform("big_pixel", 1.0f / window.getSize().y);
			shader.setUniform("scanLines", scanLines);
			shader.setUniform("blur", blur);


			int x = sf::Mouse::getPosition().x;
			int y = sf::Mouse::getPosition().y;
			Draw(0, 144, 16, 16, x, y, 4);

			window.draw(sprite, &shader);

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
