#pragma once

void initLocations() {
	std::vector<Location> locations = {};
	// Buildings
	Location Tavern("Tavern", "The *BLUE*Tavern*GREY* is four stories, and boasts no fewer than twenty-five rooms for rent, though these days only a few are ever rented out at a time. More than a few townspeople come to the *RED*firelit*GREY* common room for dinner or drinks at its bar, which is enough for *YELLOW*Penelope*GREY* to maintain the establishment.\n\nIt's said that *GREEN*Asha*GREY*, sitting perpetually at the bar and living in a rented room accounts for half of the tavern's total revenue.");
	Tavern.parent = "Merchant's Lane";
	Tavern.people = { "Penelope", "Asha", "Clyde" };

	Location Tailor("Tailor", "The *BLUE*Tailor Shop*GREY* is well stocked with all the latest gowns and garments, intricately crafted by *YELLOW*Florence*GREY* herself. Painted a dark brown in its interior to accent the colorful fabrics within, it's one of the newest additions to *RED*Merchant's Lane*GREY*. Behind several fashionable displays, hidden in a back corner, is a mannequin adorned in old *CYAN*highland chainmail*GREY*. When asked about it, *YELLOW*Florence*GREY* only looks away with a sly, fading laugh.");
	Tailor.parent = "Merchant's Lane";
	Tailor.people = { "Florence" };

	Location Barber("Barber", "The *BLUE*Barbershop*GREY* is a single, plainly furnished room at the front of *GREEN*Nestra*GREY*'s estate, in what used to be an antechamber. The shop's greatest feature is its window to the estate's atrium garden with its exquisite *WHITE*fountain*GREY*, *WHITE*statues*GREY*, and *WHITE*pillars*GREY*.\n\nHaircuts are *YELLOW*15 Gold*GREY*.\n\nType *CYAN*haircut *GREY*[*ORANGE*description*GREY*] to give yourself a description. You can use *CYAN*parse *GREY*[*ORANGE*description*GREY*] to preview your haircut.");
	Barber.parent = "Merchant's Lane";
	Barber.people = { "Nestra" };

	Location Curio("Curio Shop", "The *BLUE*Curio Shop*GREY* is a maze of narrow hallways and tiny rooms, formed by hanging carpets and shelves that rise from the floor to the low ceiling. Every imaginable sort of thing can be found cluttered onto a shelf somewhere in the shop, from ancient artifacts to tarnished copper toys. *YELLOW*Minsiki*GREY* scours every merchant ship that pulls into the harbor, looking for more *PINK*strange wares*GREY*.");
	Curio.parent = "Harbor";
	Curio.people = { "Minsiki" };

	Location Runes("Rune Shop", "The *BLUE*Rune Shop*GREY* is a dusty wooden building, jointly owned by Sarkana and Terat in spite of their animosity. They've hung a sheet from the ceiling to divide the space behind the counter in half. *GREEN*Sarkana*GREY* and *GREEN*Terat*GREY* watch eagerly to see whose runes you intend to buy.");
	Runes.parent = "Harbor";
	Runes.people = { "Sarkana", "Terat" };

	Location Church("Church", "The *BLUE*Church*GREY* is an ancient building, and by far the nicest in town. Its walls are built of an odd assortment of mismatched tan and grey stones, all carefully and sturdily laid together by long-dead craftsmen. The windows are clear *CYAN*quartz*GREY*, and the pews are fine mahogany, though scarcely do they see use these days. *GREEN*Father Tobin*GREY* tends to the grounds by himself, praying he might one day find a successor.");
	Church.parent = "Churchyard";
	Church.people = { "Tobin" };

	Location Library("Library", "The *BLUE*Library*GREY* is an old building, discreetly tucked into a corner of the *YELLOW*plaza*GREY*. The books are all held in a single spacious room, partitioned by a *CYAN*labyrinth*GREY* of tall bookshelves, some of which are subdivided by a wooden platform that acts as a partial second floor. The air is stale and dusty, and if you wander past the *GREEN*books*GREY* you'll come upon a bedroom and office where *PINK*Elias*GREY* lives and works.");
	Library.parent = "Churchyard";
	Library.people = { "Elias" };

	Location Bank("Bank", "The *BLUE*Bank*GREY* is a low, unassuming building of *BLACK*old grey stone*GREY*. It has a single circular window on its front face, but is otherwise lit by *YELLOW*dim lamps*GREY*. Despite its modest appearance the building has been reinforced by *PINK*ancient runes*GREY* that completely protect any and all *CYAN*deposits*GREY*.");
	Bank.parent = "Town Square";

	Location Apothecary("Apothecary", "The *BLUE*Apothecary*GREY* is little more than a tiny room and a counter, even smaller than *YELLOW*Minsiki's*GREY*'s *BLUE*curio shop*GREY*. There is a door behind the counter which none but *YELLOW*Qarana*GREY* have seen the other side of. On each wall are shelves of glass potion bottles that rattle as the shop door opens and closes. Within the shop a slight metallic, almost *RED*bloody*GREY* smell lingers, mixing with a *GREEN*sour*GREY*, smoky scent. If ever it's mentioned, *YELLOW*Qarana*GREY* insists it's coming from the *YELLOW*Kobos*GREY*'s stall next door.");
	Apothecary.parent = "Town Square";
	Apothecary.people = { "Qarana" };

	Location Smithy("Smithy", "The *BLUE*Smithy*GREY* is more of a pavillion rather than a shop, always shimmering in the *RED*fiery*GREY* radiance of the ingot furnace. Most of *YELLOW*Kobos*GREY*'s work consists of mundane orders for the town, nails, fish hooks, anchors, and the like, though many adventurers seek him out for his legendary weapons and armor. Many of his finest masterpieces hang behind his counter. *YELLOW*Kobos*GREY* doesn't worry about theft, though - he knows no one would dare to *RED*steal*GREY* from him again.");
	Smithy.parent = "Town Square";
	Smithy.people = { "Kobos" };

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
	Graveyard.people = { "Janice" };

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

	locations = { Tavern, Tailor, Barber, Curio, Runes, Church, Library, Apothecary, Smithy, Bank, Merchants, Harbor, Churchyard, Square, Graveyard, Ferry, Crypts, Woods, Swamp, Island };

	for (Location loc : locations) {
		LOCATIONS[low(loc.id)] = loc;
	}

	std::cout << "Beginning Check" << std::endl;

	for (auto loc : LOCATIONS) {
		getLocation(loc.second.id);
		for (Connection con : loc.second.connections) {
			getLocation(con.location);
		}
		for (std::string id : loc.second.buildings) {
			getLocation(id);
		}
	}
}	

void initEnemies() {
	std::vector<Character> enemies = {};
	//Character Enemy(std::string name, int hp, std::vector<int> armor, int difficulty, std::vector<int> zones, int AP, std::string type, std::string desc)
	Character wolf("Crazed Wolf", 40, { 2, 0 }, 40, { 2 }, 3, "animal", "A mangy wolf, foaming at its mouth and snapping viciously.");
	enemies.push_back(wolf);

	for (Character enemy : enemies) {
		ENEMIES[low(enemy.ID)] = enemy;
	}
}

UI_Item OneH(std::string n, int v, WEAPON_CLASS wepType, std::string d, int atks, int chance, int min, int max, int pen, int ap, int rng, int rare = 0) {
	return UI_Item(n, v, wepType, d, false, atks, chance, min, max, pen, ap, rng, rare);
}

UI_Item TwoH(std::string n, int v, WEAPON_CLASS wepType, std::string d, int atks, int chance, int min, int max, int pen, int ap, int rng, int rare = 0) {
	return UI_Item(n, v, wepType, d, true, atks, chance, min, max, pen, ap, rng, rare);
}

void initItems() {	//	 Name		G		Type		Description												#	%		Min		Max		Pen		AP		Range
	ITEMS["dagger"] = OneH("Dagger", 25, BLADE, "A simple dagger, lightweight and easy to use.", 3, 70, 2, 4, 0, 1, 1);
	ITEMS["longbow"] = TwoH("Longbow", 50, RANGED, "A powerful bow thats almost as tall as its user.", 1, 80, 8, 14, 20, 6, 6);
}

void initPeople() {
	std::vector<NPC> people = {};

	NPC Merchant("Wandering Merchant", true, "A *YELLOW*wandering merchant*GREY* who travels around the *RED*wilderness*GREY*, selling *CYAN*peculiar*GREY* items to any adventurers who happen upon them. They're tall, a being formed of *BLACK*shadow*GREY*, adourned in *PINK*dapper attire*GREY*. They have very little to say, but their intentions are clear.");

	NPC Minsiki("Minsiki", true, "*GREEN*Minsiki*GREY* is middle-aged, with a thick black beard and kind eyes. No one knows where Minsiki came from before he came to the village years ago and opened his shop; they still think of him as an exotic stranger. Minsiki prefers it that way, he likes to keep to himself.");
	NPC Gout("Gout", true, "*GREEN*Gout*GREY* is an older man, short and grey but broad all the same. He hobbles across the pier and his muscles flex beneath his wrinkled skin as he casts his fishing line out into the sea. His is a soldier's body, its scars etched immutably as if in marble. It has not forgotten the many campaigns and battles, and though *GREEN*Gout*GREY* never speaks of his past, his eyes would say he has not forgotten either.");
	NPC Tobin("Tobin", true, "*GREEN*Father Tobin*GREY* is a thin man, taller than most and pale. From time to time one can see sadness in his eyes, as he looks across the graves he tends, but it is only ever there for a moment and then he smiles. In his humility he has found unwavering joy.");
	NPC Kobos("Kobos", true, "*GREEN*Kobos*GREY* has been swinging hammer for forty years now, nearly as long as he's been alive. When he works his craft the singing of his anvil can be heard as far away as the harbor. Princes have offered to buy out his services, but he has no interest in their gold, only in his iron.");
	NPC Clyde("Clyde", true, "*GREEN*Clyde*GREY* is a *TEAL*huntsman*GREY*. He spends more time far afield than anyone else in the town. When some people hear his frightful tails they roll their eyes; sometimes he sounds like a *ORANGE*coward*GREY*. He pleads with anyone who will listen to avoid the woods at night.");
	NPC Terat("Terat", true, "*GREEN*Terat*GREY* is the estranged brother of *GREEN*Sarkana*GREY*. He's quite timid, and perhaps a bit anxious. He can be rather friendly and generous, but for the most part focuses on his work. He always seems to be engraving new golden runes into their cold steel plates.");
	NPC Elias("Elias", true, "*GREEN*Elias*GREY* is a young man, slender, with a mess of dusty brown hair and grey-blue eyes that gleam from behind a pair of bronze-rimmed quartz spectacles. He spends most of his time reading by lamplight, or tending to the many old tomes in his care. He tends to only leave the library at night, rendering him somewhat of a mystery among the less scholarly townsfolk.");

	NPC Qarana("Qarana", true, "Some say *GREEN*Qarana*GREY* was a swamp witch once, who grew bored of her solitary lifestyle. When she hears such rumors, Qarana only smiles. She has a thousand stories to tell of her travels, though the stories are never quite the same between tellings.");
	NPC Asha("Asha", false, "*GREEN*Asha*GREY* is young, with dark hair and dark eyes. She lives at the tavern and never works, she only spends her days drinking, often alone. It's whispered that she was a promising mage once, and an aristocrat, but no one knows much about her.");
	NPC Penelope("Penelope", true, "*GREEN*Penelope*GREY* has been running the tavern for years now. Business is slow these days, with travel being as dangerous as it is. She insists that she's doing well, but behind her warm eyes is an ever-present weariness.");
	NPC Nestra("Nestra", false, "*GREEN*Nestra*GREY* married an old merchant who spends all his time at sea. She took some of the money he sends back to her and opened her *BLUE*Barbershop*GREY*. She's not that good at cutting hair, but she tries her best, and she is getting better.");
	NPC Janice("Janice", false, "*GREEN*Janice*GREY* has lived in the town for nearly all of her long life, save for a few foreign adventures that she still remembers fondly. She seems like a simple old woman, preferring to keep quiet on most matters, but from time to time it becomes apparent that she has seen more than most people can even imagine.");
	NPC Florence("Florence", true, "*GREEN*Florence*GREY* is grey-haired, yet her stern face, criss-crossed with scars, maintains a youthful glow. She insists she's but a simple tailor, but she never goes out without a dagger in her belt, and she seems to know how to cut more than just cloth.");
	NPC Sarkana("Sarkana", true, "*GREEN*Sarkana*GREY* is the estranged sister of Terat. When the day is done she packs away her runes and travels far to the east. She has always been a bit spiteful, and prefers not to talk to anyone, except to speak ill of her brother.");

	Elias.CONVERSATIONS.push_back("Elias*GREY*: Salutations.\n");
	Elias.CONVERSATIONS.push_back("Elias*GREY*: Does anything here interest you?\n");
	Elias.CONVERSATIONS.push_back("Elias*GREY*: The other day, I was engrossed in a most interesting tome . . .\n");
	Elias.CONVERSATIONS.push_back("Elias wipes his glasses with a square cloth.\n");

	Gout.CONVERSATIONS.push_back("Gout*GREY*: I'd say good day but if it was a good day I'd be fishing Ha!");
	Gout.CONVERSATIONS.push_back("Gout*GREY*: What can I do for you?");
	Gout.CONVERSATIONS.push_back("Gout*GREY*: It's a great day for fishing.");
	Gout.CONVERSATIONS.push_back("Gout*GREY*: I'm thinking of getting out on the water myself.");
	Gout.CONVERSATIONS.push_back("Gout*GREY*: I went fishing in the Acrid Swamp once, hooked a huge feller what looked like a slug.");
	Gout.CONVERSATIONS.push_back("Gout*GREY*: I mostly fish off the pier these days, what with the monsters out around the island.");
	Gout.CONVERSATIONS.push_back("Gout slaps his gut.");
	Gout.CONVERSATIONS.push_back("Gout*GREY*: They say Asha is a mage, but the only thing I've seen her cast is a line. Ha!");
	Gout.CONVERSATIONS.push_back("Gout chuckles.");
	Gout.CONVERSATIONS.push_back("Gout*GREY*: It's dangerous out here on the island but the fishing is well worth it.");

	Nestra.CONVERSATIONS.push_back("Nestra*GREY*: Hello.");
	Nestra.CONVERSATIONS.push_back("Nestra*GREY*: Welcome to my barbershop.");
	Nestra.CONVERSATIONS.push_back("Nestra*GREY*: You know, it'd be nice to have more refined visitors to our town.");
	Nestra.CONVERSATIONS.push_back("Nestra*GREY*: Qarana, Minsiki, those rune sellers. Our town has become rather . . . cosmopolitan.");
	Nestra.CONVERSATIONS.push_back("Nestra*GREY*: You certainly look like you could use a haircut.");

	Janice.CONVERSATIONS.push_back("Janice*GREY*: Hello dear.");
	Janice.CONVERSATIONS.push_back("Janice*GREY*: Are you here to visit someone?.");
	Janice.CONVERSATIONS.push_back("Janice*GREY*: I bring flowers to the unknown graves.");
	Janice.CONVERSATIONS.push_back("Janice*GREY*: I hope their souls are all at peace.");
	Janice.CONVERSATIONS.push_back("Janice places a single white flower onto the surface of a fresh grave.");

	Asha.CONVERSATIONS.push_back("Asha doesn't seem to notice you. . .");
	Asha.CONVERSATIONS.push_back("Asha looks at you blankly. . .");
	Asha.CONVERSATIONS.push_back("Asha*GREY*: Did you know Goblins have green blood?");
	Asha.CONVERSATIONS.push_back("Asha*GREY*: Once Minsiki came in here, and he was drinking with me right? Anyways he gets really drunk, and he starts laughing like crazy, and he starts telling me about this time he cut some guy's ear off with a scimitar. It was insane.");
	Asha.CONVERSATIONS.push_back("Asha*GREY*: Penelope is nice, and pretty too. Free drinks and a free room, no one else does anything like that for me.");
	Asha.CONVERSATIONS.push_back("Asha*GREY*: I found a rusty sword once, and chucked it into the sea. That's a stupid story.");
	Asha.CONVERSATIONS.push_back("Asha*GREY*: I went to the church once, expecting a service. What a headache I had . . . Anyways, no one was there; Father Tobin looked so beaten down.");
	Asha.CONVERSATIONS.push_back("Asha*GREY*: Terat came in here the other day, sat over there in the corner. Way back there alone. After about an hour and a half I felt bad, and went to talk to him. He just mumbled something and left. What an ass.");
	Asha.CONVERSATIONS.push_back("Asha*GREY*: I've got nothing to say about Sarkana, except that I can't stand her.");
	Asha.CONVERSATIONS.push_back("Asha*GREY*: Have you ever met Clyde? He's alright. He's like a little puppy sometimes.");
	Asha.CONVERSATIONS.push_back("Asha*GREY*: Qarana is so full of herself. I guess in a place like this that makes sense. She could do so much more than brew potions, though.");
	Asha.CONVERSATIONS.push_back("Asha*GREY*: I hate spiders. Big ones and small ones. Bugs in general. I used to love honey, now I even hate bees. Mead is alright.");
	Asha.CONVERSATIONS.push_back("Asha*GREY*: Do you like my shirt? I bought it from Florence. I think it's quite stylish.");
	Asha.CONVERSATIONS.push_back("Asha*GREY*: I should have been an enchanter. You can make a lot of money from runes, and you don't have to get ripped apart doing it.");
	Asha.CONVERSATIONS.push_back("Asha*GREY*: This drink tastes like ass water. Like sweat or something, it's gross.");
	Asha.CONVERSATIONS.push_back("Asha*GREY*: The best sight in all of the village can be seen from right here at the bar. Er, my drink I mean. . .");

	Minsiki.CONVERSATIONS.push_back("Minsiki mumbles something.");
	Minsiki.CONVERSATIONS.push_back("Minsiki mumbles something that sounds like a joke.");
	Minsiki.CONVERSATIONS.push_back("Minsiki*GREY*: Good wares eh?");
	Minsiki.CONVERSATIONS.push_back("Minsiki*GREY*: Hm?");
	Minsiki.CONVERSATIONS.push_back("Minsiki wipes some blood off the scimitar on display, then clears his throat.");
	Minsiki.CONVERSATIONS.push_back("Minsiki*GREY*: This came from a long way away, just like me, huh? *GREY*He laughs.");

	Terat.CONVERSATIONS.push_back("Terat*GREY*: If you're expecting a fight, you can't go wrong with some protective runes.");
	Terat.CONVERSATIONS.push_back("Terat*GREY*: I make all of my runes myself.");
	Terat.CONVERSATIONS.push_back("Terat*GREY*: You won't find better runes than these anywhere in town.");
	Terat.CONVERSATIONS.push_back("Terat*GREY*: Hm?");
	Terat.CONVERSATIONS.push_back("Terat*GREY*: Hmm. . .");

	Sarkana.CONVERSATIONS.push_back("Sarkana*GREY*: Yes?");
	Sarkana.CONVERSATIONS.push_back("Sarkana*GREY*: What?");
	Sarkana.CONVERSATIONS.push_back("Sarkana*GREY*: Are you going to buy a rune?");
	Sarkana.CONVERSATIONS.push_back("Sarkana*GREY*: Don't bother with that tripe my brother sells. It's a waste of your money.");
	Sarkana.CONVERSATIONS.push_back("Sarkana*GREY*: I don't know why I stay here.");
	Sarkana.CONVERSATIONS.push_back("Sarkana*GREY*: The sky is always an ugly grey around here.");

	Penelope.CONVERSATIONS.push_back("Penelope seems busy, washing the bar. . .");
	Penelope.CONVERSATIONS.push_back("Penelope*GREY*: Would you like a drink?");
	Penelope.CONVERSATIONS.push_back("Penelope*GREY*: Asha came in here over a year ago, and handed me a chunk of gold for a room and some drink. Solid gold. I don't think she remembers doing it.");
	Penelope.CONVERSATIONS.push_back("Penelope*GREY*: I know I charge a lot for rooms, but I'd go out of business if I didn't.");
	Penelope.CONVERSATIONS.push_back("Penelope*GREY*: Rooms are 5 gold.");
	Penelope.CONVERSATIONS.push_back("Penelope*GREY*: All kinds of strange folks come here. Brigands, Witches. Honestly, so long as they don't cause trouble I let them stay.");
	Penelope.CONVERSATIONS.push_back("Penelope*GREY*: When I first took over the tavern, the road was still bad, but not so bad as this. We used to get more ships in too.");
	Penelope.CONVERSATIONS.push_back("Penelope*GREY*: I brew most of this alcohol myself. Most of the time it turns out really well actually. Sometimes not so much.");
	Penelope.CONVERSATIONS.push_back("Penelope*GREY*: I know the road's pretty bad these days, but I think Clyde exaggerates. Plenty of people go out and come back again. Like him, for example.");

	Qarana.CONVERSATIONS.push_back("Qarana*GREY*: Have you been to the swamp? It's beautiful isn't it?");
	Qarana.CONVERSATIONS.push_back("Qarana*GREY*: One time I saw a slug, a vast thing. It was moving slowly through the mist, algae had grown across it, even a few trees had risen up on its mighty back.");
	Qarana.CONVERSATIONS.push_back("Qarana*GREY*: Would you like to buy a potion?");
	Qarana.CONVERSATIONS.push_back("Qarana*GREY*: I put a lot of effort into the flavor of these things, you know. I guarantee they'll go down easy.");
	Qarana.CONVERSATIONS.push_back("Qarana*GREY*: You know one of these things could save your life.");
	Qarana.CONVERSATIONS.push_back("Qarana*GREY*: How can I help you today?");

	Kobos.CONVERSATIONS.push_back("Kobos*GREY*: Good day.");
	Kobos.CONVERSATIONS.push_back("Kobos*GREY*: What are you looking to buy?");
	Kobos.CONVERSATIONS.push_back("Kobos*GREY*: Weapons or Armor - you can find the highest quality here.");
	Kobos.CONVERSATIONS.push_back("Kobos*GREY*: I used to be a fighter, you know. I split a Lich's skull with that War Hammer behind the counter, and watched as its soul spilled away.");
	Kobos.CONVERSATIONS.push_back("Kobos*GREY*: I crafted a helmet for some Duke once. He came back to me complaining that his neck was sore after an Ogre had struck him in the back of the head. The fool.");
	Kobos.CONVERSATIONS.push_back("Kobos*GREY*: If you're looking for ranged weapons, try talking to Clyde. For runes, you can talk to Sarkana and Terat. They're quite good at their respective crafts.");
	Kobos.CONVERSATIONS.push_back("Kobos*GREY*: I lived my whole life here; there's a real beauty here, and I'd sooner die than give it up.");
	Kobos.CONVERSATIONS.push_back("Kobos*GREY*: Father Tobin keeps a fine collection of tomes, but I think the graves and the crypts scare people away from the church. It's a shame.");

	Florence.CONVERSATIONS.push_back("Florence*GREY*: Hello dear!");
	Florence.CONVERSATIONS.push_back("Florence*GREY*: What are you looking to buy? Something for pleasure, or for business, so to speak?");
	Florence.CONVERSATIONS.push_back("Florence*GREY*: That's real Giant Spider silk. Strong and beautiful.");
	Florence.CONVERSATIONS.push_back("Florence*GREY*: I don't know how much you get out of town, but have you seen what Goblins wear? It's disgraceful.");
	Florence.CONVERSATIONS.push_back("Florence*GREY*: You can always tell how long someone's been in town, just by how ugly their clothing is.");
	Florence.CONVERSATIONS.push_back("Florence*GREY*: We're a very well-dressed town, thanks to me.");
	Florence.CONVERSATIONS.push_back("Florence*GREY*: I really would recommend you buy something, from myself or Kobos. You can never be too safe.");

	Clyde.CONVERSATIONS.push_back("Clyde*GREY*: Hello.");
	Clyde.CONVERSATIONS.push_back("Clyde*GREY*: Be careful if you head out; I beg of you.");
	Clyde.CONVERSATIONS.push_back("Clyde*GREY*: The Woods are bad enough, but I would avoid the Swamp. The Crypts? Hm . . . *GREEN*He shudders.");
	Clyde.CONVERSATIONS.push_back("Clyde*GREY*: I was lost. Evening turned to night and suddenly I was surrouned by eyes glowing green in my torchlight. Chitinous legs were clicking in the darkness. I thought I was doomed. And then, I swear, they burst into a mist of ichor, and I was alone.");
	Clyde.CONVERSATIONS.push_back("Clyde*GREY*: Most people think goblins are pretty dumb, but I'd disagree. I saw a contingent of knights get ambushed and torn to shreds by a group of them; it was a real clever, wretched thing.");
	Clyde.CONVERSATIONS.push_back("Clyde*GREY*: It really would be better if you were to just stay in town. I'm too old to change my ways, but it's not too late for you.");

	Tobin.CONVERSATIONS.push_back("Tobin*GREY*: Hello.");
	Tobin.CONVERSATIONS.push_back("Tobin*GREY*: Blessings, my Child.");
	Tobin.CONVERSATIONS.push_back("Tobin*GREY*: I pray the Gods will deliver us from this evil.");
	Tobin.CONVERSATIONS.push_back("Tobin*GREY*: Hmm. . . I feel we will meet again soon.");
	Tobin.CONVERSATIONS.push_back("Tobin*GREY*: It's bitter work digging so many graves, but I've grown use to it.");

	Merchant.CONVERSATIONS.push_back("*GREY*A glowing smile shines from beneath the brim of the merchant's cap.");
	Merchant.CONVERSATIONS.push_back("*GREY*The merchant hisses.");
	Merchant.CONVERSATIONS.push_back("*BLACK*. . .");
	Merchant.CONVERSATIONS.push_back("*GREY*The merchant makes a clicking sound.");

	// Items
	Clyde.ITEMS = {
		"dagger",
		"longbow"
	};

	//M		F		M		 M		M		M		F		F		F		F		F		F	     M		NB		   M
	people = { Gout, Nestra, Minsiki, Tobin, Kobos, Clyde, Qarana, Asha, Penelope, Florence, Janice, Sarkana, Terat, Merchant, Elias };

	for (NPC npc : people) {
		PEOPLE[low(npc.NAME)] = npc;
	}
}

void init() {
	initLocations();
	initItems();
	initEnemies();
	initPeople();
}