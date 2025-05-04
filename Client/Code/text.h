#pragma once

void charInfo(char c, int& sX, int& sY, int& sW) {
	sW = 5;
	sX = TEXT_SMALL_X;
	sY = TEXT_SMALL_Y;
	if (c == '.') {
		sW = 1;
		sX = 26;
		sY = 48;
	}
	else if (c == '+') {
		sY += 16;
	}
	else if (c == '-') {
		sY += 16;
		sX += 8;
	}
	else if (c >= 65 && c <= 90) {
		sX = (c - 65) * 6;
	}
	else if (c >= 97 && c <= 122) {
		sX = (c - 97) * 6;
	}
	else if (c >= 48 && c <= 57) {
		sY = 8;
		sX = 6 * (c - 48);
	}
}

//Returns size of text in pixels
int measureText(std::string text, float scale = 1, int textSize = 2) {
	bool skipping = false;
	int sX, sY, sW, sH;
	int size = -1;
	bool printChar = true;
	for (char& c : text) {
		if (c == '*') {
			skipping = !skipping;
		}
		else if (!skipping) {
			charInfo(c, sX, sY, sW);
			if (c != '\n') {
				size += sW + 1;
			}
		}
	}
	return size * scale;
}

bool range(int x1, int y1, int x2, int y2, int width, int height) {
	if (height == -1) {
		height = width;
	}
	return (x1 >= x2 && y1 >= y2 && x1 <= x2 + width && y1 <= y2 + height);
}

std::string trimNum(float x) {
	char append = '!';
	int excess = 0;
	float num = abs(x);
	num = ((int)std::round(num * 1000)) / 1000.0;
	std::string result = "";
	if (abs(num) < 100) {
		if (abs(x) < 1) { //2 decimal precision
			num = ((int)std::round(num * 100)) / 100.0;
		}
		else { //1 decimal precision
			num = ((int)(num * 10)) / 10.0;
		}
		result = to_str(num);
		if (result[0] == '0' && result[1] == '.') {
			result = result.substr(1);
		}
		return result;
	}
	else { //no decimals allowed
		num = (int)x;
	}
	if (num >= 10000) {
		excess = num;
		if (num >= 1000000) {
			append = 'm';
			num = (int)(num / 1000000);
			excess -= num * 1000000;
			excess = (excess / 100000);
		}
		else {
			append = 'k';
			num = (int)(num / 1000);
			excess -= num * 1000;
			excess = (excess / 100);
		}
	}
	result = to_str(num);
	if (excess > 0 && x < 99999) {
		if (excess - (excess / 100) * 100 == 0) {
			excess = 100 * (excess / 100);
		}
		result += "." + to_str(excess);
	}
	if (append == '!') {
		return result;
	}
	return  result + append;
}

std::string low(std::string a) {
	std::string word = "";
	bool skip = false;
	for (char c : a) {
		if (c == '*') {
			skip = !skip;
		}
		if (skip) {
			word += c;
		}
		else {
			word += std::tolower(c);
		}
	}
	return word;
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
	int sX, sY;
	int sW = 5;
	int sH = 8;
	bool printChar = true;
	bool skipping = false;
	sf::Color shade = textColor;
	std::string color = "";
	if (align != LEFT) {
		int textWidth = measureText(text, scale, textSize);
		if (align == CENTER) {
			dX -= textWidth / 2.0;
			charInfo('0', sX, sY, sW);
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
			charInfo(c, sX, sY, sW);
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