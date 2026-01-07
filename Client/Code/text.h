#pragma once

void charInfo(char c, int& sX, int& sY, int& sW) {
	sW = 5;
	sX = 0;
	sY = 0;

	if (c > 0 && c < 16) {
		sW = 7;
		sY = 57;
		sX = 8 * c;
	}
	if (c == '\1') {
		sW = 5;
	}
	else if (c == ' ') {
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
	else if (c == '[') {
		sW = 2; sX = 59; sY = 48;
	}
	else if (c == ']') {
		sW = 2; sX = 62; sY = 48;
	}
	else if (c == '|') {
		sX = 72; sY = 48; sW = 1;
	}
	else if (c == ')') {
		sX = 75; sY = 48; sW = 2;
	}
	else if (c == '(') {
		sX = 78; sY = 48; sW = 2;
	}
	else if (c == '/') {
		sX = 65; sY = 48; sW = 3;
	}
	else if (c == '%') {
		sX = 86; sY = 48;
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
	else if (c == ';') {
		sX = 82; sY = 48; sW = 2;
	}
	else if (c == ':') {
		sX = 69; sY = 48; sW = 2;
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
	return ((size - 1) * scale);
}

std::string stackStrings(std::string left, std::string right, int spaces) {
	std::string msg = left;
	int num = spaces - left.size();
	for (int i = 0; i < num; i++) {
		msg += " ";
	}
	return msg + right;
}

sf::Color getColor(std::string input = "", int opacity = 255) {
	sf::Color color = COLORS[input];
	color.a = opacity;
	return color;
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

std::vector<std::string> splitLines(std::string text, int maxLength = WIDTH, int scale = 1) {
	bool proceed = (text.size() * 6 * scale > maxLength);
	if (!proceed) {
		for (char c : text) {
			if (c == '\n') {
				proceed = true;
				break;
			}
		}
	}
	if (!proceed) {
		return { text };
	}
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
				if (merged.size() > 0) {
					merged.pop_back();
					merged += "\n";
				}
			}
			else {
				lineLength += len;
			}
			merged += words[i];
			if (i < words.size() - 1) {
				merged += " ";
			}
		}
		merged += "\n";
	}

	return split(merged, '\n');
}

// Print Function
Box Print(std::string text, int dX, int dY, int maxLength = WIDTH, int scale = 1, ALIGN align = LEFT, int opacity = 255) {
	bool printing = true;
	std::string color = "white";
	std::vector<std::string> lines = splitLines(text, maxLength, scale);

	Box box(dX, dY, 0, 0);
	int maxX = 0;

	for (int i = 0; i < lines.size(); i++) {
		int drawX = dX;
		int drawY = dY + 11 * scale * i;
		if (align == CENTER || align == RIGHT) {
			int len = measureText(lines[i]);
			if (align == CENTER) {
				drawX -= len / 2;
			}
			else {
				drawX -= len;
			}
		}
		if (drawX < box.x) {
			box.x = drawX;
		}
		std::string word = "";
		Box wordBox = Box(drawX, drawY - 1, 0, 9);
		for (char c : lines[i]) {
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
					if (color == "sensitive") {
						charInfo('\1', sX, sY, sW);
						Draw(sX, sY - 1, sW, 9, drawX, drawY - 1, scale);
					}
					else {
						Draw(sX, sY, sW, 9, drawX, drawY - 1, scale, getColor(color, opacity));
					}
					drawX += (sW + 1) * scale;
					if (c != ' ') {
						wordBox.w += (sW + 1) * scale;
						word += std::tolower(c);
						if (PEOPLE.count(word) > 0) {
							if (mRange(wordBox)) {
								if (CHARACTERS.count(ID) > 0 && low(CHARACTERS[ID].TRADING) != word) {
									UI.tooltip = word;
									if (UI.doubleClicked) {
										sendData("COMMAND", "trade " + word);
									}
								}
							}
						}
					}
					else {
						word = "";
						wordBox = Box(drawX, drawY, 0, 9);
					}
				}
			}
			else {
				word = "";
				wordBox = Box(drawX, drawY, 0, 9);
				if (c == '*') {
					printing = true;
					color = low(color);
				}
				else {
					color += c;
				}
			}
			if (drawX > maxX) {
				maxX = drawX;
			}
		}
	}

	box.w = maxX - box.x;
	box.h = lines.size() * scale * 11;
	return box;
}

Box CPrint(std::string text, int dX, int dY, int maxLength = WIDTH, int scale = 1, int opacity = 255) {
	return Print(text, dX, dY, maxLength, scale, CENTER, opacity);
}

Box RPrint(std::string text, int dX, int dY, int maxLength = WIDTH, int scale = 1, int opacity = 255) {
	return Print(text, dX, dY, maxLength, scale, RIGHT, opacity);
}

std::string Title(std::string text) {
	std::vector<std::string> lines = split(text);
	for (std::string& line : lines) {
		line[0] = std::toupper(line[0]);
	}
	return join(lines);
}

std::string cleanText(std::string text) {
	std::string lastColor = "GREY";
	std::string nextColor = "";
	bool readingColor = false;
	std::string newText = "";
	for (char c : text) {
		if (c == '*') {
			readingColor = !readingColor;
			if (!readingColor) {
				if (lastColor != nextColor) {
					newText += "*" + nextColor + "*";
				}
				lastColor = nextColor;
			}
			nextColor = "";
		}
		else if (readingColor) {
			nextColor += c;
		}
		else {
			newText += c;
		}
	}
	return newText;

}