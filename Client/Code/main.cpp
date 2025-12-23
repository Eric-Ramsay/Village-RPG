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

int main()
{
	std::string testText = "*GREY**GREY*Test*GREY*You're bad*RED**BLUE*";

	testText = cleanText(testText);

	initColors();
	initTabs();
	initScrollbars();

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
	float targetTime = 1/60;

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
		UI.rightReleased = false;
		UI.doubleClicked = false;
		UI.scrollDown = false;
		UI.scrollUp = false;
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

				if (animations.size() > 0) {
					animations[0].timePassed++;
					if (animations[0].timePassed >= animations[0].duration) {
						animations.pop_front();
					}
					else {
						float x = animations[0].position.x;
						float y = animations[0].position.y;
						float endX = animations[0].endPos.x;
						float endY = animations[0].endPos.y;
						int timeRemaining = (animations[0].duration - animations[0].timePassed);
						animations[0].position.x += (endX - x) / timeRemaining;
						animations[0].position.y += (endY - y) / timeRemaining;

						if (animations[0].timePassed >= animations[0].beginFade) {
							animations[0].opacity += (animations[0].endOpacity - animations[0].opacity) / timeRemaining;
						}
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
				UI.tooltip = "";
				DrawUI();

				CPrint(to_str((int)FPS), WIDTH / 2, 1);
			}
			else {
				DrawLogs();
			}

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
