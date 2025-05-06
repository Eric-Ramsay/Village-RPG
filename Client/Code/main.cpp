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
#pragma comment (lib, "ws2_32.lib")

namespace Gdiplus {
	using std::min;
	using std::max;
};

#include <SFML\Graphics.hpp>

template <typename Type> std::string to_str(const Type& t) {
	std::ostringstream os;
	os << t;
	return os.str();
}

std::string DATA = "";

#include "miniaudio.h"
#include "structures.h"
#include "globals.h"
#include "server.h"
#include "draw.h"
#include "text.h"
#include "audio.h"

int SCREEN_X = 1;
int SCREEN_Y = 1;


int main()
{
	sf::RenderWindow window;
	sf::RenderTexture texture;
	serverInit();

	float FPS = 0;
	bool updateFPS = true;
	auto clock = sf::Clock{};
	float elapsedTime = 0;
	float targetTime = .15f;

	UI.W = sf::VideoMode::getDesktopMode().width;
	UI.H = sf::VideoMode::getDesktopMode().height;
	window.create(sf::VideoMode(UI.W, UI.H), "Village RPG", sf::Style::Fullscreen);

	window.setKeyRepeatEnabled(false);
	window.setVerticalSyncEnabled(UI.vSync);
	srand(time(NULL));
	//ma_engine_init(NULL, &audio);
	static sf::Texture sprites = createTexture("./Sprites/sprites.png");
	texture.create(640, 360);
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
		}
		if (updateFPS) {
			FPS = 1.f / clock.getElapsedTime().asSeconds();
			updateFPS = false;
		}
		elapsedTime += clock.getElapsedTime().asSeconds();
		clock.restart();
		if (elapsedTime >= targetTime) {
			elapsedTime -= targetTime;
			UI.timer++;
			if (UI.timer % 200 == 0) {
				UI.timer = 0;
			}
			updateFPS = true;
		}


		sf::Sprite sprite;
		sprite.setPosition(0, 0);
		texture.clear(sf::Color(0, 0, 10));
		SCREEN_X = texture.getSize().x;
		SCREEN_Y = texture.getSize().y;

		
		//PUT ALL YOUR DRAWING CODE HERE!
		Print("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 50, 100, 1);
		Print(low("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), 50, 125, 1);
		Print("0123456789", 50, 150, 1);
		Print(". , ! ?", 50, 175, 1);
		//Draw(0, 434, 13, 13, player.x, player.y, 2);

		Print("*RED*the *YELLOW*quick *GREEN*brown *BLUE*fox *ORANGE*jumps *PURPLE*over *GREY*the *TEAL*lazy *BROWN*dog. . .", 400, 100, 1);
		Print("sphinx of black quartz, hear my vow!", 400, 125, 1);
		Print("Do Androids Dream of Electric Sheep?", 400, 150, 1);


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

		Print(to_str((int)FPS), 0, 0, 8);

		if (numVertices > 0) {
			window.draw(&vertices[0], numVertices, sf::Quads, &sprites);
			numVertices = 0;
		}
		window.display();
	}
	closesocket(sock);
	WSACleanup();
	//ma_engine_uninit(&audio);
}