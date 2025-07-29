#pragma once

Location GetLocation(std::string id) {
	for (Location loc : LOCATIONS) {
		if (low(loc.id) == low(id)) {
			return loc;
		}
	}
	std::cout << "Error, Location " + id + " not found!" << std::endl;
	return Location("error", "");
}