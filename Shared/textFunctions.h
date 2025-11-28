#pragma once
template <typename Type> std::string to_str(const Type& t) {
	std::ostringstream os;
	os << t;
	return os.str();
}


std::string operator+(std::string str, int num)
{
	return str + to_str(num);
}

std::string operator+(int num, std::string str)
{
	return str + to_str(num);
}


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
	for (int i = 0; i < str.size(); i++) {
		char c = str[i];
		if (c == delim) {
			if (i < str.size() - 1) {
				strings.push_back("");
			}
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
	for (char c : a) {
		if (c >= 'A' && c <= 'Z') {
			word += (c - 'A') + 'a';
		}
		else {
			word += c;
		}
	}
	return word;
}

std::string caps(std::string a) {
	std::string word = "";
	for (char c : a) {
		if (c >= 'a' && c <= 'z') {
			word += (c - 'a') + 'A';
		}
		else {
			word += c;
		}
	}
	return word;
}

void replace(std::string& str, std::string findText, std::string replaceText) {
	std::string newStr = "";
	int len = findText.length();
	int strLen = str.length();
	for (int i = 0; i < strLen; i += len) {
		std::string segment = "";
		for (int j = 0; j < len; j++) {
			if (i + j < strLen) {
				segment += str[i + j];
			}
		}
		if (segment == findText) {
			segment = replaceText;
		}
		newStr += segment;
	}
	str = newStr;
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

std::vector<int> readInts(std::string& msg) {
	std::vector<int> data = {};
	while (msg.size() > 0) {
		data.push_back(readInt(msg));
	}
	return data;
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

std::string str(int num) {
	return std::to_string(num) + "!";
}

std::string str(std::vector<int> data) {
	std::string text = "";
	for (int num : data) {
		text += str(num);
	}
	return text;
}

std::string str(std::string txt) {
	return txt + "!!!";
}

std::string addLine(std::string stat, std::string value) {
	return str(stat) + str(value) + "\n";
}
std::string addLine(std::string stat, int value) {
	return str(stat) + str(value) + "\n";
}
std::string addLine(std::string stat, std::vector<int> value) {
	return str(stat) + str(value) + "\n";
}

std::string pretty(std::string txt) {
	if (txt.size() == 0) {
		return txt;
	}
	return "" + to_str((char)toupper(txt[0])) + txt.substr(1, txt.size());
}