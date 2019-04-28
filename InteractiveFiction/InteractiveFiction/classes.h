#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

//enumerators, these are the things the player will be using through out the game to interact
enum en_dirs { NORTH, EAST, SOUTH, WEST };
enum en_rooms { MAIN, BATHROOM, CLOSET, EXIT };
enum en_verbs { TAKE, DROP, USE, OPEN, CLOSE, EXAMINE, INVENTORY, LOOK, HELP };
enum en_nouns { BATHROOM_DOOR, CLOSET_DOOR, DESK, BED, KEY, NOTE };

//constant values of the number of items in teh enums
const int NONE = -1; //used to tell if there is no exit or value
const int DIRS = 4; //the number of directions
const int ROOMS = 4; //number of rooms
const int VERBS = 9; //number of verbs
const int NOUNS = 6; //number of nouns


class words
{
public:
	string word;
	int code;
};

class room
{
public:
	string description;
	int exits_to_room[DIRS];
};

class noun
{
public:
	string word;
	string description;
	int code;
	int location;
	bool can_carry;
};

