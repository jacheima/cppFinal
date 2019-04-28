#include <iostream>
#include <string>
#include <vector> //used to hold the commands of the player 
#include <cctype> //will be used to eliminate case sensitivity problems
#include "classes.h" //my header file that i created

using namespace std;



//declaring my functions
void section_command(string cmd, string &wd1, string &wd2); //handles formatting of the users input, this is located inside the case.cpp file
void set_rooms(room *rms); //sets the rooms information
void set_directions(words *dir); //sets the direction information
bool parser(int &loc, string wd1, string wd2, words *dir, words *vbs, room *rms, noun *nns); //sets the parser
void set_verbs(words *vbs); //sets the verbs information
void look_around(int loc, room *rms, words *dir, noun *nns); //sets the look around function
void set_nouns(noun *nns); //sets the noun information

int main()
{
	int location = MAIN; //set the location of the player using the enumerated type identifier

	string command; //holds the input from the user, this will be chopped into pieces in the section_command function
	string word_1; //holds word one 
	string word_2; //holds word two 

	room rooms[ROOMS]; //uses the class room, an array that holds the rooms on the map
	set_rooms(rooms); //call to the function that sets the information about the rooms

	words directions[DIRS]; //uses the class words, an array that holds the directions the player can move in
	set_directions(directions); //call to the function that sets the information about the rooms

	words verbs[VERBS]; //uses the class words, an array that holds all the accepted verbs by the program
	set_verbs(verbs); //call to the function that sets the information about the verbs

	noun nouns[NOUNS]; //uses the class noun, an array that holds all the accepted nouns that the program uses
	set_nouns(nouns); //call to the function that sets the information about the nouns

	string name; //string variable that holds the name of the user

	cout << "Hello, wait. What is your name?" << endl;
	getline(cin, name);
	cout << "Hello, " << name << "." << endl << endl;
	
	cout << "This game is an escape the room text adventure game." << endl;
	cout << "During this game you will type 1 to 2 word commands in order to interact with the world." << endl;
	cout << "At anytime, if you need help, simply type 'help' and the list of commands and a description of what they do will appear." << endl;

	cout << endl;
	cout << "------------------------------------------------------------" << endl;
	cout << endl;

	cout << "Your head is pounding. You wince as the light from the room stabs your eyes. Above you there is a rough grey ceiling. You don't recognize it." << endl;
	cout << "Your body hurts. You try to remember what happened, you grasp for the memory. White hot searing pain rips through your body as it evades you. What happened?" << endl;
	cout << "Type 'look' to look around the room." << endl;
	cout << "Type 'help' if you need help at anytime." << endl;

	while (word_1 != "QUIT") //while loop, it will continue to ask the player what they would like to do until they type quit
	{
		command.clear(); //clear the command variable
		cout << "What would you like to do?" << endl;
		getline(cin, command); //user inputs their command
		word_1.clear(); //clear the previous word_1 from the variable
		word_2.clear(); //clear the previous word_2 from the variable
		// Call the function that handles the command line format.
		section_command(command, word_1, word_2);
		//call the parser function
		parser(location, word_1, word_2, directions, verbs, rooms, nouns);

	}
	return 0; //exit
}
void set_rooms(room *rms) //function that sets the info about the rooms. 
{
	rms[MAIN].description.assign("the main room"); //assigns a description to the room. This will be called later when the player uses the look command and when they enter a new room
	rms[MAIN].exits_to_room[NORTH] = EXIT; //sets the north exit to the room named exit
	rms[MAIN].exits_to_room[EAST] = BATHROOM; //sets the east exit to the room named bathroom
	rms[MAIN].exits_to_room[SOUTH] = CLOSET; //sets the south exit to the room named closet
	rms[MAIN].exits_to_room[WEST] = NONE; // sets the west exit to none, which means there is no exit to the west


	rms[BATHROOM].description.assign("the bathroom"); //assigns a description to the room. This will be called later when the player uses the look command and when they enter a new room
	rms[BATHROOM].exits_to_room[NORTH] = NONE; //sets the north exit to none, which means there is no exit to the north
	rms[BATHROOM].exits_to_room[EAST] = NONE; //sets the east exit to none, which means there is no exit to the east
	rms[BATHROOM].exits_to_room[SOUTH] = NONE; //sets the south exit to none, which means there is no exit to the south
	rms[BATHROOM].exits_to_room[WEST] = MAIN; // sets the west exit to the room named main

	rms[CLOSET].description.assign("the closet"); //assigns a description to the room. This will be called later when the player uses the look command and when they enter a new room
	rms[CLOSET].exits_to_room[NORTH] = MAIN; // sets the north exit to the room named main
	rms[CLOSET].exits_to_room[EAST] = NONE; //sets the east exit to none, which means there is no exit to the north
	rms[CLOSET].exits_to_room[SOUTH] = NONE; //sets the south exit to none, which means there is no exit to the north
	rms[CLOSET].exits_to_room[WEST] = NONE; //sets the west exit to none, which means there is no exit to the north


	rms[EXIT].description.assign("a feild. The only thing as far as the eye can see is the doorway you've just steped through."); //assigns a description to the room. This will be called later when the player uses the look command and when they enter a new room
	rms[EXIT].exits_to_room[NORTH] = NONE; //sets the north exit to none, which means there is no exit to the north
	rms[EXIT].exits_to_room[EAST] = NONE; //sets the east exit to none, which means there is no exit to the north
	rms[EXIT].exits_to_room[SOUTH] = MAIN; //sets the south exit to the room named main
	rms[EXIT].exits_to_room[WEST] = NONE; //sets the west exit to none, which means there is no exit to the north

}

void set_directions(words *dir) //function that sets the direction information
{
	dir[NORTH].code = NORTH; //sets the code for north to NORTH
	dir[NORTH].word = "NORTH"; //sets the word for north to NORTH

	dir[SOUTH].code = SOUTH; //sets the code for south to SOUTH
	dir[SOUTH].word = "SOUTH"; //sets the word for south to SOUTH

	dir[EAST].code = EAST; //sets the code for east to EAST
	dir[EAST].word = "EAST"; //sets the word for east to EAST

	dir[WEST].code = WEST; //sets the code for west to WEST
	dir[WEST].word = "WEST"; //sets the word for west to WEST


}

bool parser(int &loc, string wd1, string wd2, words *dir, words *vbs, room *rms, noun *nns)  //the parser function, this function takes the player formatted commands and does stuff with them
{                                                                                           //in part it changes the players location when they leave a room
	                                                                                       //it looks at the verb and the noun following to see if it made sense, if it did, it tells the player something 
	static bool door_state = false;
	static bool has_key = false;
	static bool has_note = false;
	static bool isLocked = true;
	int verb_action = NONE;
	int noun_match = NONE;
	vector<string> inventory;
	inventory.push_back("INVENTORY: ");


	int i;
	for (i = 0; i < DIRS; i++)
	{
		if (wd1 == dir[i].word)
		{
			if (rms[loc].exits_to_room[dir[i].code] != NONE)
			{
				loc = rms[loc].exits_to_room[dir[i].code];
				cout << "You are now in " << rms[loc].description << "." << endl;
				return true;
			}
			else if(rms[loc].exits_to_room[dir[i].code] == EXIT)
			{
				cout << "You've escaped! You've beat the game!" << endl;
				cout << "Type 'quit' to quit the game." << endl;
			}
			else
			{
				cout << "There is not an exit that way." << endl;
				return true;
			}
		}
	}

	for (i = 0; i < VERBS; i++) //my "verb trap" it loops through the verbs until it gets to the verb that was inputed to word_1. Then it sets the verb action to the code of that verb.
	{
		if (wd1 == vbs[i].word)
		{
			verb_action = vbs[i].code;
			break;
		}
	}

	if (wd2 != " ") //if word 2 is not empty (a space)
	{
		for (i = 0; i < NOUNS; i++) //loop through the nouns
		{
			if (wd2 == nns[i].word) //if word 2 equals the noun word
			{
				noun_match = nns[i].code; //then set noun_match to the noun code
				break; //break out of the loop
			}
		}
	}


	if (verb_action == NONE) //if nothing was inputed to word 1
	{
		cout << "No valid command entered." << endl; //tell the player that the command was not valid
		return true; //exit
	}

	if (verb_action == LOOK) // if the command was the word look
	{
		look_around(loc, rms, dir, nns); //go to the look around function
		return true; //exit
	}

	if (verb_action == OPEN) //if the verb was OPEN
	{
		if (noun_match == BATHROOM_DOOR) //and the noun is the bathroom door word
		{
			if (loc == MAIN || loc == BATHROOM) // and the players location is in either the main or bathroom
			{
				if (door_state == false) //if the door is closed
				{
					door_state = true; //set the door to open
					rms[MAIN].exits_to_room[EAST] = BATHROOM; //set the east exit of MAIN to the BATHROOM
					rms[BATHROOM].exits_to_room[WEST] = MAIN; //set the west exit of BATHROOM to MAIN
					nns[BATHROOM_DOOR].description.clear(); // clear the descrption of the bathroom door
					nns[BATHROOM_DOOR].description.assign("an open bathroom door"); //reset the description of the bathroom door
					cout << "I have opened the door." << endl; //tell the player what happened
					return true; //exit
				}
				else if (door_state == true) //or if the door is open
				{
					cout << "The door is already open." << endl; //tell the player the door is already open
					return true; //exit
				}
			}
			else //or if the player is not in those locations
			{
				cout << "There is no door to open here." << endl; //tell the player that there is no door to open
				return true; //exit
			}
		}
		else // or if the player did not enter that noun
		{
			cout << "Opening that is not possible." << endl; //tell the player that it is not possible to open that noun
			return true;//exit
		}
	}
	if (verb_action == TAKE) //if the verb is take
	{
		if (noun_match == KEY) //and if the noun is key
		{
			if (loc == nns[KEY].location) //and if the player is in the location of the key
			{				
				if (has_key == false) //and if the player does not have the key
				{
					has_key = true; //set the player to having the key
					cout << "I picked up a key and put it in my inventory." << endl; //tell the player they have aquired the key
					return true; //exit
				}
				else //or if the player has the key
				{
					cout << "I already picked that up, it is in my inventory." << endl; //tell the player that they already have the key
					return true; //exit
				}

			}
			else // or if the player is not in the location of the key
			{
				cout << "There is no key to take here." << endl; //tell the player that there is no key at this location
				return true; //exit
			}
		}
		else if (noun_match == NOTE) //or if the noun is note
		{
			if (loc == nns[NOTE].location) //and the player is in the same location as the note
			{
				if (has_note == false) //and if the player does not have the note
				{
					has_note = true; //give them the note
					cout << "I picked up the note, it is now in my inventory." << endl; //tell the player they picked up the note
					return true; // exit
				}
				else //or if the player already has the note
				{
					cout << "I can't take that right now, it is already in my inventory." << endl; //tell the player they already have the note
				}
			}
			else //or if the player is not in the location of the note
			{
				cout << "There is no note to take here." << endl; //tell the player there is no note at their location
				return true; //exit
			}
		}
		else //or if the player did not enter that noun
		{
			cout << "Taking that is not possible." << endl; //tell the player they can't take it
			return true; //exit
		}
	}

	if (verb_action == CLOSE) //if the verb is close
	{
		if (noun_match == BATHROOM_DOOR) //and the noun is the bathroom door
		{
			if (loc == BATHROOM || loc == MAIN) //and the player is in the bathroom or the main room
			{
				if (door_state == true) //and if the door is open
				{
					door_state = false; //close the door
					rms[MAIN].exits_to_room[EAST] = BATHROOM; //set the exits
					rms[BATHROOM].exits_to_room[WEST] = MAIN; //set the exits
					nns[BATHROOM_DOOR].description.clear(); //clear the description
					nns[BATHROOM_DOOR].description.assign("a closed bathroom door"); //assign a new one
					cout << "I have closed the bathroom door." << endl; //tell the player they closed the bathroom door
					return true; //exit
				}
				else //or if the bathroom door is already closed
				{
					cout << "The bathroom door is already closed." << endl; //tell the player
					return true; //exit
				}
			}
			else //or if the player is not in that location
			{
				cout << "I can't close that here." << endl; //tell the player
				return true; //exit
			}
		}
		else //or if the player input a different noun
		{
			cout << "Closing that is not possible." << endl; //tell the player its not possible
			return true; //exit
		}
	}

	if (verb_action == DROP) //if the verb is drop
	{
		if (noun_match == KEY) //and the noun is key
		{
			if (loc == BATHROOM || loc == MAIN || loc == CLOSET) //and the player is in the bathroom or the main room or the closet
			{
				if (has_key == true) //if the player has the key
				{
					has_key = false; //drop the key
					cout << "I have dropped the key in " << rms[loc].description << "." << endl; //tell they dropped it and where
					nns[KEY].location = loc; //set the new location of the key
					return true; //exit
				}
				else //or if the player doesn't have the key
				{
					cout << "I can't drop that right now, it is not in my inventory." << endl; //tell them they don't have it
					return true; //exit
				}
			}
			else //or if the player isn't in those locations
			{
				cout << "I can't drop that here." << endl; //tell they player they can't drop it there
				return true; //exit
			}
		}
		else if (noun_match == NOTE) //or if the noun is note
		{
			if (loc == MAIN || loc == BATHROOM || loc == CLOSET) //and if the players location is the main room, bathroom or closet
			{
				has_note = false; //drop the note
				cout << "I droped the note in " << rms[loc].description << "." << endl; //tell the player
				nns[NOTE].location = loc; //set the new location for the note
				return true; //exit
			}
		}
		else //or if the player inputed a noun they cannot drop
		{
			cout << "Dropping that is not possible." << endl; //tell the player
			return true; //exit
		}
	}

	if (verb_action == EXAMINE) //if the verb is examine
	{
		if (noun_match == BED) //and the noun is bed
		{
			if (loc == MAIN) //and the player is in the main room
			{
				cout << "The bed is made and looks like it hasn't been slept in for a long time." << endl; //tell the player about the bed
				return true;//exit
			}
			else //or if they are not in the main room
			{
				cout << "I can't examine that here." << endl; //tell them they can't examine that
				return true;
			}
		}
		else if (noun_match == DESK) //or if the noun is desk
		{
			if (loc == MAIN) //and the player is in the main room
			{
				cout << "The desk is neat. The lamp on the desk is currently off. There is a note sticking out one of the drawers." << endl; //tell them about the desk
				return true; //exit
			}
			else //or if the player is not in the main room
			{
				cout << "I can't examine that here." << endl; //tell them
				return true; //exit
			}
		}
		else if (noun_match == NOTE) //or if the noun is note
		{
			//tell them about the note
			cout << "You pull the note from the drawer." << endl;
			cout << "It reads : There was a man named McGee" << endl;
			cout << "           Who had a pretty gold key" << endl;
			cout << "           It was a great risk" << endl;
			cout << "           And Lanney did frisk" << endl;
			cout << "           So he hid it in the place where you pee" << endl;
			return true; //exit
		}
		else //or if those noun have not been entered
		{
			cout << "I can't examine that right now." << endl; //tell the player they can't examine that
			return true;
		}
	}
	if (verb_action == INVENTORY) //if the verb is inventory
	{
		if (has_key == true) // and the player has the key
		{
			cout << "KEY" << endl; //tell them
		}

		if (has_note == true) // if the player has the note
		{
			cout << "NOTE" << endl; //tell them
		}

		return true; //exit
		
	}
	if (verb_action == USE) //if the verb is use
	{
		if (noun_match == KEY) //and the noun is key
		{
			if (loc == MAIN) //and the player is in the main room
			{
				if (isLocked == true) //and if the door is lock
				{
					isLocked = false; //unlock the door
					cout << "I've used the key to unlock the northern door." << endl; //tell the player
					return true; //exit
				}
				else //or if the door is unlocked
				{
					isLocked = true; //lock the door
					cout << "I've used the key to lock the northern door." << endl; //tell the player
					return true; //exit
				}
				
			}
		}
	}
	if (verb_action == HELP) //if the verb is help
	{
		//tell the player all of the commands
		cout << "Moving: " << endl;
		cout << "\t- To move type one of the following directions:" << endl;
		cout << "\t\t- NORTH: Moves you one room north on the map, if possible." << endl;
		cout << "\t\t- EAST: Moves you one room east on the map, if possible." << endl;
		cout << "\t\t- SOUTH: Moves you one room south on the map, if possible." << endl;
		cout << "\t\t- WEST: Moves you one room west on the map, if possible." << endl;
		cout << endl << endl;
		cout << "Commands: " << endl;
		cout << "\t- LOOK: Gives you a description of your current location." << endl;
		cout << "\t- TAKE something: Picks up the something you enter following the command and puts it in your inventory." << endl;
		cout << "\t- DROP something: Drops the something you enter following the command and takes it out of your inventory." << endl;
		cout << "\t- OPEN something: Opens the something you enter following the command, if possible." << endl;
		cout << "\t- CLOSE something: Closes the something you enter following the command, if possible." << endl;
		cout << "\t- USE something: Uses the something you enter following the command, if possible." << endl;
		cout << "\t- HELP: Opens the help menu." << endl;
		cout << "\t- QUIT: Close the application." << endl;
	}

	return false; //exit
}

void set_verbs(words *vbs) //the function sets the verbs words and code to be used in other parts of the programs for reference
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

	vbs[HELP].code = HELP;
	vbs[HELP].word = "HELP";

}

void look_around(int loc, room *rms, words *dir, noun *nns) //what the player sees when they type look
{
	int i;
	cout << "I am in " << rms[loc].description << endl;

	//tell the player where the exits are
	for (i = 0; i < DIRS; i++)
	{
		if (rms[loc].exits_to_room[i] != NONE)
		{
			cout << "I can move " << dir[i].word << "." << endl;
		}
	}
	//tell the player what they can see
	for (i = 0; i < NOUNS; i++)
	{
		if (nns[i].location == loc)
		{
			cout << "I see " << nns[i].description << "." << endl;
		}
	}
}

void set_nouns(noun *nns) //function sets the information about the nouns
{
	// BATHROOM_DOOR, DESK, BED, KEY, NOTE

	nns[BATHROOM_DOOR].word = "DOOR";
	nns[BATHROOM_DOOR].code = BATHROOM_DOOR;
	nns[BATHROOM_DOOR].description = "a bathroom door";
	nns[BATHROOM_DOOR].can_carry = false;
	nns[BATHROOM_DOOR].location = MAIN;

	nns[DESK].word = "DESK";
	nns[DESK].code = DESK;
	nns[DESK].description = "a desk";
	nns[DESK].can_carry = false;
	nns[DESK].location = MAIN;

	nns[BED].word = "BED";
	nns[BED].code = BED;
	nns[BED].description = "a bed";
	nns[BED].can_carry = false;
	nns[BED].location = MAIN;

	nns[KEY].word = "KEY";
	nns[KEY].code = KEY;
	nns[KEY].description = "a key";
	nns[KEY].can_carry = true;
	nns[KEY].location = BATHROOM;

	nns[NOTE].word = "NOTE";
	nns[NOTE].code = NOTE;
	nns[NOTE].description = "a note";
	nns[NOTE].can_carry = true;
	nns[NOTE].location = MAIN;


}