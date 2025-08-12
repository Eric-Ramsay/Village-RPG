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
	else if (c == '_') {
		sX = 40;
		sY = 48;
		if (UI.blink) {
			sW = 0;
		}
	}
	else if (c == '>') {
		sX = 46;
		sY = 48;
	}
	else if (c == '<') {
		sX = 52;
		sY = 48;
	}
	else if (c == '+') {
		sY = 48;
		sX = 6;
	}
	else if (c == '-') {
		sY = 48;
		sX = 12;
	}
	else if (c == '\'') {
		sX = 37;
		sY = 48;
		sW = 2;
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
	int size = 0;
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
		return sf::Color(245, 245, 240);
	}
	else if (text == "brown") {
		return sf::Color(140, 75, 40);
	}

	return sf::Color(245, 245, 240);
}

int measureText(std::string text, int scale) {
	int lineLength = 0;
	int sX;
	int sY;
	int sW;
	bool printing = true;
	for (char c : text) {
		if (printing) {
			if (c == '*') {
				printing = false;
			}
			else {
				charInfo(c, sX, sY, sW);
				lineLength += (sW + 1) * scale;
			}
		}
		else {
			if (c == '*') {
				printing = true;
			}
		}
	}
	return lineLength;
}

std::string splitLines(std::string text, int maxLength, int scale) {
	std::vector<std::string> lines = split(text, '\n');
	std::string merged = "";
	int sX;
	int sY;
	int sW;
	for (std::string line : lines) {
		int lineLength = 0;
		std::vector<std::string> words = split(line);
		for (int i = 0; i < words.size(); i++) {
			if (words[i] == "or") {
				int ape = 3;
			}
			int len = measureText(words[i] + " ");
			if (lineLength + len >= maxLength) {
				lineLength = len;
				merged.pop_back();
				merged += "\n";
			}
			else {
				lineLength += len;
			}
			merged += words[i] + " ";
		}
		merged += "\n";
	}

	return merged;
}

// Print Function
void Print(std::string text, int dX, int dY, int maxLength = WIDTH, int scale = 1) {
	int drawX = dX;
	int drawY = dY;
	bool printing = true;
	std::string color = "white";
	std::string lines = splitLines(text, maxLength, scale);
	int dYT = dY + 10 * scale;
	/*std::vector<std::string> splitLines = split(lines, '\n');
	for (std::string line : splitLines) {
		fillRect(dX, dYT, measureText(line, scale), scale, sf::Color::White);
		dYT += 11 * scale;
	}*/

	for (char c : lines) {
		if (printing) {
			if (c == '*') {
				printing = false;
				color = "";
			}
			else if (c == '\n') {
				drawX = dX;
				drawY += 11 * scale;
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