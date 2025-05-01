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

#include "miniaudio.h"
#include "types.h"
#include "globals.h"
#include "server.h"
#include "text.h"
#include "draw.h"
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
	window.create(sf::VideoMode(UI.W, UI.H), "TG3", sf::Style::Fullscreen);
	window.setKeyRepeatEnabled(false);
	window.setVerticalSyncEnabled(UI.vSync);
	srand(time(NULL));
	//ma_engine_init(NULL, &audio);
	static sf::Texture sprites = createTexture("./Sprites/sprites.png");
	texture.create(UI.W / max(1, UI.zoom), UI.H / max(1, UI.zoom));
	vertices.resize(vertSize);
	while (window.isOpen()) {
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

		//Drawing Code is handled here
		sf::Sprite sprite;
		sprite.setPosition(0, 0);
		texture.clear(sf::Color::Red);
		SCREEN_X = texture.getSize().x;
		SCREEN_Y = texture.getSize().y;

		//Draw()

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

		Print(to_str((int)FPS), UI.W - 2 * UI.scale, UI.H - 40, RIGHT, 2);

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