// A place for functions related to drawing things to the screen to reside

// Creates a sprite sheet from a file string
sf::Texture createTexture(std::string file) {
	sf::Texture texture;
	texture.loadFromFile(file);
	return texture;
}


sf::Vertex V(int sX, int sY, int dX, int dY, sf::Color color) {
	return sf::Vertex(sf::Vector2f(dX, dY), color, sf::Vector2f(sX, sY));
}

void SetVertex(int index, int sX, int sY, int dX, int dY, sf::Color color) {
	sf::Vertex* vertex = &vertices[index];
	vertex->position.x = dX;
	vertex->position.y = dY;
	vertex->color = color;
	vertex->texCoords.x = sX;
	vertex->texCoords.y = sY;
}

void Draw(int sX, int sY, int w, int h, int dX, int dY, int scaleX, int scaleY, sf::Color color = sf::Color(255, 255, 255), bool flip = false) {
	if ((numVertices + 4) < vertSize) {
		if (flip) {
			SetVertex(numVertices++, sX, sY, dX + w * scaleX, dY, color);
			SetVertex(numVertices++, sX + w, sY, dX, dY, color);
			SetVertex(numVertices++, sX + w, sY + h, dX, dY + h * scaleY, color);
			SetVertex(numVertices++, sX, sY + h, dX + w * scaleX, dY + h * scaleY, color);
		}
		else {
			SetVertex(numVertices++, sX, sY, dX, dY, color);
			SetVertex(numVertices++, sX + w, sY, dX + w * scaleX, dY, color);
			SetVertex(numVertices++, sX + w, sY + h, dX + w * scaleX, dY + h * scaleY, color);
			SetVertex(numVertices++, sX, sY + h, dX, dY + h * scaleY, color);
		}
	}
	else {
		numVertices += 4;
	}
}

void Draw(int sX, int sY, int w, int h, int dX, int dY, int scale = 1, sf::Color color = sf::Color(255, 255, 255), bool flip = false) {
	Draw(sX, sY, w, h, dX, dY, scale, scale, color, flip);
}

const int PIXEL_SPOT_X = 413;
const int PIXEL_SPOT_Y = 0;

void fillRect(int dX, int dY, int w, int h, sf::Color color) {
	Draw(PIXEL_SPOT_X, PIXEL_SPOT_Y, 1, 1, dX, dY, w, h, color);
}

void DrawSquare(int dX, int dY, int w, int h) {
	fillRect(dX, dY, w, 3, sf::Color::White);
	fillRect(dX, dY, 3, h, sf::Color::White);
	fillRect(dX + w - 3, dY, 3, h, sf::Color::White);
	fillRect(dX, dY + h - 3, w, 3, sf::Color::White);
}

sf::Color getColor(std::string text = "") {
	text = low(text);
	if (text == "gold") {
		return sf::Color(255, 223, 0);
	}
	else if (text == "red") {
		return sf::Color(220, 30, 40);
	}
	else if (text == "alt_red") {
		return sf::Color(220, 75, 70);
	}
	else if (text == "green") {
		return sf::Color(25, 225, 30);
	}
	else if (text == "alt_green") {
		return sf::Color(55, 205, 75);
	}
	else if (text == "blue") {
		return sf::Color(0, 0, 255);
	}
	else if (text == "orange") {
		return sf::Color(255, 165, 0);
	}
	else if (text == "grey") {
		return sf::Color(200, 200, 200);
	}
	else if (text == "brown") {
		return sf::Color(160, 83, 45);
	}

	return sf::Color(255, 255, 255);
}

enum ALIGN {
	LEFT, CENTER, RIGHT
};

//Prints text onto the screen
void Print(std::string text, int dX, int dY, ALIGN align = LEFT, int scale = 1, int textSize = 2, sf::Color textColor = sf::Color(255, 255, 255)) {
	int sX, sY, sW, sH;
	bool printChar = true;
	bool skipping = false;
	sf::Color shade = textColor;
	std::string color = "";
	if (align != LEFT) {
		int textWidth = measureText(text, scale, textSize);
		if (align == CENTER) {
			dX -= textWidth / 2.0;
			charInfo('0', sX, sY, sW, sH, printChar, textSize);
			dY -= sH / 2;
		}
		else {
			dX -= textWidth;
		}
	}
	for (char c : text) {
		if (c == '*') {
			skipping = !skipping;
			if (!skipping) {
				if (getColor(color) != shade) {
					shade = getColor(color);
				}
				else {
					shade = textColor;
				}
				color = "";
			}
		}
		else if (!skipping) {
			charInfo(c, sX, sY, sW, sH, printChar, textSize);
			if (printChar) {
				Draw(sX, sY, sW, sH, dX, dY, scale, shade);
			}
			dX += scale * (sW + 1);
		}
		else {
			color += c;
		}
	}
}

void drawPosition(float x, float y, int size = 1, sf::Color color = sf::Color(255, 255, 255)) {
	Draw(448, 16, 1, 1, x, y, size, color);
}

sf::Color mix(sf::Color a, sf::Color b) {
	float combined = a.a + b.a;
	float aScale = a.a / combined;
	float bScale = b.a / combined;
	int red = (a.r * aScale + b.r * bScale);
	int green = (a.g * aScale + b.g * bScale);
	int blue = (a.b * aScale + b.b * bScale);
	return sf::Color(red, green, blue);
}

sf::Color mix(sf::Color a, float aStr, sf::Color b, float bStr) {
	return mix(sf::Color(a.r, a.g, a.b, aStr), sf::Color(b.r, b.g, b.b, bStr));
}

void DrawUI() {
	
}
