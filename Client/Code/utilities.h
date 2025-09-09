#pragma once

bool range(int x1, int y1, int x2, int y2, int width, int height) {
	if (height == -1) {
		height = width;
	}
	return (x1 >= x2 && y1 >= y2 && x1 <= x2 + width && y1 <= y2 + height);
}

void stateMessage(LOGIN state) {
	if (state == CHOOSE) {
		logs.push_back("Do you want to *PURPLE*log in*GREY* or *ORANGE*register*GREY* a new account?");
	}
	else if (state == LOGIN_USERNAME) {
		logs.push_back("Please enter your *PINK*username");
	}
	else if (state == REGISTER_USERNAME) {
		logs.push_back("Please enter a new *PINK*username");
	}
	else if (state == LOGIN_PASSWORD || state == REGISTER_PASSWORD) {
		logs.push_back("Please enter your *PINK*password");
	}
}