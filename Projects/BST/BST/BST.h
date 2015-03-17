/*
Name: Kyle Vergottini
Class: EECS 2510
Project: Binary Search Tree
Due Date: 2/16/2015
This file is the header for the BST class.  BST stands for binary search tree,
and this class is used to create a dynamic set with the seven dynamic set operators
using a binary search tree structure.  This header is included in BSTApp and contains
the declarations for all of the functions necessary to implement the dynamic set.
However, all of the calls made to the class through BSTApp will be done through the function cmd.
*/

#include <string>
using namespace std;

class BST
{
public:
	/*The only public functions for the BST are its constructor and cmd.  Cmd accepts a command, calls the necessary private functions,
	then outputs the results that the user should eventually see as a string.*/
	BST();
	string cmd(string command);

private:
	/*The Node struct is the building block of the tree.  Each node on the tree contains a word, a count for that word, and the pointers to
	its children and parent nodes.  Each pointer is defaulted to NULL.*/
	struct Node
	{
		string	word;
		int		count;
		Node*	leftChildPointer = NULL;
		Node*	rightChildPointer = NULL;
		Node*	parentPointer = NULL;
	};

	/*The only variable that an instance of the BST class contains is the root node.  The root is defaulted to NULL in the constructor,
	and once the root is set, the rest of the tree can be linked to from that root node.*/
	Node* root;

	//The following functions are helpers used in other functions in the class

	/*SweetSpot takes a string and returns either the node with that string or, if one doesn't exist, the leaf
	with the closest value.  While searching the tree for a string is necessary for most all of the other functions,
	returning the closest one is especially useful for inserts.*/
	Node* sweetSpot(string key);

	/*The successor and the predecessor functions take a node and return the node with the closest higher-valued or lower-valued node respectively.
	Predecessor is used in the prev function, and successor is used in both next and deleteNode.*/
	Node* successor(Node* key);
	Node* predecessor(Node* key);

	/*Transplant is used exclusively in the deleteNode function.  It takes two nodes and replaces the original in the list with the replacement.*/
	void transplant(Node* original, Node* replacement);

	/*Traverse is used exclusively in the list function.  It is split out from list because of its recursive nature.*/
	string traverse(Node* key);

	//The rest of the functions are the dynamic operators to be called by cmd.  Each returns a string that is to be output for the user to see.

	/*Inserts a string into the list with a count of 1.  If the string already exists on the tree, its count is incremented instead.
	Returns the string along with the updated count.*/
	string insert(string key);

	/*Deletes a string from the list.  This involves either decrementing its count or removing it entirely if its count is only 1.
	Returns the string along with the updated count.  If the string wasn't in the list, the count returned is -1.*/
	string deleteNode(string key);

	/*Searchs for a string in the list.  Returns the string along with its count.  If the string isn't in the list, the count returned is 0.*/
	string search(string key);

	/*Returns the lowest value string in the list.*/
	string min();

	/*Returns the highest value string in the list.*/
	string max();

	/*Returns the next value in the ordered list from a string.  That is, it returns the closest higher-valued stirng.*/
	string next(string key);

	/*Returns the previous value in the ordered list from a string.*/
	string prev(string key);

	/*Returns all strings in the list, in ascending order, along with their counts.*/
	string list();
};