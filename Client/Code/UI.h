#pragma once

void DrawBattle() {
	std::string msg = "";
	msg += "*GREEN*ALLIES\n";
	for (std::string ally : BATTLE.teams[0]) {
		//msg += "*GREEN*" + ally->NAME + "  *GREY*" + ally->HP + "/" + MaxHP(*ally) + "  " + "Row " + ally->ROW + "\n";
	}

	msg += "*RED*Enemies\n";
	for (std::string enemy : BATTLE.teams[1]) {
		//msg += "*RED*" + enemy + "  *GREY*" + enemy->HP + "/" + MaxHP(*enemy) + "  " + "Row " + enemy->ROW + "\n";
	}
}

void DrawRoom() {
	std::string msg = "";
	Location room = getLocation(CHARACTERS[ID].LOCATION);
	if (room.parent != "") {
		msg = "*RED*" + room.parent + "*GREY* - *BLUE*" + room.id + "*GREY*\n";
	}
	else {
		msg = "*RED*" + room.id + "*GREY*\n";
	}

	msg += room.description + "\n\n";

	if (room.buildings.size() > 0) {
		msg += "*BLUE*Buildings\n";
		for (std::string building : room.buildings) {
			msg += "*YELLOW*" + building + "\n";
		}
		msg += "\n";
	}

	for (Connection con : room.connections) {
		msg += "*GREEN*" + con.direction + "*GREY* - *BLUE*" + con.location + "\n";
	}
	Print(msg, 20, 20, WIDTH-40);
}

void DrawUI() {
	if (CHARACTERS.count(ID) == 0) {
		Print("No Character!", 50, 50);
	}
	DrawRoom();
}