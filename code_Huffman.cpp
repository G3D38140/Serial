/*   																	
    Compress using Huffman trees												
    simple_Huffman                v.3.0     (c) 2012 Jan Mojzis				
    speed 5.1 MB / 1s
    tested on Windows 7 Home SP1 32 bit
    Intel Atom CPU N450 1.66 GHz, 2.0 GB RAM
    tested with stream compress

    public class: simple_Huffman												
    main functions: int Compress(BYTE *input, int inputlength)
                      (call Finalize when output is no longer needed)

                    int CompressFile(char *Infilename, char *Outfilename)

                    int Decompress(BYTE *input, int inputlength)
                      (call Finalize when output is no longer needed)

                    int DecompressFile(char *Infilename, char *Outfilename)
    UPDATE 1.4.2012	-- removed parent from node, due to its redundancy		
                      -- removed friend from node								
                      -- added functions: int getLastError(), BYTE *getOutput(),
                      Finalize(), int DecompressFile(char *Infilename, char *Outfilename), int CompressFile(char *Infilename, char *Outfilename)
						-- output in both compression and decompression is now	
						internal, returned value is size of output				
						to obtain output itself call getOutput after each		
						compression and decompression							
						when you obtain output, please use finalize to deallocate
						allocated internal output. Do not deallocate output.							
						Call Finalize, when output is no longer needed. Do not	
						call Finalize on File compressions.						
						returned output size in file compression indicates, whether
						it was successfull (> 0)								
						-- allocation and deallocation (trees, nodes, allocatedoutput) should be correct now
						-- no allocation in loops give faster performance		
						now it should be possible to compress/decompress in loops
                      -- version changed to 3.0								

    UPDATE 25.8.2011  -- Header information shortened from 1280 MAX to 512  
                         MAX. One char now requires only 2B instead of 5B   
    UPDATE 29.10.2010 -- Self allocated output is now self deallocated:     
                         you must call Finalize() after EACH compress/decomp
    UPDATE 4.10.2010 -- Merged make tree procedures from compr & decompr to 
                        one procedure call: MakeTree()                      
                     -- removed few unnecessary conditions                  
    UPDATE 24.9.2010 -- removed 2 conditions: in cod/decod and in decod     
    UPDATE 24.9.2010 -- removed (int)0^ statements                          
    UPDATE 24.9.2010 -- speed up failed. Moved vars to registers            
    UPDATE 23.9.2010 -- added pointers instead of array indices in hope     
                        of speed up                                         
    UPDATE 30.8.2010 -- made class node private, also made private all      
                        attributes and functions of sumple_Huffman, but     
                        those, that should be private                       
    compressed buffer structure (sizes in bytes)                            
       [1] trees count                                                      
       trees (BYTES)                                                        
       [1] steps count                                                      
       steps (BYTES)                                                        
       [4] uncompressed size                                                
       [1-?] compressed stream of bits                                      
    Notes:                                                                  
       x program is fast                                                    
       x errors: none known                                                 
       x program works correctly                                            
*/
#pragma	 once
#include "stdafx.h"
#include  "stdio.h"
#include "string.h"
#include "math.h"
#include "resource.h"		// symboles principaux
#include "code_huffman.h"
#include "Cluster_Size.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TAILLE_TAMPON 65536

int simple_Huffman::comparefreq(const void *A, const void *B){ // sorting, we want to sort descending
	if((*(node**)A)->count == (*(node**)B)->count)     // A->count == B->count ?
		return 0;
	return (*(node**)A)->count < (*(node**)B)->count ? 1 : -1;// no? so if A->count < B->count give 1 else -1
}

// leaves initialization - set code and codelength to leaves
void simple_Huffman::InitializeLeaves(node *n, int path, int level){ // enter level is 0, enter path is 0
	bool leaf = true;
	if (n->right){      
		InitializeLeaves(n->right, path ^ (1 << level), level+1); // recursive part
		leaf = false;                                             
	}       
	if (n->left){
		InitializeLeaves(n->left, path, level+1);   // recursive part
		leaf = false;                               
	}  
	if (leaf){
		n->codelength = level;  // important in compression and decompression
		n->code = path;         
	}               
}
// for a givent tree find a better position
// used in compression, called from tryToPop
void simple_Huffman::moveTreesRight(node **toTree){
	node *a, *b;
	register node **ptr = trees+treescount-1;
	register node **_toTree = toTree;
	node **end = trees+treescount;
	while (ptr > _toTree){                
		a = *(ptr-1);        
		b = *(ptr);
		if (a->count >= b->count) {
			*(STEPS+stepscount) = treescount-(end-ptr);
			return;
		}
		*(ptr-1) = b;
		*(ptr) = a;
		ptr--;
	}  
	*(STEPS+stepscount) = treescount-(end-ptr);
}
// instead of re-sorting every iteration, just search for a better
// position for a given tree
// store info (step) when a position is found
// used in decompression
void simple_Huffman::moveTreesRight2(node **toTree){
	node *a, *b;
	register node **ptr = trees+treescount-1;
	register node **_toTree = toTree;
	while (ptr > _toTree){                
		a = *(ptr-1);        
		b = *(ptr);
		*(ptr-1) = b;
		*(ptr) = a;
		ptr--;
	}  
}
// try to move last tree to a better position
// used in compression
void simple_Huffman::tryToPop(){
	register node **ptr = trees+treescount-2;
	register node **ourtree = trees+treescount-1;
	register node **begin = trees;
	while(ptr >= begin){
		if (ourtree != ptr)
			if (((*ptr)->count > (*ourtree)->count) || (ptr == begin)){
				moveTreesRight(ptr);
				return;
			}      
			ptr--;
	}
}
// generate tree from sorted trees array
void simple_Huffman::MakeTree(){
	register node *n;
	register node **endtree = trees+treescount-1;//+255;  // pointer to end of trees
	memset(nodes, 0, sizeof(node)*256);
	node *nptr = nodes;
	node **backupptr = trees_backup+treescount-1;
	while (treescount > 1){ // while trees count is > than > 1
		n = nptr;//new node(); // allocate a new tree
		nptr++;
		// according to experiments, there is no really a reason to separate count
		// there is no difference if lesser will be left or right. Must be the same
		// always though

		n->right = *(endtree-1);
		n->left = *endtree;
		// references count from both trees (and their subtrees) are summed and assigned to new tree
		n->count = n->right->count + n->left->count;      

		*endtree = *backupptr;  // one tree goes "out", so its position will be assigned to
		// default default tree with 0 ref. count
		*(endtree-1) = n;                              
		treescount--;               // trees count is decreased by 1
		endtree--;  
		backupptr--;
		*(STEPS+stepscount) = treescount;

		// do not sort again, instead, try to move tree to a better pos forward
		// go from back to front and if pos is found, where is -1 > and +1 <, put it into +1
		// shift all + back
		tryToPop(); // try pop last tree a bit up
		stepscount++;
	}  
}

// maketree for decompression (no searching)
void simple_Huffman::MakeTreeD(){  
	node *n;
	node *nptr2 = nodes;
	node **backupptr;
	node **tptr = trees;  
	stepscount = 0;
	while (treescount > 1)  // merge trees until only 1 remains
	{
		n = nptr2;
		nptr2++;
		tptr = trees+treescount;
		backupptr = trees_backup+treescount;

		n->right = *(tptr-2);
		n->left = *(tptr-1);

		*(tptr-1) = *(backupptr-1);// NULL;//empty;  // one tree goes "out", so its position will be assigned to
		// default default tree with 0 ref. count
		*(tptr-2) = n;

		treescount--;   // decrease trees count by 1   

		moveTreesRight2(trees+(*(STEPS+stepscount)));  // do not qsort, instead move one last tree to new pos (if possible)
		stepscount++;
	}
}

// compress input, output is accessible thru getOutput()
// return value indicates allocated output size
// but only if return value is > 0
// if return value is = 0, then you can get the error code by
// calling getLastError()
// do not forget to call Finalize after each compression, when output is no longer
// needed
int simple_Huffman::Compress(BYTE *input, int inputlength){

	initialize();
	register BYTE *inptr = input;
	BYTE *stop = input+inputlength;  // stop - tam je koniec
	allocatedoutput = new BYTE[5*inputlength];
	if (allocatedoutput == NULL) { LastError = 3; return 0; }
	allocatedoutput = allocatedoutput;
	int byteswritten = 0;
	register BYTE *outptrX = allocatedoutput; 

	//get the individ. characters count

	while (inptr != stop){

		(*(trees+*inptr))->count++;
		(*(trees+*inptr))->chr = (*inptr);

		inptr++;
	}

	qsort(trees, 256, sizeof(node*),  comparefreq); // sort by references count

	for (register int i = 0; i < 256; i++) // get the trees count
		if ((*(trees+i))->count > 0)
			treescount++;      
		else break; 

		(*outptrX) = treescount-1; // write down trees count to 1 BYTE so, that 1=0, 256 = 255
		outptrX++;
		for (register int i = 0; i < treescount; i++){
			*outptrX = (*(trees+i))->chr;
			outptrX++;
		}
		MakeTree();

		// write steps
		*outptrX = stepscount;
		outptrX++;
		for (register int i = 0; i < stepscount; i++){
			*outptrX = *(STEPS+i);
			outptrX++;
		}

		*outptrX     =  inputlength >> 24;  // write original stream's length
		*(outptrX+1) =  inputlength >> 16;
		*(outptrX+2) =  inputlength >> 8;
		*(outptrX+3) =  inputlength;
		outptrX+=4;

		// merge all chars trees into one big (Huffman) tree, on pos. 0 will be this one main tree
		InitializeLeaves(*trees, 0,0); // initialize leaves (for writing down the ref. count table & compression)

		// compression

		int bitpath = 0;
		int bitswritten = 0;
		int outbitswritten = 0;
		register int i, bitcount;

		inptr = input;  
		*outptrX = 0;
		while (inptr != stop){
			bitpath = (*(leaves+*inptr))->code;
			bitcount = (*(leaves+*inptr))->codelength;
			bitswritten =0;
			for (i = 0; i < bitcount; i++){
				// bits are written right to left, from LSB to MSB
				// when byte is filled up, it cames to next one
				(*outptrX) ^= ((bitpath >> bitswritten)&1) << outbitswritten;
				bitswritten++;
				(*(outptrX+1)) = '\0';
				outptrX += ((outbitswritten >> 2) & (outbitswritten >> 1) & outbitswritten) &1; // 0..6 =0,  7=1
				outbitswritten++; // bytes written growing with each 8 written bits ^^
				outbitswritten&=7;

			}
			inptr++;
		}

		byteswritten = outptrX - allocatedoutput;
		if (bitswritten > 0) // if necessary, increase the bytes written
			byteswritten++;   // e.g. if written 1 bit, even 1 bit needs whole byte  
		performInternalCleanup();
		return byteswritten;   
}

// decompress input, output is accessible thru getOutput()
// return value indicates allocated output size
// but only if return value is > 0
// if return value is = 0, then you can get the error code by
// calling getLastError()
// do not forget to call Finalize after each decompression, when output is no longer
// needed
int simple_Huffman::Decompress(BYTE *input, int inputlength)
{      

	initialize();
	BYTE *stop = input + inputlength;
	register BYTE *inptr = input;
	// node *n = nullptr;
	treescount = *inptr;  // najprv nacitame treecount  
	inptr++;
	treescount++; // trees count is always +1
	node **tptr = trees;  

	for (register int i = 0; i < treescount; i++)
	{  
		(*tptr)->chr = *inptr;
		inptr++; // go forward
		tptr++;         
	}

	stepscount = *inptr;
	inptr++;
	int *sptr = STEPS;
	for (register int i = 0; i < stepscount; i++){
		(*sptr) = *inptr;
		inptr++;    
		sptr++;
	}

	// read orig. stream size
	int outsize;
	outsize  = *inptr << 24;  // read uncompressed stream s.
	outsize ^= *(inptr+1) << 16;
	outsize ^= *(inptr+2) << 8;
	outsize ^= *(inptr+3);
	inptr+=4;

	allocatedoutput = new BYTE[outsize+10]; // allocate output
	if (allocatedoutput == NULL) {performInternalCleanup(); LastError = 3; return 0;}

	MakeTreeD();

	InitializeLeaves(*trees, 0,0);  // initialize leaves - set their codes and code lengths

	register BYTE *outptr = allocatedoutput;
	int bit = 0;
	register node *nptr ;
	register int b;
	// int outbitswritten = 0;
	while(inptr <= stop){  // decompress
		nptr = *trees; // root
		while(nptr->codelength == 0){
			b = ((*inptr) >> bit) &1;
			nptr = (b > 0) ? nptr->right :  nptr->left;
			inptr+=( (bit >> 2) & (bit >> 1) & bit) & 1;
			bit++;        
			bit&=7;
		}  
		(*outptr) = nptr->chr;      
		outptr ++;
	}
	performInternalCleanup();
	return outsize;
}   
// gets allocated output
// call after SUCCESSFULL stream compression/decompression
BYTE *simple_Huffman::getOutput(){
	if (allocatedoutput)
		return allocatedoutput;
	return NULL;
}

// compress file
// return value indicates final file size
// but only if return value is > 0
// if return value is = 0, then you can get the error code by
// calling getLastError()
int simple_Huffman::CompressFile(char *input, char *output){
	FILE* in;
	DWORD Size_Cluster_in = 10;
	DWORD Size_Cluster_out = 10;

	fopen_s(&in, input, "rb");
	if (!in) { LastError = 1; return (FALSE); }
	FILE* out;

	fopen_s(&out, output, "wb");
	if (!out) {fclose(in); LastError = 2; return (FALSE);}

	Cluster_Size *pt_Cluster = new Cluster_Size;
	Size_Cluster_in = pt_Cluster->Getx8((CString)input);
	Size_Cluster_out = pt_Cluster->Getx8((CString)output);
	delete pt_Cluster;

	char *inbuffer = new char[Size_Cluster_in];
	*inbuffer = 0x00;
	char *outbuffer = new char[Size_Cluster_out];
	*outbuffer = 0x00;
	setvbuf(in,inbuffer, _IOFBF, Size_Cluster_in);
	setvbuf(out,outbuffer, _IOFBF, Size_Cluster_out);
	*inbuffer = 0x00;
	initialize();

	fseek(in, 0L, SEEK_END);
	int insize = ftell(in);
	int c;
	int out_char;
	fseek(in, 0L, SEEK_SET);  // seek begin

	//get the individ. characters count

	for (register int i = 0; i < insize; i++){
		c = fgetc(in);
		(*(trees+c))->count++;
		(*(trees+c))->chr = c;
	}
	fseek(in, 0L, SEEK_SET);  // seek begin once again

	qsort(trees, 256, sizeof(node*),  comparefreq); // sort by references count

	for (register int i = 0; i < 256; i++) // get the trees count
		if ((*(trees+i))->count > 0)
			treescount++;      
		else break; 

		fputc(treescount-1, out); // write down trees count to 1 BYTE so, that 1=0, 256 = 255
		for (register int i = 0; i < treescount; i++){
			fputc((*(trees+i))->chr, out);
		}

		MakeTree();

		fputc(stepscount, out);
		// zapis stepsy
		for (register int i = 0; i < stepscount; i++){
			fputc(*(STEPS+i), out);
		}

		fputc(insize >> 24, out);  // write original stream's length
		fputc(insize >> 16, out);
		fputc(insize >> 8, out);
		fputc(insize, out);
		// merge all chars trees into one big (Huffman) tree, on pos. 0 will be this one main tree
		InitializeLeaves(*trees, 0,0); // initialize leaves (for writing down the ref. count table & compression)

		// compression

		int bitpath = 0;
		int bitswritten = 0;
		int outbitswritten = 0;
		int byteswritten = 0;
		register int i, bitcount;
		out_char = 0;
		for (int X = 0; X < insize; X++){
			c = fgetc(in);
			bitpath = (*(leaves+c))->code;
			bitcount = (*(leaves+c))->codelength;
			bitswritten =0;
			for (i = 0; i < bitcount; i++){
				// bits are written right to left, from LSB to MSB
				// when byte is filled up, it cames to next one
				out_char ^= ((bitpath >> bitswritten)&1) << outbitswritten;
				bitswritten++;
				if (outbitswritten == 7){
					fputc(out_char, out);
					out_char = 0;
				}
				outbitswritten++; // bytes written growing with each 8 written bits ^^
				outbitswritten&=7;

			}
		}

		byteswritten = ftell(out);
		if (bitswritten > 0){ // if necessary, increase the bytes written
			byteswritten++;   // e.g. if written 1 bit, even 1 bit needs whole byte  
			fputc(out_char, out);
			byteswritten++;
		}
		fclose(in);
		fclose(out);

		performInternalCleanup();
		delete [] inbuffer;
		delete [] outbuffer;
		return byteswritten;   

}

// decompress file
// return value indicates final file size
// but only if return value is > 0
// if return value is = 0, then you can get the error code by
// calling getLastError()

int simple_Huffman::DecompressFile(char *input, char *output){
	FILE *in ;
	FILE *out ;
	int insize;
	DWORD Size_Cluster_in = 0;
	DWORD Size_Cluster_out = 0;

	fopen_s(&in, input, "rb");

	if (!in) 
	{
	  LastError = 1; 
	  return (FALSE);
	}

	fopen_s(&out, output, "wb");

	if (!out)
	 {   fclose(in);
		 LastError = 2; return (FALSE);
	 }

	Cluster_Size *pt_Cluster = new Cluster_Size;
	Size_Cluster_in = pt_Cluster->Getx8((CString)input);
	Size_Cluster_out = pt_Cluster->Getx8((CString)output);
	delete pt_Cluster;

	char *inbuffer = new char[Size_Cluster_in];
	*inbuffer = 0x00;
	char *outbuffer = new char[Size_Cluster_out];
	*outbuffer = 0x00;
	setvbuf(in,inbuffer, _IOFBF, Size_Cluster_in);
	setvbuf(out,outbuffer, _IOFBF, Size_Cluster_out);

	initialize();

	// node *n = nullptr;
	fseek(in, 0L, SEEK_END);
	insize = ftell(in);
	fseek(in, 0L, SEEK_SET);
	treescount = fgetc(in);  // najprv nacitame treecount  
	treescount++; // trees count is always +1
	node **tptr = trees;  

	for (register int i = 0; i < treescount; i++)
	{  
		(*tptr)->chr = fgetc(in);
		tptr++;         
	}

	stepscount = fgetc(in);
	int *sptr = STEPS;
	for (register int i = 0; i < stepscount; i++){
		(*sptr) = fgetc(in);
		sptr++;
	}

	// read orig. stream size
	int outsize;
	outsize  = fgetc(in) << 24;  // read uncompressed stream s.
	outsize ^= fgetc(in) << 16;
	outsize ^= fgetc(in) << 8;
	outsize ^= fgetc(in);

	MakeTreeD();
	InitializeLeaves(*trees, 0,0);  // initialize leaves - set their codes and code lengths

	int bit = 0;
	register node *nptr ;
	register int b;
	// int outbitswritten = 0;
	BYTE in_chr = fgetc(in);
	int position = 0;
	while (position < outsize){
		nptr = *trees; // root

		while(nptr->codelength == 0){
			b = ((in_chr) >> bit) &1;
			nptr = (b > 0) ? nptr->right :  nptr->left;
			if (bit == 7){
				in_chr = fgetc(in);
			}
			bit++;        
			bit&=7;
		}  
		fputc(nptr->chr, out);
		position++;
	}

	fclose(in);
	fclose(out);
	delete [] inbuffer;
	delete [] outbuffer;
	// perform cleanup
	performInternalCleanup();
	return position;
}
void simple_Huffman::performInternalCleanup(){
	for (register int i = 0; i < 256; i++) {
		memset(*(trees_backup+i), 0, sizeof(node));
		*(trees+i) = *(trees_backup+i);
		delete trees_backup[i];
		*(leaves+i) = *(trees+i);
		// delete trees[i];

	}

	memset(nodes, 0, 256*sizeof(node));

	memset(STEPS, 0, 256*sizeof(int));
	stepscount = 0;
	treescount = 0;	

}
void simple_Huffman::Finalize()
{
	if (allocatedoutput)
		delete allocatedoutput;
	allocatedoutput = NULL; 
}

void simple_Huffman::initialize() {
	stepscount = 0;      
	treescount = 0;
	LastError = 0;
	for (register int i = 0; i < 256; i++) {
		*(trees_backup+i) = *(trees+i);
		*(leaves+i) = *(trees+i);
		*(STEPS+i)  = 0;
	}
	memset(nodes, 0, 256*sizeof(node));
	if (allocatedoutput) delete allocatedoutput;
	allocatedoutput = NULL;
}

simple_Huffman::~simple_Huffman()
{
	Finalize();
}

simple_Huffman::simple_Huffman()
{
	CString Ref = "Compiled on " __DATE__ " at " __TIME__;
	CString File_Source = "File: " __FILE__;
	LastError = 0;
	for (register int i = 0; i < 256; i++) 
		*(trees+i)  = new node();
	allocatedoutput = NULL;
}

// get last error related to compression/decompression
// 0 - no errors, 1 - input file open fail, 2 - output file open fail, 3 - output not allocated
int simple_Huffman::getLastError(){
	return LastError;
}