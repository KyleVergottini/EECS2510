#include "stdafx.h"
#include "Huffman.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

int main()
{
	char WaitChar;
	Huffman test;
	test.InitializeFromFile("Shakespeare.txt");
	test.EncodeFile("Shakespeare.txt", "Shakespeare");
	test.DecodeFile("Shakespeare.enc", "Shakespeare");
	cout << "Press enter to exit program.";
	cin.get(WaitChar);
	return 0;
}

Huffman::Huffman() {};

void Huffman::BuildTable(Node* Current, string Path)
{
	if (Current->LeftChildPointer != NULL)

	{
		BuildTable(Current->LeftChildPointer, Path + "0");
	}
	if (Current->RightChildPointer != NULL)
	{
		BuildTable(Current->RightChildPointer, Path + "1");
	}
	if (Current->Character != NULL)
	{
		EncoderTable[Current->Character] = Path;
	}
}

void Huffman::InitializeFromFile(string FileName)
{
	ifstream inputFile;	//Create file input
	inputFile.open(FileName, ios::binary);	//Open using file name passed in
	if (inputFile.fail())	//Check to make sure it was opened successfully
	{
		cout << "File " << FileName << " failed to open.\n";	//If not, print an error message and stop the function
		return;
	}
	int frequency[256] = { 0 };	//Set up an array to hold the counts of each of the 255 ASCII characters in the input
								//Each character is represented by its index in the array
	char symbol;	//Holds the characters from the file
	int bytesRead = 0;	//Holds the number of bytes read from the file
	inputFile.get(symbol);	//Get the first character
	while (!inputFile.eof())	//Go through the file until the end is reached
	{
		frequency[(int)symbol]++;	//Increment the count of whichever element in the array corresponds to the current character
		bytesRead++;	//Increment the byte count
		inputFile.get(symbol);	//Go to the next character
	}
	if (bytesRead == 0)	//Check to make sure some bytes were read
	{
		cout << "File " << FileName << " is empty.\n";	//If not, the file is empty and there is nothing to initialize from
		return;
	}
	inputFile.close();	//Close the file input stream after reaching the end
	Node* subtree[256] = { NULL };	//Set up an array to hold the nodes that will make up the Huffman tree
	for (int i = 0; i < 256; i++)	//For element in the frequency array, assign a new tree node to the corresponding index
									//in the new array with its ASCII int value and its frequency.
	{
		subtree[i] = new Node();
		subtree[i]->Character = i;
		subtree[i]->Weight = frequency[i];
	}
	int min1, min2;	//The two lowest weight nodes are tracked
	Node* combination;
	do {
		min1 = -1;	//Set both minimums to -1 to clear them
		min2 = -1;
		for (int i = 0; i < 256; i++)	//Iterate through the array of nodes
		{
			if (subtree[i] != NULL && (min1 == -1 || subtree[i]->Weight < subtree[min1]->Weight))
			{
				min1 = i;	//A node is set to the 1st min if there is no 1st min or the current node has a lower weight
			}
		}
		for (int i = 0; i < 256; i++)	//Iterate through it a second time
		{
			if (i != min1 && subtree[i] != NULL && (min2 == -1 || subtree[i]->Weight < subtree[min2]->Weight))
			{
				min2 = i;	//A node is set to the 2nd min if there is no 2nd min or the current node has a lower weight 
							//AND it isn't already the 1st
			}
		}
		if (min2 != -1)	//As long as two min's were actually found
		{
			combination = new Node();	//Create a new node 
			combination->Character = NULL;	//Set the character to null as this is an inner node
			combination->Weight = subtree[min1]->Weight + subtree[min2]->Weight;	//Set the weight as the combination of the min's
			combination->LeftChildPointer = subtree[min1];	//Set the new node as the parent of the two min's
			combination->RightChildPointer = subtree[min2];
			subtree[min1] = combination;	//Set the new node in place of min1 in the array
			subtree[min2] = NULL;	//Set min2's spot to NULL
		}
	} while (min2 != -1);	//If no min2 is found, there's only one node left which is the root for the completed tree
	EncoderTree = subtree[min1];	//Assign the Huffman tree to the EncoderTree variable
	BuildTable(EncoderTree, "");	//Build the table of encoding strings using the tree, which is then assigned to EncoderTable
	int i = 0;
	do {
		PaddingString = EncoderTable[i];	//Iterate through the tree to find a suitable string for the PaddingString
		i++;
	} while (PaddingString.length() < 7);	//When one is found with a length of at least 8
	PaddingString = PaddingString.substr(0,7);	//The PaddingString is set to be the first 7 characters of that string
	cout << "Initialization from file " << FileName << " complete.  " << bytesRead << " bytes read.\n";	//Print a message to show that it's complete
	return;
}

void Huffman::EncodeFile(string InFile, string OutFile)
{
	if (EncoderTree == NULL)	//If the EnocderTree isn't set, InitializeFromFile hasn't been run and the function ends
	{
		cout << "File not initialized";
		return;
	}
	long startTime = time(NULL);	//Start tracking the time taken to run this function
	ifstream inputFile;	//Code to read input is similar between the three public functions
	ofstream outputFile(OutFile + ".enc", ofstream::trunc | ofstream::binary);	//Also includes output stream to output file with extensions .enc
	inputFile.open(InFile, ios::binary);
	if (inputFile.fail())
	{
		cout << "File " << InFile << " failed to open.\n";
		return;
	}
	char inputChar;
	unsigned char outputChar;
	string binaryValue;	//Used to hold bytes to be inserted into encoded file
	int decimalValue;	//Used to hold decimal conversion of binaryValue
	int bytesRead = 0;
	int bytesWritten = 0;
	string outputBits = "";
	inputFile.get(inputChar);
	while (!inputFile.eof())
	{
		outputBits = outputBits + EncoderTable[(int)inputChar];	//After reading a character, append its encoding string to outputBits
		while (outputBits.length() >= 8)	//While outputBits has at least 8 characters
		{
			binaryValue = outputBits.substr(0, 8);	//Set the first byte to binaryValue
			outputBits = outputBits.substr(8);	//Remove the first byte from outputBits
			decimalValue = 0;	//Set decimalValue to 0
			for (int j = 0; j < 8; j++)	//Iterate through the binaryValue
			{
				if (binaryValue.substr(7 - j,1) == "1")	//If the current character is a 1
				{
					decimalValue += pow(2, j);	//Mathematically generate the decimalValue
				}
			}
			outputChar = (unsigned char)decimalValue;	//Convert the decimalValue to an unsigned char
			outputFile.write(reinterpret_cast<char*>(&outputChar), 1);	//Cast outputChar to a char* to write to the encoding file
			bytesWritten++;	//Increment the bytesWritten count
		}
		bytesRead++;	//Increment the bytesRead for each character read
		inputFile.get(inputChar);	//Get the next character from the input
	}
	if (outputBits.length() != 0)	//If there are still leftover bits when the file is completely read
	{
		outputBits = outputBits + PaddingString.substr(0, 8 - outputBits.length());	//Pad the outputBits with the PaddingString so that it makes 8 bits
		decimalValue = 0;
		for (int j = 0; j < 8; j++)	//Convert the last byte to a decimal value
		{
			if (outputBits.substr(7 - j, 1) == "1")
			{
				decimalValue += pow(2, j);
			}
		}
		outputChar = (unsigned char)decimalValue;
		outputFile.write(reinterpret_cast<char*>(&outputChar), 1);	//Output the last byte to the file
		bytesWritten++;
	}
	inputFile.close();
	outputFile.close();
	if (bytesRead == 0)
	{
		cout << "File " << InFile << " is empty.\n";
		return;
	}
	long endTime = time(NULL);	//Check the time at completion
	cout << bytesRead << " bytes read from " << InFile << ".\n";	//Print the bytes read from the input
	cout << bytesWritten << " bytes written to " << OutFile << ".enc.\n";	//Print the bytes written to the encoded file
	cout << "Encoded to be " << 100 * (double)bytesWritten / (double)bytesRead << "% of the original file.\n";	//Print the bytes written-to-read ratio as a percentage
	cout << endTime - startTime << " seconds elapsed to encode.\n";	//Print the seconds elapsed to run the function
	return;
}

void Huffman::DecodeFile(string InFile, string OutFile)
{
	//As mentioned before, most of the code at the beginning of the functions are the same between all 3
	long startTime = time(NULL);
	ifstream inputFile;
	inputFile.open(InFile, ios::binary);
	ofstream outputFile(OutFile + ".dec", ofstream::trunc | ofstream::binary);	//Decoded file will be output with the extension .dec
	if (inputFile.fail())
	{
		cout << "File " << InFile << " failed to open.\n";
		return;
	}
	char symbol;
	unsigned char outputChar;
	int decimalValue;
	string outputBits;
	int bytesRead = 0;
	int bytesWritten = 0;
	Node* current = EncoderTree;	//This is used to store the current node in the tree, as the file will be decoded using the Huffman tree
	inputFile.get(symbol);
	while (!inputFile.eof())	//While the file is being read
	{
		decimalValue = (int)(unsigned char)symbol;	//Get the decimal value of the value read
		outputBits = "";	//Clear the outputBits variable
		for (int i = 7; i >= 0; i--)	//Similar to the EncodeFile function, find the binary value of the character read mathematically as a byte
		{
			if (decimalValue - pow(2, i) >= 0)
			{
				outputBits = outputBits + "1";
				decimalValue -= pow(2, i);
			}
			else
			{
				outputBits = outputBits + "0";
			}
		}
		for (int i = 0; i < 8; i++)	//Once the byte conversion is obtained, iterate through it use it to move through the tree
		{
			if (outputBits.substr(i, 1) == "1")	//If the current bit is 1, move right through the tree
			{
				current = current->RightChildPointer;
			}
			else	//Otherwise, move left
			{
				current = current->LeftChildPointer;
			}
			if (current->Character != NULL)	//If, after moving, the character of the current node is not NULL
			{
				outputChar = (unsigned char)current->Character;	//We must be at a tree node, so read the current node's character as a char
				outputFile.write(reinterpret_cast<char*>(&outputChar), 1);	//Output the char to the output file
				bytesWritten++;	//Increment the number of bytes written
				current = EncoderTree;	//Move back to the root of the tree before continuing
			}
		}
		inputFile.get(symbol);	//Read the next character in the file.  The place in the tree will be maintained between reads
		bytesRead++;	//Increment the number of bytes read
	}
	outputFile.close();
	long endTime = time(NULL);
	cout << bytesRead << " bytes read from " << InFile << ".\n";
	cout << bytesWritten << " bytes written to " << OutFile << ".dec.\n";
	cout << endTime - startTime << "seconds elapsed to decode.\n";
	return;
}