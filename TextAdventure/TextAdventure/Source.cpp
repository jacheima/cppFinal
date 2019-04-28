#include <iostream>
#include <string>
#include <vector> //used to hold the commands of the player 
#include <cctype> //will be used to eliminate case sensitivity problems

using namespace std;

enum en_dirs {NORTH, EAST, SOUTH, WEST};
enum en_rooms {MAIN, BATHROOM, CLOSET, EXIT};
enum en_verbs {TAKE, DROP, USE, OPEN, CLOSE, EXAMINE, INVENTORY, LOOK};
enum en_nouns {BATHROOM_DOOR, CLOSET_DOOR, DESK, BED, KEY, NOTE};

const int NONE = -1;
const int DIRS = 4;
const int ROOMS = 4;
const int VERBS = 8;
const int NOUNS = 6;

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
	int location = MAIN; //set the location of the player using the enumerated type identifier

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
	rms[MAIN].description.assign("the main room");
	rms[MAIN].exits_to_room[NORTH] = EXIT; //this will be locked
	rms[MAIN].exits_to_room[EAST] = BATHROOM;
	rms[MAIN].exits_to_room[SOUTH] = CLOSET;
	rms[MAIN].exits_to_room[WEST] = NONE;
	

	rms[BATHROOM].description.assign("the bathroom");
	rms[BATHROOM].exits_to_room[NORTH] = NONE;
	rms[BATHROOM].exits_to_room[EAST] = NONE;
	rms[BATHROOM].exits_to_room[SOUTH] = NONE;
	rms[BATHROOM].exits_to_room[WEST] = MAIN;
	
	rms[CLOSET].description.assign("the closet");
	rms[CLOSET].exits_to_room[NORTH] = MAIN;
	rms[CLOSET].exits_to_room[EAST] = NONE;
	rms[CLOSET].exits_to_room[SOUTH] = NONE;
	rms[CLOSET].exits_to_room[WEST] = NONE;
	

	rms[EXIT].description.assign("You escaped.");
	rms[EXIT].exits_to_room[NORTH] = NONE;
	rms[EXIT].exits_to_room[EAST] = NONE;
	rms[EXIT].exits_to_room[SOUTH] = MAIN;
	rms[EXIT].exits_to_room[WEST] = NONE;
	
	


	

}

void set_directions(words *dir)
{
	dir[NORTH].code = NORTH;
	dir[NORTH].word = "NORTH";

	dir[SOUTH].code = SOUTH;
	dir[SOUTH].word = "SOUTH";

	dir[EAST].code = EAST;
	dir[EAST].word = "EAST";

	dir[WEST].code = WEST;
	dir[WEST].word = "WEST";


}

bool parser(int &loc, string wd1, string wd2, words *dir, words *vbs, room *rms, noun *nns)
{

	static bool door_state = false;
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

	if(wd2 != " ")
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
		if (noun_match == BATHROOM_DOOR)
		{
			if (loc == MAIN || loc == BATHROOM)
			{
				if (door_state == false)
				{
					door_state = true;
					cout << "I have opened the door." << endl;
					return true;
				}
				else if (door_state == true)
				{
					cout << "The door is already open." << endl;
				}
			}
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