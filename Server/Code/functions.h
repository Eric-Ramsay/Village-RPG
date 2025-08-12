#pragma once
bool fileExists(std::string name) {
	std::ifstream f(name.c_str());
	return f.good();
}