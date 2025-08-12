#pragma once

bool range(int x1, int y1, int x2, int y2, int width, int height) {
	if (height == -1) {
		height = width;
	}
	return (x1 >= x2 && y1 >= y2 && x1 <= x2 + width && y1 <= y2 + height);
}