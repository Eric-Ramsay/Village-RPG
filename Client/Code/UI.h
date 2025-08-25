#pragma once

void DrawCharacterUI() {
	if (CHARACTERS.count(ID) > 0) {
		Character C = CHARACTERS[ID];
		Print("*GREEN*" + C.NAME + " *GREY*the *TEAL*" + Title(C.TYPE), 440, 1);
		Print(DrawBar(C.HP, MaxHP(C), 20, "*RED*"), 440, 11);
		Print(DrawBar(C.STAMINA, MaxStamina(C), 20, "*GREEN*"), 440, 21);
		Print("*TEAL*\4 *GREY*" + to_str(C.ARMOR[0]), 440, 31);
		Print("*TEAL*\5 *GREY*" + to_str(C.ARMOR[1]), 440, 41);
	}
}

void DrawBattle() {
	std::string msg = "";
	Print("*RED*" + BATTLE.zone, 1, 1);
	DrawCharacterUI();

	std::vector<std::vector<std::string>> tiles(12, std::vector<std::string>(12, "blank_tile"));

	tiles[0][2] = "water_tile";
	tiles[1][2] = "water_tile";
	tiles[2][2] = "water_tile";
	tiles[2][3] = "water_tile";
	tiles[2][4] = "water_tile";
	tiles[3][4] = "water_tile";
	tiles[4][4] = "water_tile";
	tiles[5][4] = "water_tile";
	tiles[5][3] = "water_tile";
	tiles[6][3] = "water_tile";
	tiles[6][2] = "water_tile";
	tiles[6][1] = "water_tile";
	tiles[6][0] = "water_tile";

	Print("*PINK*Enemies", 200, 18);
	for (int i = 0; i < BATTLE.teams[1].size(); i++) {
		Character C = CHARACTERS[BATTLE.teams[1][i]];
		Print("*RED*P" + to_str(i + 1) + " *GREY*" + C.NAME, 200, 28 + (i * 10));
		tiles[C.Y][C.X] = "*RED*E" + to_str(i + 1);
	}

	Print("*YELLOW*Allies", 200, 107);
	for (int i = 0; i < BATTLE.teams[0].size(); i++) {
		Character C = CHARACTERS[BATTLE.teams[0][i]];
		Print("*GREEN*P" + to_str(i + 1) + " *GREY*" + C.NAME, 200, 117 + (i * 10));
		tiles[C.Y][C.X] = "*GREEN*P" + to_str(i + 1);
	}

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			if (tiles[i][j] == "blank_tile") {
				Draw(0, 80, 16, 16, (j * 16), 11 + (i * 16));
			}
			else if (tiles[i][j] == "water_tile") {
				Draw(16, 80, 16, 16, (j * 16), 11 + (i * 16));
			}
			else {
				Print(tiles[i][j], (3 + j * 16), 15 + (i * 16));
			}
		}
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
	else if (CHARACTERS[ID].LOCATION == BATTLE.id) {
		DrawBattle();
	}
	else {
		DrawRoom();
	}
}