/*
Name: Kyle Vergottini
Class: EECS 2510
Project: Binary Search Tree
Due Date: 2/16/2015
This file contains the main() function for the project
and calls the BST class.  The project will be explained in detail
in the BST header and source files.
*/

#include <iostream>
#include <string>
using namespace std;
#include "BST.h"

int main()
{
	BST tree;	//Declares an instance of the class
	string input;	
	while (input != "exit") //Enters a loop until the input is set to exit
							//This condition is redundant as cmd will end the program if exit is entered
	{
		cout << ">>";	//Used to denote a user input versus the output
		getline(cin, input);	//Gets an input from the console
		cout << tree.cmd(input) << "\n";	//Runs the input through the cmd function
	}
	return 0;
}