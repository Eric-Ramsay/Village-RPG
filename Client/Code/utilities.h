#pragma once

bool endsWith(std::string stringOne, std::string stringTwo) {
	if (stringOne.size() < stringTwo.size()) {
		return false;
	}
	for (int i = 0; i < stringTwo.size(); i++) {
		if (stringOne[(stringOne.size() - stringTwo.size()) + i] != stringTwo[i]) {
			return false;
		}
	}
	return true;
}

std::vector<std::string> split(std::string str, char delim = ' ') {
	std::vector<std::string> strings = { "" };
	for (char c : str) {
		if (c == delim) {
			strings.push_back("");
		}
		else {
			strings[strings.size() - 1] += c;
		}
	}
	return strings;
}

std::string join(std::vector<std::string> words, char delim = ' ') {
	std::string text = "";
	for (int i = 0; i < words.size(); i++) {
		text += words[i];
		if (i < words.size() - 1) {
			text += delim;
		}
	}
	return text;
}

template <typename Type> std::string to_str(const Type& t) {
	std::ostringstream os;
	os << t;
	return os.str();
}

int readInt(std::string& msg) {
	int index = 0;
	int total = 0;
	int mult = 1;
	char c = 48;
	if (msg[0] == '-') {
		mult = -1;
		index++;
	}
	while (msg.size() > index && msg[index] != '!') {
		c = msg[index++];
		total *= 10;
		total += (int)(c - 48);
	}
	if (index >= msg.length()) {
		msg = "";
	}
	else {
		msg = msg.substr(index + 1);
	}
	return mult * total;
}

std::string removeAtEnd(std::string str, int num) {
	if (num > str.length()) {
		return "";
	}
	for (int i = 0; i < num; i++) {
		str.pop_back();
	}
	return str;
}

std::string readStr(std::string& msg) {
	std::string str = "";
	int index = 0;
	while (index < msg.length() && !endsWith(str, "!!!")) {
		str += msg[index++];
	}
	if (index >= msg.length()) {
		msg = "";
	}
	else {
		msg = msg.substr(index);
	}
	return removeAtEnd(str, 3);
}


void addLine(std::string& data, std::string line) {
	data += line + "\n";
}
void addLine(std::string& data, int line) {
	data += to_str(line) + "\n";
}