/*
Name: Kyle Vergottini
Class: EECS 2510
Project: Binary Search Tree
Due Date: 2/16/2015
This file contains the implementation of the BST class.
*/

//--NOTE REGARDING THE ASSIGNMENT
/*A number of the functions described say to return a blank line
in certain cases.  None of the functions for this class will return
a blank line as this seems unintuitive.  At the least, a line will
be output describing the case reached.*/

#include "BST.h"
using namespace std;

/*Default, and only, class constructor.  Root is defaulted to NULL.*/
BST::BST()
{
	root = NULL;
}

/*Cmd acts as a command line and is the only public function in the class.  It takes an input string, parses it, runs
the appropriate functions, and finally returns the output of the function called. */
string BST::cmd(string command)
{
	string action, argument;	//The command will be split into two parts (or just be set to the action if no argument is entered)
	int breakPoint = command.find_first_of(" ");	//Check to see if a space was entered
	if (breakPoint == -1)	//If no space was entered, just set the command to action
	{
		action = command;
	}
	else	//If a space was entered
	{
		action = command.substr(0, breakPoint);	//Separate out the action and the argument into separate variables
		argument = command.substr(breakPoint + 1);
	}
	for (int i = 0; i < action.length(); i++)	//Iterate through each of the characters in the action
	{
		char* c = &action[i];	//Point to the character at that index
		*c = tolower(action[i]);	//Lowercase the character
	}	//The action part of the command will be effectively case insensitive because of this
	if (breakPoint == -1)	//Check the relevant commands based on whether or not arguments were supplied
	{
		//Cycle through each of the possible commands and run the corresponding function
		if (action == "max")
		{
			return max();
		}
		else if (action == "min")
		{
			return min();
		}
		else if (action == "list")
		{
			return list();
		}
		else if (action == "help")	//If help is entered, return a list of all of the commands with explanations
		{
			string help = "The following commands are available:\n";
			help += "insert <string> : If string is not already in the list, add it and return the\n";
			help += "	string along with the count of 1.  If it is, increment the count and\n";
			help += "	return the string along with the updated count.\n";
			help += "delete <string> : If the string exists in the list, the count will be\n";
			help += "	decremented and the string along with the updated count will be\n";
			help += "	returned.  If the string only has a count of 1, the string will be\n";
			help += "	deleted from the list and returned along with the count of 0.\n";
			help += "search <string> : If the string exists in the list, it will be returned along\n";
			help += "	with its count.  If not, it will be returned with the count of 0.\n";
			help += "min : Returns the smallest valued string in the list.\n";
			help += "max : Returns the greatest valued string in the list.\n";
			help += "next <string> : Returns the next valued string in the ordered list from the\n";
			help += "	entered string.\n";
			help += "prev <string> : Returns the previous valued string in the ordered list from the\n";
			help += "	entered string.\n";
			help += "list : Returns all of the strings currently in the list along with their counts.";
			help += "exit : Closes out of the program.\n";
			help += "NOTE - <string> denotes an argument.  < and > themselves should not be entered.";
			return help;
		}
		else if (action == "exit")	//Close out of the program
		{
			exit(0);
		}
	}
	else
	{
		//Cycle through each of the possible commands and run the corresponding function with the argument passed in
		if (action == "insert")
		{
			return insert(argument);
		}
		else if (action == "delete")
		{
			return deleteNode(argument);
		}
		else if (action == "search")
		{
			return search(argument);
		}
		else if (action == "next")
		{
			return next(argument);
		}
		else if (action == "prev")
		{
			return prev(argument);
		}
	}
	return "The command you entered is invalid. Please try again or enter help for options.";	//If this point is reached, no command matched the input
}

/*Returns the node with the selected word (or the nearest node if a node doesn't exist for that key)*/
BST::Node* BST::sweetSpot(string key)
{
	Node* current = root;	//Start at the root of the tree
	Node* previous = current;
	int comparison;
	while (current != NULL)	//Begin traversing the tree until we reach the end or the word is found
	{
		previous = current;	//Save the current node
		comparison = current->word.compare(key);	//Compare the word on the current node to the key
		if (comparison < 0) //If the value of the word is less than the key, continue right through the tree
		{
			current = current->rightChildPointer;
		}
		else if (comparison > 0)	//If it is greater, continue left
		{
			current = current->leftChildPointer;
		}
		else if (comparison == 0)	//If the key matches the word, return the current node
		{
			return current;
		}
	}
	return previous;	//If we've reached this point, the key is not in the tree
						//In this case, return the previous node
}

/*Inserts the key into the tree.
If the key already exists in the tree, the count on that node is incremented.
Otherwise, a new node is inserted with this key.
In any case, this function returns the key along with the count on that node*/
string BST::insert(string key)
{
	//If the tree is empty, create the root with the key
	if (root == NULL)
	{
		root = new Node;
		root->word = key;
		root->count = 1;
		return key + " 1";
	}
	else
	{
		Node* spot = sweetSpot(key);	//Attempt to find the key in the tree
		if (spot->word == key)	//If the key is found, increment the node with this word
		{
			spot->count++;
			return key + " " + to_string(spot->count);
		}
		else	//If not, the returned node will be the parent of the new node
		{
			int comparison = spot->word.compare(key);	//Compare the key with the word of the parent node
			if (comparison < 0)	//If the value of the word is less than the key
			{
				spot->rightChildPointer = new Node;	//Create a new right child node
				spot->rightChildPointer->word = key;	//Set the word of the new node
				spot->rightChildPointer->count = 1;	//Start the count at 1
				spot->rightChildPointer->parentPointer = spot;	//Set the parent pointer
				return key + " 1";
			}
			if (comparison > 0)	//If the value of the word is greater than the key
			{
				spot->leftChildPointer = new Node;	//Go through the same steps for a new left child node
				spot->leftChildPointer->word = key;
				spot->leftChildPointer->count = 1;
				spot->leftChildPointer->parentPointer = spot;
				return key + " 1";
			}
		}
	}
}

/*Locates the key in the tree.
If the key exists, it is returned along with the count on that key's node.
If it does not, the key is returned with 0.*/
string BST::search(string key)
{
	Node* spot = sweetSpot(key);	//Attempt to find the key in the tree
	if (spot == NULL || spot->word != key)	//If the key is not found or the tree is empty
	{
		return key + " 0";	//Return the key along with 0
	}
	else
	{
		return key + " " + to_string(spot->count);	//Otherwise, return the key and its count
	}
}

/*Return the word with the least value that exists in the tree.
This word will be in the leftmost node on the tree, so we can find it
by following the left pointers through the tree until we reach the end.*/
string BST::min()
{
	if (root == NULL)
	{
		return "Empty set";	//If the tree has no root, the tree is empty
	}
	Node* current = root;	//Start with the root of the tree
	while (current->leftChildPointer != NULL)	//While the current node has a left child
	{
		current = current->leftChildPointer;	//Move leftward through the tree
	}
	return current->word;	//Return the word in the leftmost node
}

/*Return the word with the greatest value that exists in the tree.
This function works exactly the same as the min function, but it moves
rightward through the tree instead.*/
string BST::max()
{
	if (root == NULL)
	{
		return "Empty set";
	}
	Node* current = root;
	while (current->rightChildPointer != NULL)
	{
		current = current->rightChildPointer;
	}
	return current->word;
}

/*Return the next node in the tree from the key.  If the key has a right child,
the successor is the leftmost node under that.  Otherwise, the successor is found by going up through
the tree until a node is reached by going right.  If neither of these cases is reached, the node has no successor.
This function both takes and returns a node pointer and is not accessible through the command.  It is only used
in the next and deleteNode functions.*/
BST::Node* BST::successor(Node* key)
{
	Node* current = key;
	if (current->rightChildPointer != NULL)	//If the key has a right child
	{
		current = current->rightChildPointer;	//Move to that child
		while (current->leftChildPointer != NULL)	//Continue left through the tree until reaching the end
		{
			current = current->leftChildPointer;
		}
		return current;	//Return whichever node is landed on
	}
	Node* parent = current->parentPointer;	//Begin tracking the parent of the current node
	while (parent != NULL && current == parent->rightChildPointer)	//Keep moving up and left through the tree
	{
		current = parent;
		parent = parent->parentPointer;
	}
	return parent;	//Whichever node is landed on is the successor
					//If the returned node is null, then the node has no successor
}

/*Return the previous node in the tree from the key.  This function works exactly the same as successor,
but all movement in the tree is reversed.*/
BST::Node* BST::predecessor(Node* key)
{
	Node* current = key;
	if (current->leftChildPointer != NULL)
	{
		current = current->leftChildPointer;
		while (current->rightChildPointer != NULL)
		{
			current = current->rightChildPointer;
		}
		return current;
	}
	Node* parent = current->parentPointer;
	while (parent != NULL && current == parent->leftChildPointer)
	{
		current = parent;
		parent = parent->parentPointer;
	}
	return parent;
}

/*Return the next word in the tree from the key.  The next node is determined using the successor function.*/
string BST::next(string key)
{
	if (root == NULL)	//If there is no root, the set is empty
	{
		return "Empty set";
	}
	Node* spot = sweetSpot(key);	//Attempt to find the key in the tree
	if (spot->word != key)	//If the key is not found, return this
	{
		return "Not in the set";
	}
	Node* nextNode = successor(spot); //Find the successor
	if (nextNode == NULL)	//If the successor is null, there is no next node
	{
		return "No next word";
	}
	else	//Otherwise, the word of the successor is the next
	{
		return nextNode->word;
	}
}

/*Return the previous word in the tree from the key.  The previous node is determined using the predecessor function.*/
string BST::prev(string key)
{
	if (root == NULL)	//If there is no root, the set is empty
	{
		return "Empty set";
	}
	Node* spot = sweetSpot(key);	//Attempt to find the key in the tree
	if (spot->word != key)	//If the key is not found, return this
	{
		return "Not in the set";
	}
	Node* prevNode = predecessor(spot); //Find the successor
	if (prevNode == NULL)	//If the successor is null, there is no next node
	{
		return "No previous word";
	}
	else	//Otherwise, the word of the successor is the next
	{
		return prevNode->word;
	}
}

/*Return the words and counts of all nodes in the subtree with the key as the root.
This is the recursive function used to everything in the tree.  The function is called with
the subtree starting with the left child, then the contents of the current node are returned,
then the function is called with the right child.  The string returned contains the contents
of the tree with line breaks in the appropriate places.*/
string BST::traverse(BST::Node* key)
{
	string listOutput = "";
	if (key->leftChildPointer != NULL)
	{
		listOutput = traverse(key->leftChildPointer);
	}
	listOutput += key->word + " " + to_string(key->count) + "\n";
	if (key->rightChildPointer != NULL)
	{
		listOutput += traverse(key->rightChildPointer);
	}
	return listOutput;
}

/*Returns the contents of the list in order of ascending value.  This is done using the
function traverse after checking to make sure that the tree isn't empty.*/
string BST::list()
{
	if (root == NULL)
	{
		return "Empty Set";
	}
	else
	{
		string contents = traverse(root);	//Gets the contents of the list from traverse
		return contents.substr(0, contents.length() - 1);	//Removes the last line break from the list
	}
}

/*Replaces the original node in the tree with the replacement.  This function is a helper function
used in deleteNode.  The pointers between both nodes and their parents are resolved.  However,
their child pointers and their child's parent pointers need to be taken care of separately.
This function can still be properly used with NULL passed in as the replacement.*/
void BST::transplant(Node* original, Node* replacement)
{
	if (original == root)	//If the original is the root of the tree, the replacement becomes the root
	{
		root = replacement;
	}
	else if (original == original->parentPointer->leftChildPointer)	//If the original is a left child
	{
		original->parentPointer->leftChildPointer = replacement;	//Make the replacement the original's parent's left child
	}
	else if (original == original->parentPointer->rightChildPointer)	//If the original is a right child
	{
		original->parentPointer->rightChildPointer = replacement;	//Make the replacement the original's parent's right child
	}
	if (replacement != NULL)	//If the replacement node is not null
	{
		replacement->parentPointer = original->parentPointer;	//Set the original's parent to be the replacement's parent
	}
	delete original;	//Delete the original node from memory
}

string BST::deleteNode(string key)
{
	Node* spot = sweetSpot(key);	//Attempt to find the key in the tree
	if (spot == NULL || spot->word != key)	//If the key is not found or the tree is null, return count of -1
	{
		return key + " -1";
	}
	else if (spot->count > 1)	//If the key exists with a count greater than 1
	{
		spot->count--;	//Decrement the count
		return key + " " + to_string(spot->count);	//Return the updated count
	}
	else
	{
		if (spot->leftChildPointer == NULL)	//If the key node has no left child
		{
			transplant(spot, spot->rightChildPointer);	//Transplant it with its right child
														//If the node is a leaf, it will be properly removed at this point
		}
		else if (spot->rightChildPointer == NULL)	//If the key node has no right child
		{
			transplant(spot, spot->leftChildPointer);	//Transplant it with its left child
		}
		else	//If the node has two children
		{
			Node* nextSpot = successor(spot);	//Find the nodes successor
			Node* leftChildSpot = spot->leftChildPointer;	//Hold the node's left child so we don't lose it when the node is deleted
			if (nextSpot == spot->rightChildPointer)	//If its successor is its right child
			{
				transplant(spot, nextSpot);	//Transplant the node with its successor
				nextSpot->leftChildPointer = leftChildSpot;	//By definition, its successor has no left child in this case, so
															//set its left child to be the original nodes left child
				leftChildSpot->parentPointer = nextSpot;	//Set the original node's left child's parent pointer to the successor
			}
			else
			{
				Node* rightChildSpot = spot->rightChildPointer;	//In this case, we'll also need to hold the node's right child
				nextSpot->parentPointer->leftChildPointer = nextSpot->rightChildPointer;	//Set the successor's parent's left child to be the successor's child
																							//This could potentially be NULL if the successor is a leaf
				if (nextSpot->rightChildPointer != NULL)	//If the successor does have a child
				{
					nextSpot->rightChildPointer->parentPointer = nextSpot->parentPointer;	//Set the successor's child's parent to be the successor's parent 
				}	//At this point, the successor will have been removed from the tree
				transplant(spot, nextSpot);	//Transplant the original node with its successor
				leftChildSpot->parentPointer = nextSpot;	//Reset the original node's left child's parent pointer
				rightChildSpot->parentPointer = nextSpot;	//Reset its right child's as well
				nextSpot->leftChildPointer = leftChildSpot;	//Set the left child pointer of the replacement
				nextSpot->rightChildPointer = rightChildSpot;	//Set the right child pointer of the replacement
			}
		}
		return key + " 0";	//Return the count as 0 
	}
}