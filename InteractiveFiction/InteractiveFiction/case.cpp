#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "classes.h"

void section_command(string cmd, string &wd1, string &wd2) //this function changes the formatting of the input to elminiate the case sensitivties issues
{
	string sub_str; //holds the word 
	vector<string> words; //vector holds all of the words after they have been split up
	char search = ' '; //what the function is searching to seperate the words
	size_t i, j;
	// Split Command into vector
	for (i = 0; i < cmd.size(); i++) //loop through the number of characters in the string command
	{
		if (cmd.at(i) != search) //the if there is a space at the current character
		{
			sub_str.insert(sub_str.end(), cmd.at(i)); 
		}
		if (i == cmd.size() - 1) // if i equals one less than the size of the command variable
		{
			words.push_back(sub_str); //push the word to the vector
			sub_str.clear(); //clear the sub string
		}
		if (cmd.at(i) == search) //if the command at the index is an empty character
		{
			words.push_back(sub_str); //push the word to the vector
			sub_str.clear(); //clear the sub string
		}
	}
	// Clear out any blanks
	// I worked backwords through the vectors here as a cheat to not invalidate the iterator
	for (i = words.size() - 1; i > 0; i--)
	{
		if (words.at(i) == "") //if the word at index i is a space
		{
			words.erase(words.begin() + i); //erase the blank
		}
	}
	// Make words upper case
	// Right here is where the functions from cctype are used
	for (i = 0; i < words.size(); i++)
	{
		for (j = 0; j < words.at(i).size(); j++)
		{
			if (islower(words.at(i).at(j))) //if the character at the index is lower case
			{
				words.at(i).at(j) = toupper(words.at(i).at(j)); //then change it to upper case
			}
		}
	}
	// For the moment I only want the first to words at most (verb / noun).
	if (words.empty()) //if no words were give as input
	{
		cout << "No command given" << endl; //tell the player
	}
	if (words.size() == 1) //if only one word was inputed
	{
		wd1 = words.at(0); //set it to the first position in the vector
	}
	if (words.size() == 2) // if two words were inputed
	{
		wd1 = words.at(0); //set word one to the first index
		wd2 = words.at(1); //set word two to the second index
	}
	if (words.size() > 2) //if the input was greater than two
	{
		cout << "Command too long. Only type one or two words (direction or verb and noun)" << endl; //tell the player
	}
}