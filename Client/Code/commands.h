#pragma once
std::string CommandTravel(std::vector<std::string> words, int index) {
	if (words.size() == 0) {
		return "*RED*Command not understood. . . ";
	}
	std::string direction = "";
	Location location = GetLocation(C.LOCATION);
	if (words[0] == "north" || words[0] == "n") {
		direction = "north";
	}
	if (words[0] == "south" || words[0] == "s") {
		direction = "south";
	}
	if (words[0] == "west" || words[0] == "w") {
		direction = "west";
	}
	if (words[0] == "east" || words[0] == "e") {
		direction = "east";
	}
	if (direction != "") {
		for (Connection con : location.connections) {
			if (low(con.direction) == direction) {
				C.LOCATION = con.location;
				return "You travel " + direction + ".";
			}
		}
	}
	if (words[0] == "to") {
		words.erase(words.begin());
	}
	Location destination = GetLocation(join(words));
	if (destination.id == "error") {
		if (words[0] == "the") {
			words.erase(words.begin());
			destination = GetLocation(join(words));
		}
		if (destination.id == "error") {
			return "*RED*Couldn't find '" + join(words) + "'!";
		}
	}
	C.LOCATION = destination.id;
	return "You travel to the " + destination.id;
}

/*
function CommandTravel(words, C, index) {
	words.splice(0, 1);
	let combatMsg = "";
	let msg = "";
	let tryGo = false;
	if (index == -1) {
		tryGo = true;
	}
	let oldRoom;
	let newRoom;
	C.TRADING = "";
	if (index > -1) {
		if (hasEffect(C, "stunned")) {
			return "*RED*You can't act while you're stunned!\n";
		}
		if (battles[index].started && C.ENDED) {
			return "*RED*You can't act while your turn is ended!\n";
		}
		if (battles[index].started && hasEffect(C, "rooted")) {
			return "*RED*You're rooted and can't move!\n";
		}
		let direction = 1;
		let count = 1;
		if (words[0] == "left") {
			direction = -1;
		}
		else if (words[0] == "right") {
			direction = 1;
		}
		else if (words[0] == "to") {
			if (words[1][0].toLowerCase() == "r" && words[1].length == 2) {
				let num = parseInt(words[1][1]) - 1;
				if (num < 0 || num > 4) {
					return "*RED*You can't go to that row.\n";
				}
				count = num - C.ROW;
				direction = count/Math.abs(count);
				count = Math.abs(count);
				if (direction == 0) {
					return "*RED*You're already on that row.\n";
				}
			}
			else {
				tryGo = true;
			}
		}
		else {
			tryGo = true;
		}
		if (!tryGo) {
			for (let n = 0; n < count; n++) {
				if (C.ROW + direction >= 0 && C.ROW + direction <= 4) {
					let cost = 3;
					if (hasEffect(C, "slowed")) {
						cost += 3;
					}
					if (direction > 0) {
						for (const enemy of battles[index].enemies) {
							if (enemy.ROW == C.ROW && hasEffect(enemy, "blocking")) {
								return msg + "*RED*" + P(Name(enemy)) + " blocks your path!\n";
							}
						}
					}
					if (C.AP >= cost || !battles[index].started) {
						if (battles[index].started) {
							C.AP -= cost;
						}
						C.ROW += direction/Math.abs(direction);
						msg += C.NAME + " moves ";
						if (direction > 0) {
							msg += "*GREEN*right*GREY*.\n";
						}
						else {
							msg += "*GREEN*left*GREY*.\n";
						}
					}
					else {
						return msg + "*RED*You don't have the AP to move any farther.\n";
					}
				}
				else {
					return msg + "*RED*You can't go any farther that way.\n";
				}
			}
		}
	}
	//Out of Combat movement
	if (tryGo && (index == -1 || !(battles[index].started))) {
		msg = "";
		if (C.BUILDING != "") {
			C.BUILDING = "";
		}
		if (words.length == 0) {
			return "";
		}

		if (words[0] == "to") {
			words.splice(0, 1);
			if (words[0] == "the") {
				words.splice(0, 1);
			}
		}
		let args = words.join(" ").toLowerCase();
		let altArgs = "the " + args;

		if (words[0].length <= 2) {
			words[0] = words[0].replace("s", "south");
			words[0] = words[0].replace("e", "east");
			words[0] = words[0].replace("n", "north");
			words[0] = words[0].replace("w", "west");
		}

		let room;
		let prosperity = data["town"].prosperity;
		for (let i = 0; i < locations.length; i++) {
			if (locations[i].id.toLowerCase() == C.LOCATION.toLowerCase()) {
				for (const connection of locations[i].connections) {
					if (words[0] == connection.direction) {
						for (let j = 0; j < locations.length; j++) {
							if (locations[j].id == connection.id) {
								if (prosperity < locations[j].prosperity) {
									return "*RED*The town isn't prosperous enough yet to travel to this location.";
								}
								if (locations[j].cost > C.GOLD) {
									return "*RED*You don't have enough gold to travel there.";
								}
								if (locations[j].cost > 0) {
									C.GOLD -= locations[j].cost;
									msg += "*GREY*You pay *YELLOW*" + locations[j].cost + " Gold*GREY*.\n";
								}
								Travel(C, locations[j], index);
								return "";
							}
						}
					}
				}
			}
		}

		for (let i = 0; i < locations.length; i++) {
			let location = locations[i].id.toLowerCase();
			if (location == args || location == altArgs) {
				if (!locations[i].canTravelHere) {
					return "*RED*You can't go that way.\n";
				}
				if (prosperity < locations[i].prosperity) {
					return "*RED*The town isn't prosperous enough yet to travel to this location.";
				}
				if (locations[i].cost > C.GOLD) {
					return "*RED*You don't have enough gold to travel there.";
				}
				if (locations[i].cost > 0) {
					C.GOLD -= locations[i].cost;
					msg += "*GREY*You pay *YELLOW*" + locations[i].cost + " Gold*GREY*.\n";
				}
				Travel(C, locations[i], index);
				msg = "*GREEN*You travel to *BLUE*" + locations[i].id + "\n";
				return msg;
			}
			for (const building of locations[i].buildings) {
				let name = building.id.toLowerCase();
				if (name == args || name == altArgs) {
					if (prosperity < locations[i].prosperity || prosperity < building.prosperity) {
						return "*RED*The town isn't prosperous enough yet to travel to this location.";
					}
					if (location != C.LOCATION.toLowerCase()) {
						if (locations[i].cost > C.GOLD) {
							return "*RED*You don't have enough gold to travel there.";
						}
						if (locations[i].cost > 0) {
							C.GOLD -= locations[i].cost;
							msg += "*GREY*You pay *YELLOW*" + locations[i].cost + " Gold*GREY*.\n";
						}
						Travel(C, locations[i], index);
						msg = "*GREEN*You travel to *BLUE*" + locations[i].id + "*GREY* and enter the *GREEN*" + Prettify(name) + ".\n";
					}
					C.BUILDING = name;
					return msg;
				}
			}
		}
		return "*RED*You can't go that way.\n";
	}
	return msg;
}*/

void Command(std::string input) {
	std::string msg = "";
	replace(input, "  ", " ");
	std::vector<std::string> words = split(low(input));
	std::string keyword = words[0];
	words.erase(words.begin());
	if (keyword == "go" || keyword == "enter" || keyword == "travel") {
		msg = CommandTravel(words, -1);
	}
}
