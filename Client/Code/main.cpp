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
#include <random>
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
#include "enums.h"
#include "structures.h"
#include "globals.h"
#include "..\..\Shared\sharedFunctions.h"
#include "utilities.h"
#include "audio.h"
#include "draw.h"
#include "server.h"
#include "text.h"

#include "..\..\Shared\init.h"
#include "UI_Elements.h"
#include "UI.h"
#include "listener.h"

void backspaceDescription() {
	int index = UI.description.size() - 1;
	if (index == -1) {
		return;
	}
	if (UI.description[index].text != "") {
		UI.description[index].text.pop_back();
	}
	while (index > -1 && UI.description[index].text == "") {
		UI.description.pop_back();
		index = UI.description.size() - 1;
	}
}

void typeDescription(char c) {
	if (UI.description.size() == 0) {
		UI.description.push_back(TextSegment("*" + UI.color + "*", ""));
	}
	std::string text = "";
	for (TextSegment seg : UI.description) {
		text += seg.color;
		text += seg.text;
	}
	std::vector<std::string> lines = splitLines(text + c, 185, 1);
	if (lines.size() <= 10) {
		UI.description[UI.description.size() - 1].text += c;
	}
}

void weaponReport() {
	std::vector<std::vector<float>> balance = {};
	std::vector<std::vector<std::string>> balanceStr = {};

	for (auto item : ITEMS) {
		if (item.second.type == "weapon") {
			int numToAdd = (item.second.range - balance.size());
			for (int j = 0; j <= numToAdd; j++) {
				balance.push_back({});
				balanceStr.push_back({});
			}
			float dmg = std::floor((2 - item.second.twoHanded) * item.second.attacks * item.second.attack.hitChance * (item.second.attack.max + item.second.attack.min) / 2.0) / 100.0;
			balance[item.second.range].push_back(dmg);

			int apCost = item.second.AP * item.second.attacks * (2 - item.second.twoHanded);

			std::string dmgStr = to_str(dmg);
			std::string apStr = to_str(apCost);
			std::string effStr = to_str(std::floor(100.0 * dmg / (float)apCost) / 100);
			std::string penStr = to_str(item.second.attack.pen);
			std::string textStr = stackStrings(item.second.name, dmgStr, 20);
			textStr = stackStrings(textStr, apStr, 35);
			textStr = stackStrings(textStr, effStr, 50);
			textStr = stackStrings(textStr, penStr, 65);
			balanceStr[item.second.range].push_back(textStr);
		}
	}

	std::string totalStr = "";
	for (int i = 0; i < balance.size(); i++) {
		std::string headerStr = "Range " + to_str(i) + " Weapons";
		headerStr = stackStrings(headerStr, "Damage", 20);
		headerStr = stackStrings(headerStr, "AP Cost", 35);
		headerStr = stackStrings(headerStr, "Efficiency", 50);
		headerStr = stackStrings(headerStr, "Penetration", 65);
		totalStr += headerStr + "\n";
		while (balance[i].size() > 0) {
			int maxIndex = 0;
			for (int j = 1; j < balance[i].size(); j++) {
				if (balance[i][j] > balance[i][maxIndex]) {
					maxIndex = j;
				}
			}
			balance[i].erase(balance[i].begin() + maxIndex);
			totalStr += balanceStr[i][maxIndex] + "\n";
			balanceStr[i].erase(balanceStr[i].begin() + maxIndex);
		}
		totalStr += "\n\n";
	}
	std::cout << totalStr << std::endl;
}

void handleEvents(sf::RenderWindow* window, float& timeSinceLastClick) {
	UI.mouseReleased = false;
	UI.mousePressed = false;
	UI.rightPressed = false;
	UI.rightReleased = false;
	UI.doubleClicked = false;
	UI.scrollDown = false;
	UI.scrollUp = false;
	int xScale = UI.W / WIDTH;
	int yScale = UI.H / HEIGHT;
	sf::Event event;
	while (window->pollEvent(event)) {
		UI.mX = sf::Mouse::getPosition().x / xScale;
		UI.mY = sf::Mouse::getPosition().y / yScale;
		if (event.type == sf::Event::Closed) {
			window->close();
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
					std::cout << timeSinceLastClick << std::endl;
					if (timeSinceLastClick < .5) {
						UI.doubleClicked = true;
						timeSinceLastClick += 1;
					}
					else {
						timeSinceLastClick = 0;
					}
					//updateTabs();
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					UI.rightReleased = true;
					UI.rightHeld = false;
				}
			}
			else if (event.type == sf::Event::MouseWheelScrolled) {
				if (event.mouseWheelScroll.delta > 0) {
					UI.scrollUp = true;
				}
				else {
					UI.scrollDown = true;
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
					if (UI.hairCut) {
						backspaceDescription();
					}
					else if (UI.viewingGraves) {
						if (UI.graveSearch.size() > 0) {
							UI.graveSearch.pop_back();
							updateOrbPos(&graveBar, 0);
						}
					}
					else if (UI.creatingCharacter) {
						if (UI.charName.size() > 0) {
							UI.charName.pop_back();
						}
					}
					else {
						if (input.size() > 0) {
							input.pop_back();
						}
					}
				}
				else if (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::RControl) {
					if (UI.view == 0) {
						if (UI.topLocked || (UI.viewedPlayer != "" && UI.viewedPlayer != ID)) {
							UI.topLocked = !UI.topLocked;
						}
					}
					else {
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
				if (c != 0) {
					if (UI.hairCut) {
						typeDescription((char)c);
					}
					else if (UI.creatingCharacter) {
						if (UI.charName.size() < 20) {
							UI.charName += (char)c;
						}
					}
					else if (UI.viewingGraves) {
						if (UI.graveSearch.size() < 20) {
							UI.graveSearch += (char)c;
							updateOrbPos(&graveBar, 0);
						}
					}
					else if (input.size() < 50) {
						input += (char)c;
					}
				}
				else if (event.key.code == sf::Keyboard::Enter) {
					if (UI.hairCut) {
						typeDescription('\r');
					}
					else {
						input = replace(input, "  ", " ");
						logs.push_back("*BLACK*> " + input);
						if (!logBar.dragging) {
							updateOrbPos(&logBar, logBar.index);
						}

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
	}
}

int main()
{
	std::string testText = "*GREY**GREY*Test*GREY*You're bad*RED**BLUE*";

	testText = cleanText(testText);

	initColors();
	initTabs();
	initScrollbars();

	init();
	weaponReport();
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
	float targetTime = 1.0f/60.0f;

	UI.W = sf::VideoMode::getDesktopMode().width;
	UI.H = sf::VideoMode::getDesktopMode().height;
	window.create(sf::VideoMode(UI.W, UI.H), "Village RPG", sf::Style::Fullscreen);

	window.setMouseCursorVisible(false);
	window.setKeyRepeatEnabled(false);
	window.setVerticalSyncEnabled(false);
	srand(time(NULL));
	//ma_engine_init(NULL, &audio);
	static sf::Texture sprites = createTexture("./Sprites/sprites.png");
	texture.create(WIDTH, HEIGHT);
	vertices.resize(vertSize);
	lineVertices.resize(lineVertSize);

	sf::Shader shader;
	shader.loadFromFile("./Code/shader.frag", sf::Shader::Fragment);

	while (!quit && window.isOpen()) {
		handleEvents(&window, timeSinceLastClick);
		if (UI.inGame) {
			//Sleep(1);
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
				if (UI.timer % 30 == 0) {
					UI.blink = !UI.blink;
				}
				if (UI.timer == 50) {
					UI.showTerrain = true;
				}
				if (UI.timer == 90) {
					UI.showTerrain = false;
				}
				if (UI.timer % 125 == 0) {
					UI.timer = 0;
				}
				updateFPS = true;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
					if (holdingBackspace > 5) {
						if (UI.hairCut) {
							backspaceDescription();
						}
						else if (UI.viewingGraves) {
							if (UI.graveSearch.size() > 0) {
								UI.graveSearch.pop_back();
								updateOrbPos(&graveBar, 0);
							}
						}
						else if (UI.creatingCharacter) {
							if (UI.charName.size() > 0) {
								UI.charName.pop_back();
							}
						}
						else if (input.size() > 0) {
							input.pop_back();
						}
					}
					else {
						holdingBackspace++;
					}
				}
				else {
					holdingBackspace = 0;
				}


				for (auto& animation : ANIMATIONS) {
					for (int l = animation.second.text.size() - 1; l >= 0; l--) {
						if (++animation.second.text[l].timePassed > animation.second.text[l].duration) {
							animation.second.text.erase(animation.second.text.begin() + l);
						}
					}
					for (int l = animation.second.lines.size() - 1; l >= 0; l--) {
						if (++animation.second.lines[l].timePassed > animation.second.lines[l].duration) {
							animation.second.lines.erase(animation.second.lines.begin() + l);
						}
					}
					if (animation.second.move.timePassed < animation.second.move.duration) {
						animation.second.move.timePassed++;
					}
					if (animation.second.fade.timePassed < animation.second.fade.duration) {
						animation.second.fade.timePassed++;
					}
				}
			}

			processMessages();

			sf::Sprite sprite;
			sprite.setPosition(0, 0);
			texture.clear(sf::Color(0, 5, 15));

			updateScrollbars();
			if (UI.signInState == COMPLETED) {
				if (!UI.viewLocked) {
					UI.view = 0;
					UI.viewedEffect = "";
				}
				if (!UI.topLocked) {
					UI.viewedPlayer = ID;
				}
				UI.weaponTooltip = "";
				UI.tooltip = "";
				DrawUI();

				Print(to_str((int)FPS), 2, 1);
				//Print(to_str((int)UI.timer), 16, 1);
			}
			else {
				DrawLogs();
			}

			//DrawLine(Position(WIDTH / 2, HEIGHT / 2), Position(UI.mX, UI.mY), getColor("RED"));

			if (numVertices > vertSize) {
				vertSize = numVertices + 1000;
				vertices.resize(vertSize);
				std::cout << "Resizing to store " << vertSize << " elements. . ." << std::endl;
			}
			if (numLineVertices > lineVertSize) {
				lineVertSize = numLineVertices + 500;
				lineVertices.resize(lineVertSize);
			}

			if (numVertices > 0) {
				texture.draw(&vertices[0], numVertices, sf::Triangles, &sprites);
				numVertices = 0;
			}
			if (numLineVertices > 0) {
				texture.draw(&lineVertices[0], numLineVertices, sf::Lines);
				numLineVertices = 0;
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

			if (numVertices > 0) {
				window.draw(&vertices[0], numVertices, sf::Triangles, &sprites);
				numVertices = 0;
			}
			window.display();
		}
		else {
			clock.restart();
		}
	}
	closesocket(sock);
	WSACleanup();
	//ma_engine_uninit(&audio);
}
