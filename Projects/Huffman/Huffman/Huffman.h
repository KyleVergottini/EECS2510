#include <string>

using namespace std;

class Huffman
{
private:
	/*Copied over from the BST project and is used to create the Huffman encoder tree.
	Differences between this and the BST node are name changes in the variables that hold the data
	and a lack of parent pointer as it is not needed here.*/
	struct Node
	{
		int		Character;	//While Character represents the ASCII character associated with that node,
							//it is stored as an int in the tree.
		int		Weight;
		Node*	LeftChildPointer = NULL;
		Node*	RightChildPointer = NULL;
	};

	/*Points to the root of the Huffman tree.  It is initialized as NULL and is set by the
	InitializeByFile.  It also acts for a flag to show that InitializeByFile has been run
	as it will be NULL if it has not.*/
	Node* EncoderTree = NULL;
	
	/*Generated using the Huffman tree, storing the encoding bit strings for each of the 256 characters.
	Generated just after the Huffman tree is built in InitializeFromFile.*/
	string EncoderTable[256];

	/*Generated just after the EncoderTable using the first bit string found with a length of at least 8.
	Used to pad the end of the encoded file in the case that the number of encoded bits are not evenly
	divisble into bytes.*/
	string PaddingString;

	/*Function used to build the table using the tree.  Defined separately because of its recursive nature.*/
	void BuildTable(Node* Current, string Path);

public:
	/*Default (and only) construct with no parameters.  The only variable
	on an instance of Huffman is encoder, which points to the root of its tree used for
	encoding.  This is not set intil InitializeFromFile is ran, so each instance of Huffman
	has no unique qualities until then.*/
	Huffman();

	/*Takes an input file (located by name passed in) and builds a Huffman tree based on it.
	The root of this tree will be assigned to EncoderTree.  This tree will then be used to create
	the EncoderTable and PaddingString for use in the other functions.  When complete, the
	number of bytes read from the file will be printed to the console.*/
	void InitializeFromFile(string FileName);

	/*Takes an input file, encodes it using the EncoderTable, and ouputs it to a new file with
	the name of OutFile passed in.  If InitializeFromFile has not yet been run, this function
	will stop and print a message reporting this.  In the case of a successful run, the following
	metrics will be printed:  Number of input / output byte counts, percentage of the encoded file's
	byte count over the input file's byte count, and the elapsed time to encode the file.*/
	void EncodeFile(string InFile, string OutFile);

	/*Takes an input file, decodes it using the EncoderTree, and outputs it to a new file.
	InitializeFromFile must have been run first, and the same metrics that were printed
	from EncodeFile should be printed here too.*/
	void DecodeFile(string InFile, string OutFile);
};