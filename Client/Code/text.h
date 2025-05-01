#pragma once

const int TEXT_SMALL_X = 0;
const int TEXT_MEDIUM_X = 0;
const int TEXT_LARGE_X = 0;

const int TEXT_SMALL_Y = 576;
const int TEXT_MEDIUM_Y = 544;
const int TEXT_LARGE_Y = 480;

void charInfoSmall(char c, int& sX, int& sY, int& sW, int& sH, bool& printChar) {
	printChar = true;
	sW = 5;
	sH = 7;
	sX = TEXT_SMALL_X;
	sY = TEXT_SMALL_Y;
	if (c == '.') {
		sW = 1;
		sY += 25;
	}
	else if (c == '+') {
		sY += 16;
	}
	else if (c == '-') {
		sY += 16;
		sX += 8;
	}
	else if (c >= 65 && c <= 90) {
		sX += (c - 65) * 6;
	}
	else if (c >= 97 && c <= 122) {
		sX += (c - 97) * 6;
	}
	else if (c >= 48 && c <= 57) {
		sY += 8;
		sX += 6 * (c - 48);
	}
}

void charInfoMedium(char c, int& sX, int& sY, int& sW, int& sH, bool& printChar) {
	printChar = true;
	sW = 8;
	sH = 11;
	sX = TEXT_MEDIUM_X;
	sY = TEXT_MEDIUM_Y;
	if (c == '.') {
		sW = 2; sX += 94; sY += 22;
	}
	else if (c == '+') {

	}
	else if (c == '-') {

	}
	else if (c >= 65 && c <= 90) {
		sX += (c - 65) * 9;
	}
	else if (c >= 97 && c <= 122) {
		sX += (c - 97) * 9;
	}
	else if (c >= 48 && c <= 57) {
		sY += 12;
		sX += 9 * (c - 48);
		if (c == 49) {
			sW = 6;
		}
	}
}

void charInfoLarge(char c, int& sX, int& sY, int& sW, int& sH, bool& printChar) {
	printChar = true;
	sW = 10;
	sH = 14;
	sX = TEXT_LARGE_X;
	sY = TEXT_LARGE_Y;
	if (c == '.') {
		sW = 5; sX += 144; sY += 40;
	}
	else if (c == '+') {

	}
	else if (c == '-') {

	}
	else if (c >= 65 && c <= 90) {
		sX += (c - 65) * 12;
	}
	else if (c >= 97 && c <= 122) {
		sX += (c - 97) * 12;
		sY += 18;
		switch (c) {
		case 'i': case 'l': sW = 2; break;
		case 't': sW = 6; break;
		case 'k': sW = 7; break;
		case 'z': sW = 8; break;
		case 'g': case 'j': case 'y': case 'p': case 'q': sH = 19; break;
		}
	}
	else if (c >= 48 && c <= 57) {
		sY += 40;
		if (c == 49) {
			sW = 6;
		}
		sX += 11 * (c - 48);
	}
}

void charInfo(char c, int& sX, int& sY, int& sW, int& sH, bool& printChar, int textSize) {
	if (c == ' ') {
		printChar = false;
		return;
	}
	if (textSize == 0) {
		return charInfoSmall(c, sX, sY, sW, sH, printChar);
	}
	if (textSize == 1) {
		return charInfoMedium(c, sX, sY, sW, sH, printChar);
	}
	return charInfoLarge(c, sX, sY, sW, sH, printChar);
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
			charInfo(c, sX, sY, sW, sH, printChar, textSize);
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
