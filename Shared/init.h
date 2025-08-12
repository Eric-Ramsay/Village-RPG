#pragma once

void initLocations() {
	// Buildings
	Location Tavern("Tavern", "The *BLUE*Tavern*GREY* is four stories, and boasts no fewer than twenty-five rooms for rent, though these days only a few are ever rented out at a time. More than a few townspeople come to the *RED*firelit*GREY* common room for dinner or drinks at its bar, which is enough for *YELLOW*Penelope*GREY* to maintain the establishment. It's said that *GREEN*Asha*GREY*, sitting perpetually at the bar and living in a rented room accounts for half of the tavern's total revenue.");
	Tavern.parent = "Merchant's Lane";

	Location Tailor("Tailor", "The *BLUE*Tailor Shop*GREY* is well stocked with all the latest gowns and garments, intricately crafted by *YELLOW*Florence*GREY* herself. Painted a dark brown in its interior to accent the colorful fabrics within, it's one of the newest additions to *RED*Merchant's Lane*GREY*. Behind several fashionable displays, hidden in a back corner, is a mannequin adorned in old *CYAN*highland chainmail*GREY*. When asked about it, *YELLOW*Florence*GREY* only looks away with a sly, fading laugh.");
	Tavern.parent = "Merchant's Lane";

	Location Barber("Barber", "The *BLUE*Barbershop*GREY* is a single, plainly furnished room at the front of *GREEN*Nestra*GREY*'s estate, in what used to be an antechamber. The shop's greatest feature is its window to the estate's atrium garden with its exquisite *WHITE*fountain*GREY*, *WHITE*statues*GREY*, and *WHITE*pillars*GREY*. Haircuts are *YELLOW*15 Gold*GREY*, type *CYAN*!haircut [description]*GREY* to give yourself a description. You can use *CYAN*!parse [description]*GREY* to preview your haircut.");
	Tavern.parent = "Merchant's Lane";

	Location Curio("Curio Shop", "The *BLUE*Curio Shop*GREY* is a maze of narrow hallways and tiny rooms, formed by hanging carpets and shelves that rise from the floor to the low ceiling. Every imaginable sort of thing can be found cluttered onto a shelf somewhere in the shop, from ancient artifacts to tarnished copper toys. *YELLOW*Minsiki*GREY* scours every merchant ship that pulls into the harbor, looking for more *PINK*strange wares*GREY* for his shop.");
	Tavern.parent = "Harbor";

	Location Runes("Rune Shop", "The *BLUE*Rune Shop*GREY* is a dusty wooden building, jointly owned by Sarkana and Terat in spite of their animosity. They've hung a sheet from the ceiling to divide the space behind the counter in half. *GREEN*Sarkana*GREY* and *GREEN*Terat*GREY* watch eagerly to see whose runes you intend to buy.");
	Tavern.parent = "Harbor";

	Location Church("Church", "The *BLUE*Church*GREY* is an ancient building, and by far the nicest in town. Its walls are built of an odd assortment of mismatched tan and grey stones, all carefully and sturdily laid together by long-dead craftsmen. The windows are clear *CYAN*quartz*GREY*, and the pews are fine mahogany, though scarcely do they see use these days. *GREEN*Father Tobin*GREY* tends to the grounds by himself, praying he might one day find a successor.");
	Tavern.parent = "Churchyard";

	Location Library("Library", "The *BLUE*Library*GREY* is an old building, discreetly tucked into a corner of the *YELLOW*plaza*GREY*. The books are all held in a single spacious room, partitioned by a *CYAN*labyrinth*GREY* of tall bookshelves, some of which are subdivided by a wooden platform that acts as a partial second floor. The air is stale and dusty, and if you wander past the *GREEN*books*GREY* you'll come upon a bedroom and office where *PINK*Elias*GREY* lives and works.");
	Tavern.parent = "Churchyard";

	Location Bank("Bank", "The *BLUE*Bank*GREY* is a low, unassuming building of *BLACK*old grey stone*GREY*. It has a single circular window on its front face, but is otherwise lit by *YELLOW*dim lamps*GREY*. Despite its modest appearance the building has been reinforced by *PINK*ancient runes*GREY* that completely protect any and all *CYAN*deposits*GREY*.");
	Tavern.parent = "Town Square";

	Location Apothecary("Apothecary", "The *BLUE*Apothecary*GREY* is little more than a tiny room and a counter, even smaller than *YELLOW*Gout*GREY*'s *BLUE*bait shop*GREY*. There is a door behind the counter which none but *YELLOW*Qarana*GREY* have seen the other side of. On each wall are shelves of glass potion bottles that rattle as the shop door opens and closes. Within the shop a slight metallic, almost *RED*bloody*GREY* smell lingers, mixing with a *GREEN*sour*GREY*, smoky scent. If ever it's mentioned, *YELLOW*Qarana*GREY* insists it's coming from the *YELLOW*Kobos*GREY*'s stall next door.");
	Tavern.parent = "Town Square";

	Location Smithy("Smithy", "The *BLUE*Smithy*GREY* is more of a pavillion rather than a shop, always shimmering in the *RED*fiery*GREY* radiance of the ingot furnace. Most of *YELLOW*Kobos*GREY*'s work consists of mundane orders for the town, nails, fish hooks, anchors, and the like, though many adventurers seek him out for his legendary weapons and armor. Many of his finest masterpieces hang behind his counter. *YELLOW*Kobos*GREY* doesn't worry about theft, though - he knows no one would dare to *RED*steal*GREY* from him again.");
	Tavern.parent = "Town Square";

	// Locations
	Location Merchants("Merchant's Lane", "A wide dusty road flanked by various wooden shops and market stalls. The air is filled with the clamor of townspeople going about their business. Dark pines loom in the west, far beyond the comforts of the town.");
	Merchants.buildings = { "Tavern", "Tailor", "Barber" };
	Merchants.connections = {
		Connection("south", "Town Square"),
		Connection("north", "Harbor"),
		Connection("west", "Wilted Woods")
	};

	Location Harbor("Harbor", "The harbor on the north side of town. The coast seems endless to the east and west, and a cool southbound wind blows in from the grey sea. A few small ships are out fishing, just shy of the horizon. A large ferry is docked at the end of the pier.");
	Harbor.buildings = { "Curio Shop", "Rune Shop" };
	Harbor.connections = {
		Connection("south", "Merchant's Lane"),
		Connection("north", "Island Ferry")
	};

	Location Churchyard("Churchyard", "A stony, two-story church dominates this side of town. It's dead quiet here. Beyond the church is a graveyard. It seems like new graves are always being dug.");
	Churchyard.buildings = { "Church", "Library" };
	Churchyard.connections = {
		Connection("west", "Town Square"),
		Connection("north", "Graveyard"),
		Connection("east", "Haunted Crypts")
	};

	Location Square("Town Square", "A sprawling, largely empty plaza. There used to be festivals here, but these days they're little more than a distant memory. A few tiny insects flit about the open air, and the smell of mud rises from the southern swamp.");
	Square.buildings = { "Apothecary", "Smithy", "Bank" };
	Square.connections = {
		Connection("north", "Merchant's Lane"),
		Connection("east", "Churchyard"),
		Connection("south", "Acrid Swamp")
	};

	Location Graveyard("Graveyard", "The graveyard is a vast field, long fallen into disrepair. The graves of ancient warriors are now little more than chunks of weathered stone. Lately, burying the dead alone has been more than the town can manage.");
	Graveyard.connections = {
		Connection("south", "Churchyard"),
	};

	Location Ferry("Island Ferry", "The ferry is a flat wooden ship creaking as it rocks gently with the waves. On the northern horizon, veiled in atmospheric blue, one can just barely see a forested island.");
	Ferry.connections = {
		Connection("north", "Stony Island"),
		Connection("south", "Harbor"),
	};

	Location Crypts("Haunted Crypts", "Beyond the barrows and beneath the earth. . . a sprawling labrynth of ancient stone tunnels. For both living and dead, no peace is to be found in this accursed place.");
	Crypts.dungeon = true;
	Crypts.connections = {
		Connection("west", "Churchyard")
	};

	Location Woods("Wilted Woods", "An old network of roads winds through this forest, overgrown and travelled only by the most desperate. No birds dare sing here, there is naught but the creaking of the dead trees.");
	Woods.dungeon = true;
	Woods.connections = {
		Connection("east", "Merchant's Lane")
	};

	Location Swamp("Acrid Swamp", "Muddy water rises to your knees, pond scum spread across its surface hiding that which lurks below. A thick damp mist hangs in the air, obscuring the trunks and vines at the edge of your perception into frightful figures.");
	Swamp.dungeon = true;
	Swamp.connections = {
		Connection("north", "Town Square")
	};

	Location Island("Stony Island", "The island has been abandoned for some time now, though some mossy cabins still stand in the forest. No fisherman would dare sail over the vibrant reef of the north shore, for terrible creatures dwell there. . .");
	Island.dungeon = true;
	Island.connections = {
		Connection("south", "Island Ferry")
	};

	LOCATIONS = { Tavern, Tailor, Barber, Curio, Runes, Church, Library, Apothecary, Smithy, Bank, Merchants, Harbor, Churchyard, Square, Graveyard, Ferry, Crypts, Woods, Swamp, Island };

	std::cout << "Beginning Check" << std::endl;

	for (Location loc : LOCATIONS) {
		getLocation(loc.id);
		for (Connection con : loc.connections) {
			getLocation(con.location);
		}
		for (std::string id : loc.buildings) {
			getLocation(id);
		}
	}
}	

void initEnemies() {
	//Character Enemy(std::string name, int hp, std::vector<int> armor, int difficulty, std::vector<int> zones, int moves, std::string type, std::string desc)
	Character wolf("Crazed Wolf", 40, { 2, 0 }, 40, { 2 }, 3, "animal", "A mangy wolf, foaming at its mouth and snapping viciously.");
	ENEMIES.push_back(wolf);
}