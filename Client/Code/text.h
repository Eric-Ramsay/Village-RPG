#pragma once

void charInfo(char c, int& sX, int& sY, int& sW) {
	sW = 5;
	sX = 0;
	sY = 0;
	if (c == '.') {
		sW = 1;
		sX = 26;
		sY = 48;
	}
	if (c == ' ') {
		sX = 156;
	}
	else if (c == '+') {
		sY = 16;
	}
	else if (c == '-') {
		sY = 16;
		sX = 8;
	}
	else if (c == '?') {
		sX = 31; sY = 48;
	}
	else if (c == '!') {
		sX = 27; sY = 48; sW = 1;
	}
	else if (c == ',') {
		sX = 18; sY = 48; sW = 2;
	}
	else if (c == '.') {
		sX = 23; sY = 48; sW = 1;
	}
	else if (c >= 65 && c <= 90) {
		sY = 0;
		sX = (c - 65) * 6;
	}
	else if (c >= 97 && c <= 122) {
		sY = 16;
		sX = (c - 97) * 6;
	}
	else if (c >= 48 && c <= 57) {
		sY = 32;
		sX = 6 * (c - 48);
	}
	if (c == 'i' || c == 't' || c == 'l') {
		sW = 3;
	}
	if (c == 'j' || c == 'k' || c == 'f') {
		sW = 4;
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
	if (text == "yellow") {
		return sf::Color(255, 223, 0);
	}
	else if (text == "red") {
		return sf::Color(220, 30, 40);
	}
	else if (text == "green") {
		return sf::Color(25, 225, 30);
	}
	else if (text == "purple") {
		return sf::Color(140, 90, 240);
	}
	else if (text == "blue") {
		return sf::Color(65, 140, 255);
	}
	else if (text == "teal") {
		return sf::Color(45, 155, 145);
	}
	else if (text == "orange") {
		return sf::Color(255, 165, 0);
	}
	else if (text == "grey") {
		return sf::Color(100, 85, 85);
	}
	else if (text == "brown") {
		return sf::Color(140, 75, 40);
	}

	return sf::Color(245, 245, 240);
}

// Print Function
void Print(std::string text, int dX, int dY, int scale = 1) {
	int drawX = dX;
	int drawY = dY;
	bool printing = true;
	std::string color = "white";
	for (char c : text) {
		if (printing) {
			if (c == '*') {
				printing = false;
				color = "";
			}
			else {
				int sX = 0;
				int sY = 0;
				int sW = 0;
				charInfo(c, sX, sY, sW);
				Draw(sX, sY, sW, 8, drawX, drawY, scale, getColor(color));
				drawX += (sW + 1) * scale;
			}
		}
		else {
			if (c == '*') {
				printing = true;
			}
			else {
				color += c;
			}
		}
	}
}