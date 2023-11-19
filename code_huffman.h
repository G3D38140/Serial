
#pragma once
//#ifndef _SIMPLEHUFFMANH_
//#define _SIMPLEHUFFMANH_ 
#include <memory.h>     // memset
#include <stdlib.h>     // qsort

typedef unsigned char BYTE;

class simple_Huffman
{
private:
	class node{
	public:
		node *left, *right;
		int count;       // ref. count
		int code;        // code in bits
		int codelength;  // code length
		BYTE chr;        // ASCII char, serves in compression and then in writing of frequencies
		node(void)  {memset(this, 0, sizeof(node));} // constructor, memset is faster than individ.
		~node() {
			if (left) {delete left; left = NULL;}
			if (right) {delete right; right = NULL;}
		}
	};
	node* trees[256] = { 0 };  // Array of trees, here will be only one tree at pos. 0 when finished with merging 
	node *leaves[256]; // array of leaves, each contains ASCII char,
	                   // ref. count, later code and codelength too. It is a reference to original trees
	node *trees_backup[256];
	int STEPS[256];    // as the tree is constructed, this stores exact steps (movings) of the last tree
	int stepscount;
	node nodes[256];

	void MakeTree();
	void MakeTreeD(); // maketree for decompression (no searching)
	int treescount;
	BYTE *allocatedoutput;
	void InitializeLeaves(node *, int, int);              // initialization of leaves with codes and code lengths
	static int comparefreq(const void *A, const void *B); // sorting, we want to sort descending
	void moveTreesRight(node **toTree);
	void moveTreesRight2(node **toTree);
	void tryToPop();
	void moveToTop();
	void performInternalCleanup();
	void initialize();
	int LastError;
public:
	simple_Huffman();
	~simple_Huffman();

	int Compress(BYTE *input, int inputlength);
	int Decompress(BYTE *input, int inputlength);
	int CompressFile(char *InFile, char *OutFile);
	int DecompressFile(char *InFile, char *OutFile);
	void Finalize(); // call after each stream compress/decompress to deallocate
	BYTE *getOutput(); // call after each stream compress/decompress to obtain output
	int getLastError();
};
//#endif
