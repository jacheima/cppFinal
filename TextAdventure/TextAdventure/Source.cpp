#include "iostream"
#include "string"
#include "vector" // For the command handling function.
#include "cctype" // Will be used to eliminate case sensitivity problems.

using namespace std;

enum en_dirs {FORWARD, LEFT, RIGHT, BACK, ABOVE, BELOW};
enum en_rooms {SHELF, FLOOR, TOOLBOX, WATCH, SECRET_ROOM, SMALL_BOX, PHONE, RIGHT_WALL, LEFT_WALL, CEILING, EXIT, DOOR, RING};
enum en_verbs {TAKE, DROP, USE, OPEN, CLOSE, EXAMINE, INVENTORY, LOOK};
enum en_nouns {HANGER, OVER_COAT, BOX, POCKET_WATCH, PHOTO, TOOL_BOX, WIRE_CUTTERS, CROWBAR, GOLD_KEY, BLAZER, PDA, LETTER, SILVER_KEY, LOCKED_BOX, GEM, NOTE_ONE, NOTE_TWO, TRAP_DOOR, MAIN_DOOR, THE_RING};

const int NONE = -1;
const int DIRS = 6;
const int ROOMS = 13;
const int VERBS = 8;
const int NOUNS = 20;

struct words
{
	string word;
	int code;
};

struct room
{
	string description;
	int exits_to_room[DIRS];
};

struct noun
{
	string word;
	string description;
	int code;
	int location;
	bool can_carry;
};

void section_command(string cmd, string &wd1, string &wd2);
void set_rooms(room *rms);
void set_directions(words *dir);
bool parser(int &loc, string wd1, string wd2, words *dir, words *vbs, room *rms, noun *nns);
void set_verbs(words *vbs);
void look_around(int loc, room *rms, words *dir, noun *nns);
void set_nouns(noun *nns);

int main()
{
	int location = DOOR; //set the location of the player using the enumerated type identifier

	string command;
	string word_1;
	string word_2;

	room rooms[ROOMS];
	set_rooms(rooms);

	words directions[DIRS];
	set_directions(directions);

	words verbs[VERBS];
	set_verbs(verbs);

	noun nouns[NOUNS];
	set_nouns(nouns);

	while (word_1 != "QUIT")
		// I have provided an escape condition from the loop here
	{
		command.clear();
		cout << "What would you like to do?" << endl;
		getline(cin, command);
		cout << "Your raw command was " << command << endl;
		word_1.clear();
		word_2.clear();
		// Call the function that handles the command line format.
		section_command(command, word_1, word_2);
		//call the parser function
		parser(location, word_1, word_2, directions, verbs, rooms, nouns);
		// For test purposes, output the command after formatting by the function.
		cout << word_1 << " " << word_2 << endl;

	}
	return 0;
}

void section_command(string cmd, string &wd1, string &wd2)
{
	string sub_str;
	vector<string> words;
	char search = ' ';
	size_t i, j;
	// Split Command into vector
	for (i = 0; i < cmd.size(); i++)
	{
		if (cmd.at(i) != search)
		{
			sub_str.insert(sub_str.end(), cmd.at(i));
		}
		if (i == cmd.size() - 1)
		{
			words.push_back(sub_str);
			sub_str.clear();
		}
		if (cmd.at(i) == search)
		{
			words.push_back(sub_str);
			sub_str.clear();
		}
	}
	// Clear out any blanks
	// I work backwords through the vectors here as a cheat not to invalidate the iterator
	for (i = words.size() - 1; i > 0; i--)
	{
		if (words.at(i) == "")
		{
			words.erase(words.begin() + i);
		}
	}
	// Make words upper case
	// Right here is where the functions from cctype are used
	for (i = 0; i < words.size(); i++)
	{
		for (j = 0; j < words.at(i).size(); j++)
		{
			if (islower(words.at(i).at(j)))
			{
				words.at(i).at(j) = toupper(words.at(i).at(j));
			}
		}
	}
	// Very simple. For the moment I only want the first to words at most (verb / noun).
	if (words.empty())
	{
		cout << "No command given" << endl;
	}
	if (words.size() == 1)
	{
		wd1 = words.at(0);
	}
	if (words.size() == 2)
	{
		wd1 = words.at(0);
		wd2 = words.at(1);
	}
	if (words.size() > 2)
	{
		cout << "Command too long. Only type one or two words (direction or verb and noun)" << endl;
	}
}

void set_rooms(room *rms)
{
	rms[DOOR].description.assign("a door");
	rms[DOOR].exits_to_room[FORWARD] = EXIT; //this will be locked
	rms[DOOR].exits_to_room[BACK] = NONE;
	rms[DOOR].exits_to_room[LEFT] = LEFT_WALL;
	rms[DOOR].exits_to_room[RIGHT] = RIGHT_WALL;
	rms[DOOR].exits_to_room[ABOVE] = CEILING;
	rms[DOOR].exits_to_room[BELOW] = FLOOR;

	rms[RIGHT_WALL].description.assign("a wall with a sticky note attached");
	rms[RIGHT_WALL].exits_to_room[FORWARD] = NONE;
	rms[RIGHT_WALL].exits_to_room[BACK] = NONE;
	rms[RIGHT_WALL].exits_to_room[RIGHT] = SHELF;
	rms[RIGHT_WALL].exits_to_room[LEFT] = DOOR;
	rms[RIGHT_WALL].exits_to_room[ABOVE] = CEILING;
	rms[RIGHT_WALL].exits_to_room[BELOW] = FLOOR;

	rms[LEFT_WALL].description.assign("a wall");
	rms[LEFT_WALL].exits_to_room[FORWARD] = NONE;
	rms[LEFT_WALL].exits_to_room[BACK] = NONE;
	rms[LEFT_WALL].exits_to_room[LEFT] = SHELF;
	rms[LEFT_WALL].exits_to_room[RIGHT] = DOOR;
	rms[LEFT_WALL].exits_to_room[ABOVE] = CEILING;
	rms[LEFT_WALL].exits_to_room[BELOW] = FLOOR;

	rms[SHELF].description.assign("a shelf with a coat hanging off it, an old cardboard box and a pocket watch resting on the shelf.");
	rms[SHELF].exits_to_room[FORWARD] = NONE;
	rms[SHELF].exits_to_room[BACK] = NONE;
	rms[SHELF].exits_to_room[LEFT] = RIGHT_WALL;
	rms[SHELF].exits_to_room[RIGHT] = LEFT_WALL;
	rms[SHELF].exits_to_room[ABOVE] = CEILING;
	rms[SHELF].exits_to_room[BELOW] = FLOOR;

	rms[CEILING].description.assign("a trap door that has been boarded over");
	rms[CEILING].exits_to_room[FORWARD] = SECRET_ROOM;
	rms[CEILING].exits_to_room[BACK] = DOOR;
	rms[CEILING].exits_to_room[LEFT] = NONE;
	rms[CEILING].exits_to_room[RIGHT] = NONE;
	rms[CEILING].exits_to_room[ABOVE] = NONE;
	rms[CEILING].exits_to_room[BELOW] = DOOR;

	rms[FLOOR].description.assign("a tool box and a crumpled piece of paper lay on the floor");
	rms[FLOOR].exits_to_room[FORWARD] = NONE;
	rms[FLOOR].exits_to_room[BACK] = DOOR;
	rms[FLOOR].exits_to_room[LEFT] = NONE;
	rms[FLOOR].exits_to_room[RIGHT] = NONE;
	rms[FLOOR].exits_to_room[ABOVE] = DOOR;
	rms[FLOOR].exits_to_room[BELOW] = NONE;

	rms[TOOLBOX].description.assign("Inside the toolbox you see snips and a small crowbar.");
	rms[TOOLBOX].exits_to_room[ABOVE] = NONE;
	rms[TOOLBOX].exits_to_room[LEFT] = NONE;
	rms[TOOLBOX].exits_to_room[RIGHT] = NONE;
	rms[TOOLBOX].exits_to_room[FORWARD] = NONE;
	rms[TOOLBOX].exits_to_room[BELOW] = NONE;
	rms[TOOLBOX].exits_to_room[BACK] = FLOOR;

	rms[WATCH].description.assign("a pocket watch. The latch is jammed. Maybe I can find a small object to open with.");
	rms[WATCH].exits_to_room[ABOVE] = NONE;
	rms[WATCH].exits_to_room[LEFT] = NONE;
	rms[WATCH].exits_to_room[RIGHT] = NONE;
	rms[WATCH].exits_to_room[FORWARD] = NONE;
	rms[WATCH].exits_to_room[BELOW] = NONE;
	rms[WATCH].exits_to_room[BACK] = SHELF;

	rms[SECRET_ROOM].description.assign("a secret room. You can see a box laying on its side on the floor, a blazer hanging over a rickety chair, and a worn piece of paper lay on the floor near the opening of the trap doo.");
	rms[SECRET_ROOM].exits_to_room[ABOVE] = NONE;
	rms[SECRET_ROOM].exits_to_room[LEFT] = NONE;
	rms[SECRET_ROOM].exits_to_room[RIGHT] = NONE;
	rms[SECRET_ROOM].exits_to_room[FORWARD] = NONE;
	rms[SECRET_ROOM].exits_to_room[BELOW] = CEILING;
	rms[SECRET_ROOM].exits_to_room[BACK] = CEILING;

	rms[SMALL_BOX].description.assign("a small brown box. It's locked, I need a key to open it.");
	rms[SMALL_BOX].exits_to_room[ABOVE] = NONE;
	rms[SMALL_BOX].exits_to_room[LEFT] = NONE;
	rms[SMALL_BOX].exits_to_room[RIGHT] = NONE;
	rms[SMALL_BOX].exits_to_room[FORWARD] = NONE;
	rms[SMALL_BOX].exits_to_room[BELOW] = NONE;
	rms[SMALL_BOX].exits_to_room[BACK] = SECRET_ROOM;

	rms[RING].description.assign("a small golden ring with a red gem. The gem seems loose.");
	rms[RING].exits_to_room[ABOVE] = NONE;
	rms[RING].exits_to_room[LEFT] = NONE;
	rms[RING].exits_to_room[RIGHT] = NONE;
	rms[RING].exits_to_room[FORWARD] = NONE;
	rms[RING].exits_to_room[BELOW] = NONE;
	rms[RING].exits_to_room[BACK] = SMALL_BOX;

	rms[PHONE].description.assign("a brief case. It has a 4 digit combination lock, do I know the code?");
	rms[PHONE].exits_to_room[BACK] = SECRET_ROOM;
	rms[RING].exits_to_room[ABOVE] = NONE;
	rms[RING].exits_to_room[LEFT] = NONE;
	rms[RING].exits_to_room[RIGHT] = NONE;
	rms[RING].exits_to_room[FORWARD] = NONE;
	rms[RING].exits_to_room[BELOW] = NONE;


	

}

void set_directions(words *dir)
{
	dir[ABOVE].code = ABOVE;
	dir[ABOVE].word = "ABOVE";

	dir[LEFT].code = LEFT;
	dir[LEFT].word = "LEFT";

	dir[RIGHT].code = RIGHT;
	dir[RIGHT].word = "RIGHT";

	dir[BELOW].code = BELOW;
	dir[BELOW].word = "BELOW";

	dir[FORWARD].code = FORWARD;
	dir[FORWARD].word = "FORWARD";

	dir[BACK].code = BACK;
	dir[BACK].word = "BACK";

}

bool parser(int &loc, string wd1, string wd2, words *dir, words *vbs, room *rms, noun *nns)
{

	static bool open_state = false; //is the door open, false = closed and true = opened
	bool is_watchLocked = true; //is the watch locked, true = locked and false = open
	bool is_toolboxOpen = false; // is the tool box open, true = open and false = closed
	bool is_watchOpen = false; //is the watch open, true = open and false = closed
	bool gotKey_watch = false; // did the player get the key from watch, false = no and true = yes
	bool is_boxLocked = true; //is the locked box locked, true = locked and false = unlocked
	bool is_boxOpen = false; // is the locked box open, true = open and false = closed
	bool is_trapDoorLocked = true; //is the trap door locked, true = locked and false = unlocked
	bool is_trapDoorOpen = false; //is the trap door open, false = closed and true = open
	bool is_mainDoorLocked = true;
	bool is_mainDoorOpen = false;

	int verb_action = NONE;
	int noun_match = NONE;


	int i;
	for(i = 0; i < DIRS; i++)
	{
		if(wd1 == dir[i].word)
		{
			if(rms[loc].exits_to_room[dir[i].code] != NONE)
			{
				loc = rms[loc].exits_to_room[dir[i].code];
				cout << "You now see " << rms[loc].description << "." << endl;
				return true;
			}
			else
			{
				cout << "There is not an exit that way." << endl;
				return true;
			}
		}
	}

	for (i = 0; i < VERBS; i++)
	{
		if(wd1 == vbs[i].word)
		{
			verb_action = vbs[i].code;
			break;
		}
	}

	if(wd2.empty())
	{
		for(i = 0; i < NOUNS; i++)
		{
			if(wd2 == nns[i].word)
			{
				noun_match = nns[i].code;
				break;
			}
		}
	}
	

	if(verb_action == NONE)
	{
		cout << "No valid command entered." << endl;
		return true;
	}

	if(verb_action == LOOK)
	{
		look_around(loc, rms, dir, nns);
		return true;
	}

	if (verb_action == OPEN)
	{
		if (noun_match == TOOL_BOX)
		{
			if(loc == FLOOR)
			{
				if(is_toolboxOpen == false)
				{
					is_toolboxOpen = true;
					cout << "I've opened the tool box. There are a pair of snips inside." << endl;
					return true;
				}
				else if(is_toolboxOpen == true)
				{
					cout << "The toolbox is already open." << endl;
					return true;
				}
			}
			else
			{
				cout << "There is no toolbox to open here." << endl;
				return true;
			}
		}
		else
		{
			cout << "Opening that is not possible." << endl;
			return true;
		}

		if(noun_match == BOX)
		{
			if(loc == SHELF)
			{
				cout << "The box is empty." << endl;
				return true;
			}
		}

		if(noun_match == POCKET_WATCH)
		{
			if(loc == SHELF)
			{
				if(is_watchOpen == false && is_watchLocked == true)
				{
					cout << "I can't open this right now, it's jammed. I need something thin to open it." << endl;
					return true;
				}
				else if(is_watchOpen == false && is_watchLocked == false)
				{
					is_watchOpen = true;
					cout << "I've opened the watch. It reads 6:10. It looks like I can change the time." << endl;
					return true;
				}
				else
				{
					cout << "The watch is already open." << endl;
					return true;
				}
			}
			else
			{
				cout << "There is no pocket watch here." << endl;
				return true;
			}
		}
		else
		{
			cout << "Opening that is not possible." << endl;
			return true;
		}

		if(noun_match == LOCKED_BOX)
		{
			if(loc == SECRET_ROOM)
			{
				if(is_boxOpen == false && is_boxLocked == true)
				{
					cout << "I can't open that right now, it's locked. I need a key." << endl;
					return true;
				}
				else if(is_boxOpen == false && is_boxLocked == false)
				{
					is_boxOpen == true;
					cout << "I've opened the box. There is a ring inside." << endl;
					return true;
				}
				else
				{
					cout << "The box is already open." << endl;
					return true;
				}
			}
			else
			{
				cout << "There is no box here." << endl;
				return true;
			}
		}
		else
		{
			cout << "Opening that is not possible." << endl;
			return true;
		}

		if(noun_match == TRAP_DOOR)
		{
			if(loc == CEILING)
			{
				if(is_trapDoorOpen == false && is_trapDoorLocked == true)
				{
					cout << "I can't open that right now, it's locked. I need a key." << endl;
					return true;
				}
				else if(is_trapDoorOpen == false && is_trapDoorLocked == false)
				{
					is_trapDoorOpen = true;
					cout << "I've opened the door. It looks like it leads to another room." << endl;
					return true;
				}
				else
				{
					cout << "The door is already open." << endl;
					return true;
				}
			}
			else
			{
				cout << "There is no door here." << endl;
				return true;
			}
		}
		else
		{
			cout << "Opening that is not possible." << endl;
			return true;
		}

		if(noun_match == MAIN_DOOR)
		{
			if(loc == DOOR)
			{
				if(is_mainDoorOpen == false && is_mainDoorLocked == true)
				{
					cout << "I can't open that right now, it's locked. It looks like the key is some sort of gem." << endl;
					return true;
				}
				if(is_mainDoorOpen == false && is_mainDoorLocked == false)
				{
					is_mainDoorOpen = true;
					cout << "I've opened the door! I'm free! I should leave now." << endl;
					return true;
				}
				else
				{
					cout << "The door is already open. I should leave." << endl;
					return true;
				}
			}
			else
			{
				cout << "There is no door here." << endl;
				return true;
			}
		}
		else
		{
			cout << "Opening that is not possible." << endl;
			return true;
		}

	}

	cout << "That was not a valid command." << endl;
	return false;
}

void set_verbs(words *vbs)
{
	vbs[TAKE].code = TAKE;
	vbs[TAKE].word = "TAKE";

	vbs[DROP].code = DROP;
	vbs[DROP].word = "DROP";

	vbs[USE].code = USE;
	vbs[USE].word = "USE";

	vbs[OPEN].code = OPEN;
	vbs[OPEN].word = "OPEN";

	vbs[CLOSE].code = CLOSE;
	vbs[CLOSE].word = "CLOSE";

	vbs[EXAMINE].code = EXAMINE;
	vbs[EXAMINE].word = "EXAMINE";

	vbs[INVENTORY].code = INVENTORY;
	vbs[INVENTORY].word = "INVENTORY";

	vbs[LOOK].code = LOOK;
	vbs[LOOK].word = "LOOK";

}

void look_around(int loc, room *rms, words *dir, noun *nns)
{
	int i;
	cout << "I see " << rms[loc].description << endl;

	//tell the player where the exits are
	for(i = 0; i < DIRS; i++)
	{
		if( rms[loc].exits_to_room[i] != NONE)
		{
			cout << "I can move " << dir[i].word <<  "." << endl;
		}
	}
	for(i = 0; i < NOUNS; i++)
	{
		if(nns[i].location == loc)
		{
			cout << "I see " << nns[i].description << "." << endl;
		}
	}
}

void set_nouns(noun *nns)
{
	nns[HANGER].word = "hanger";
	nns[HANGER].code = HANGER;

	nns[OVER_COAT].word = "coat";
	nns[OVER_COAT].code = OVER_COAT;

	nns[BOX].word = "box";
	nns[BOX].code = BOX;

	nns[POCKET_WATCH].word = "watch";
	nns[POCKET_WATCH].code = POCKET_WATCH;

	nns[PHOTO].word = "photograph";
	nns[PHOTO].code = PHOTO;

	nns[TOOL_BOX].word = "toolbox";
	nns[TOOL_BOX].code = TOOL_BOX;

	nns[WIRE_CUTTERS].word = "snips";
	nns[WIRE_CUTTERS].code = WIRE_CUTTERS;

	nns[CROWBAR].word = "crowbar";
	nns[CROWBAR].code = CROWBAR;

	nns[GOLD_KEY].word = "key";
	nns[GOLD_KEY].code = GOLD_KEY;

	nns[BLAZER].word = "blazer";
	nns[BLAZER].code = BLAZER;

	nns[PDA].word = "PDA";
	nns[PDA].code = PDA;

	nns[LETTER].word = "letter";
	nns[LETTER].code = LETTER;

	nns[SILVER_KEY].word = "key";
	nns[SILVER_KEY].code = SILVER_KEY;

	nns[LOCKED_BOX].word = "box";
	nns[LOCKED_BOX].code = LOCKED_BOX;

	nns[RING].word = "ring";
	nns[RING].code = RING;

	nns[NOTE_ONE].word = "note";
	nns[NOTE_ONE].code = NOTE_ONE;

	nns[NOTE_TWO].word = "note";
	nns[NOTE_TWO].code = NOTE_TWO;

	nns[GEM].word = "gem";
	nns[GEM].code = GEM;

	nns[MAIN_DOOR].word = "door";
	nns[MAIN_DOOR].code = MAIN_DOOR;

	nns[TRAP_DOOR].word = "door";
	nns[TRAP_DOOR].code = TRAP_DOOR;



}