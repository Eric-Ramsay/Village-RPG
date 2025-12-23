#pragma once

std::vector<Message> messageBuffer = {};
bool SENDING = false;
Message* bufferStart = nullptr;
Message* bufferEnd = nullptr;
Message* firstProcess = nullptr;

bool connected = true;
std::string IP = "10.0.0.12";
sockaddr_in hint;
int port = 1234;
SOCKET sock;

std::unordered_map<std::string, UI_Item> ITEMS = {};
std::unordered_map<std::string, NPC> PEOPLE = {};
std::unordered_map<std::string, Location> LOCATIONS = {};
std::unordered_map<std::string, Character> ENEMIES = {};
std::unordered_map<std::string, Battle> BATTLES = {};

std::deque<Animation> animations = {};

UIState UI;

std::vector<sf::Vertex> vertices;
int numVertices = 0;
int vertSize = 200000;

//Battle BATTLE;

int WIDTH = 640;
int HEIGHT = 360;

std::string input = "";
int holdingBackspace = 0;

std::vector<std::string> logs = {"*BLACK*Connecting . . ."};

std::string ID = "";
std::unordered_map<std::string, Character> CHARACTERS = {};
std::unordered_map<std::string, Character> GRAVEYARD = {};

std::vector<Terrain> TERRAIN = {};
std::unordered_map<std::string, UI_Effect> EFFECTS = {};


std::vector<Character> GRAVES = {};
std::vector<Character> sortedGraves = {};

std::string USERNAME = "eric";
std::string PASSWORD = "123";
bool rememberAccount = true;

bool blur = false;
bool scanLines = false;
bool quit = false;

std::vector<std::vector<Drop>> zoneLoot = { {}, {}, {}, {} };
std::vector<UI_Item> genericLoot = {};

std::vector<Tab*> tabList = {};
Tab playerMenu({ "INV", "EFFECTS", "MAGIC", "DESC", "TRAITS"});
Tab combatMenu({ "BATTLE", "LOOT" });
Tab viewMenu({ "DESCRIPTION", "RUNES" });

std::unordered_map<std::string, Tooltip> TOOLTIPS = {};

std::unordered_map<std::string, sf::Color> COLORS = {};

bool updateMovement = true;


std::vector<Scrollbar*> scrollbars = {};
Scrollbar tradeBar(10, 100);
Scrollbar logBar(7, 97, true);
Scrollbar graveBar(8, 240);
Scrollbar lootBar(17, 170);

int numMessages = 0;