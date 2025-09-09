#pragma once
bool fileExists(std::string name) {
	std::ifstream f(name.c_str());
	return f.good();
}

void parseChange(Account& account, std::string type, std::string data) {
	if (type == "USERNAME") {
		account.USERNAME = readStr(data);
	}
	if (type == "PASSWORD") {
		account.PASSWORD = readStr(data);
	}
}