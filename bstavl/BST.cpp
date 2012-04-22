#include "BST.hpp"
#include "util.hpp"
#include "my570BST.hpp"
#include "AVL.hpp"


extern bool DisplayFlag;



/**
 * @param1: argc is of type integer and represents the num of arguments
 * @param2: argv[][] is the array of cmd line params as strings
 * @param3: operation is of value 0:Insertion operation or 1:Deletion operation
**/
void BST(int argc, char* argv[],int operation) {

	processBSTArguments(argc,argv,operation);

}

/**
 * This function handles any error conditions in the command line arguments
 * USAGE:
 * hw3 bst [-displayall] [file]
 * hw3 bst_delete [-displayall] string1 string2 ... stringN file
 * @param1: argc is of type integer and represents the num of arguments
 * @param2: argv[][] is the array of cmd line params as strings
 * @param3: operation is of value 0:Insertion operation or 1:Deletion operation
 * exits if there are any errors  
**/
void processBSTArguments(int argc,char* argv[],int operation) {

	if(isDisplaySet(argc,argv)) {
		DisplayFlag = true;
		#ifdef DEBUG1
		fprintf(stdout,"DISPLAY FLAG: SET\n");
		#endif
	}
	else {
		#ifdef DEBUG1 
		fprintf(stdout,"DISPLAY FLAG: NOT SET\n");
		#endif	
	}
	//  hw3 bst [-displayall] [file]
    //  hw3 bst_delete [-displayall] string1 string2 ... stringN file
	switch(operation) {
		case 0: ///< BST Insert operation 
				#ifdef DEBUG1 
				fprintf(stdout,"Operation:Insertion\n");
				#endif		
				BST_Insertion(argc,argv);
				
				break;
		case 1: ///< BST Delete operation
				#ifdef DEBUG1
				fprintf(stdout,"Operation:Deletion\n");
				#endif
				BST_Deletion(argc,argv);
			    break;
		default: ///< This CASE SHOULD NEVER HAPPEN!!!!
				fprintf(stderr, "Unknown case for BST\n");
				exit(-1);
				break;
	}
}//end of processBSTArguments

/**
 *	bst	  			:  	Build a BST from the input by inserting the keys in order then display the resulting BST.
 *	bst_delete	  	:  	Build a BST from file by inserting the keys in order then delete nodes with keys string1, string2, ...,
 *						stringN from it and display the resulting BST after each node is deleted.
 *	avl	  			:  	Build an AVL Tree from the input by inserting the keys in order and display the resulting AVL Tree.
 *	avl_delete	 	:  	Build an AVL Tree from file by inserting the keys in order then delete nodes with keys string1, string2, 
 *						..., stringN from it and display the resulting AVL Tree after each node is deleted.
**/
/**
 *    hw3 bst [-displayall] [file]
 
   case1: hw3 bst									===stdin		 2
   case2: hw3 bst filename							===file 		 3
   case3: hw3 bst filename -displayall				===file	 		 4
   case4: hw3 bst -displayall filename				===file   argc = 4
   case5: hw3 bst -displayall						===stdin  argc = 3
   
*/
void BST_Insertion(int argc,char* argv[]){
	if(DisplayFlag) {
		//case5: hw3 bst -displayall	
		if(argc==3) { 
			BSTInsertfromStdin();
			return;
		}
		//   case4: hw3 bst -displayall filename			
		if(argc==4) {
			if(!strcmp(argv[3],"-displayall") ){
				//case3: hw3 bst filename -displayall	
				BSTInsertfromFile(argv[2]);		///<argv[2] should be a filename; argv[3] is -displayall
			} else {
				//case4: hw3 bst -displayall filename
				BSTInsertfromFile(argv[3]);		///<argv[3] should be a filename;
			}
			return;
		}
	} else {
		if(argc==2) {
			BSTInsertfromStdin();
			return;
		}
		if(argc==3) {
			///hw3 bst filename
			BSTInsertfromFile(argv[2]);		///<argv[2] should be a filename;
			return;		
		}
	}
	fprintf(stderr,"(malformed command)\n");
	BSTUsage();
}



void BSTInsertfromStdin() {
	#ifdef DEBUG1
	fprintf(stdout,"BSTInsertfromStdin()\n");
	#endif
	ProcessBSTInsertion(cin);

}

void BSTInsertfromFile(char *filename) {

	#ifdef DEBUG1
	fprintf(stdout,"BSTInsertfromFile(char *filename):Filename:%s\n",filename);
	#endif

	ifstream in;
	ProcessFilenameFailure(filename);
	in.open(filename);
	if (in.fail()) {
		fprintf(stderr, "(Cannot open %s) \n",filename);
		exit(-1);
	}
	if(isDir(filename)) {
		fprintf(stderr,"(input file: %s is a directory)\n",filename);
		exit(-1);
	}
	ProcessBSTInsertion(in);
	in.close();
}


/*** AT this point everything is fine and you can start processing!!! Insertion 
 * Returns a pointer to the TREE with given stream of inputs.
 * Takes a stream file or stdin
**/
My570BST* ProcessBSTInsertion(istream& in) {

	int count = 0;
	char* buf = new char[MAXBUFSIZE];
	in.getline(buf,MAXBUFSIZE);

	My570BST *BST=new My570BST();					///< CREATING TREE
	while (!in.eof()) {
	
		if(strlen(buf)>1000) {  
			fprintf(stderr,"Input Key size exceeds 1000 characters\n");
			exit(-1);
		}
		if(strlen(buf)==0) {  
			#ifdef DEBUG1
			fprintf(stdout,"Found Empty line\n");
			#endif
			buf = new char[MAXBUFSIZE];
			in.getline(buf,MAXBUFSIZE);		
			continue;
		}
		/////TRIM THE STRING//////////
		myTrim(buf);								///< trim trailing and leading whitespaces 
		//////////////////////////////
		if(strlen(buf)==0) {
			#ifdef DEBUG2 
			fprintf(stdout,"String became empty after trimming\n");
			fprintf(stdout,"%s\n",buf);				///< PRINT THE RAW BUFFER
			#endif
			buf = new char[MAXBUFSIZE];
			in.getline(buf,MAXBUFSIZE);		
			continue;
		}
		#ifdef DEBUG1
			fprintf(stdout,"My570BST* ProcessBSTInsertion(istream& in): %s\n",buf);				///< PRINT THE RAW BUFFER
		#endif 
		//////INSERTION CODE HERE ////<<<<... 
		(void)BST->InsertNode((void*)buf);
		count++;
		if(DisplayFlag){
			fprintf(stdout,"Inserted \"%s\" (node %d):\n\n",buf,count);									///< PRINT SUCCESS OF INSERTION TO STDOUT
		}
		buf = new char[MAXBUFSIZE];
		in.getline(buf,MAXBUFSIZE);					///< GET THE NEXT LINE
		
		if(DisplayFlag){
			///< this check is made since the last display tree doesnt need any newlines
			if(!in.eof()) {
				BST->DisplayTree();
				fprintf(stdout,"\n");
			} else {
				BST->DisplayTree();
			} 
		}
	}//end of while

	if(!DisplayFlag){
		BST->DisplayTree();
	}
	
	#ifdef DEBUG1
		fprintf(stdout,"EOF REACHED\n");
		fprintf(stdout,"total number of nodes in the tree = %d\n",count);
	#endif

return BST;
}








// ================================================BST DELETION ============================================================= //

// ================================================BST DELETION ============================================================= //

// ================================================BST DELETION ============================================================= //

// ================================================BST DELETION ============================================================= //

// ================================================BST DELETION ============================================================= //

// ================================================BST DELETION ============================================================= //

// ================================================BST DELETION ============================================================= //

// ================================================BST DELETION ============================================================= //

/**
 *	bst	  			:  	Build a BST from the input by inserting the keys in order then display the resulting BST.
 *	bst_delete	  	:  	Build a BST from file by inserting the keys in order then delete nodes with keys string1, string2, ...,
 *						stringN from it and display the resulting BST after each node is deleted.
 *	avl	  			:  	Build an AVL Tree from the input by inserting the keys in order and display the resulting AVL Tree.
 *	avl_delete	 	:  	Build an AVL Tree from file by inserting the keys in order then delete nodes with keys string1, string2, 
 *						..., stringN from it and display the resulting AVL Tree after each node is deleted.
**/
/*
 *    hw3 bst_delete [-displayall] string1 string2 ... stringN file

*/
void BST_Deletion(int argc,char* argv[]){
	processError_lessargs(argc,argv); // we exit if this fails inside.
	#ifdef DEBUG1
	fprintf(stdout,"BST_Deletion(argc,argv):Processings starts\n");
	#endif
	/////REMEMBER THAT -displayall can come anywhere.. so dont assume that file is the argc-1 th argument
	/***THIS IS NEEDED FOR DELETION STRINGS! I think this is taken care of ?
	if(count<1) {
		fprintf(stderr,"Atleast one string needs to be specified for deletion N>=1\n");
		exit(-1);
	}
	**/
	char * filename;
	int endpoint;
	if(DisplayFlag) {
		if(strcmp(argv[argc-1],"-displayall")==0) {
			filename = argv[argc-2];
			endpoint = argc-2;
		} else {
			filename =argv[argc-1];
			endpoint = argc-1;
		}
	} else {
		filename =argv[argc-1];
		endpoint = argc-1;
	}

	
	char deletionstrArr[400][MAXBUFSIZE];
	int numofdelstr=0;
	for(int i=2;i<endpoint;i++ ) {
		if( strcmp(argv[i],"-displayall")!=0) {
			strcpy(deletionstrArr[numofdelstr],argv[i]);
			if(strlen(deletionstrArr[numofdelstr])>1000) { ///< exit if a string exceeds 1000 chars.
				fprintf(stderr,"One of the deletion string exceeds 1000 characters\n");
				exit(-1);
			}
			numofdelstr++;
		}
	}
	
	
	///< At this point, DisplayFlag tells if -displayall is set
	///<				deletionstrArr[][] contains all the deletion strs;
	///< 				filename contains the filename of inputs.
	BSTDeletionFileInput(filename,deletionstrArr,numofdelstr);
}


void BSTDeletionFileInput(char *filename,char deletionstrArr[][MAXBUFSIZE],int numofdelstr) {

	#ifdef DEBUG1
		fprintf(stdout,"BSTDeletionFileInput::filename:%s\n",filename);
		fprintf(stdout,"BSTDeletionFileInput::Deletion strings are :\n");
		for(int i=0; i<numofdelstr;i++) {
			fprintf(stdout,"%s\n",deletionstrArr[i]);
		}
	#endif

	ifstream in;
	ProcessFilenameFailure(filename);
	in.open(filename);
	if (in.fail()) {
		fprintf(stderr, "(Cannot open %s) \n",filename);
		exit(-1);
	}
	if(isDir(filename)) {
		fprintf(stderr,"(input file: %s is a directory)\n",filename);
		exit(-1);
	}
	My570BST *BST = ProcessBSTDeletion(in); //BUILD BST
	//BST->DisplayTree();
	in.close();
	
	for(int i =0;i<numofdelstr;i++) {
		(void)BST->DeleteNode((void*)deletionstrArr[i]);
		fprintf(stdout,"Deleted \"%s\":\n\n",deletionstrArr[i]);
		BST->DisplayTree();
		if(i!=numofdelstr-1)
			fprintf(stdout,"\n");
	}
	
}


/*** AT this point everything is fine and you can start processing!!! Insertion 
 * Returns a pointer to the TREE with given stream of inputs.
 * Takes a stream file or stdin
**/
My570BST* ProcessBSTDeletion(istream& in) {

	int count = 0;
	char* buf = new char[MAXBUFSIZE];
	in.getline(buf,MAXBUFSIZE);

	My570BST *BST=new My570BST();					///< CREATING TREE
	while (!in.eof()) {
	
		if(strlen(buf)>1000) {  
			fprintf(stderr,"Input Key size exceeds 1000 characters\n");
			exit(-1);
		}
		if(strlen(buf)==0) {  
			#ifdef DEBUG1
			fprintf(stdout,"Found Empty line\n");
			#endif												///<ProcessBSTDeletion
			buf = new char[MAXBUFSIZE];
			in.getline(buf,MAXBUFSIZE);		
			continue;
		}
		/////TRIM THE STRING//////////
		myTrim(buf);								///< trim trailing and leading whitespaces 
		//////////////////////////////
		if(strlen(buf)==0) {								////<ProcessBSTDeletion
			#ifdef DEBUG2 
			fprintf(stdout,"String became empty after trimming\n");
			fprintf(stdout,"%s\n",buf);				///< PRINT THE RAW BUFFER
			#endif
			buf = new char[MAXBUFSIZE];
			in.getline(buf,MAXBUFSIZE);		
			continue;
		}
		#ifdef DEBUG1
			fprintf(stdout,"My570BST* ProcessBSTInsertion(istream& in): %s\n",buf);				///< PRINT THE RAW BUFFER
		#endif 
		//////INSERTION CODE HERE ////<<<<... 
		(void)BST->InsertNode((void*)buf);
		count++;
		if(DisplayFlag){
			fprintf(stdout,"Inserted \"%s\" (node %d):\n",buf,count);									///< PRINT SUCCESS OF INSERTION TO STDOUT
		}
		buf = new char[MAXBUFSIZE];
		in.getline(buf,MAXBUFSIZE);					///< GET THE NEXT LINE
		
		if(DisplayFlag){
			///< this check is made since the last display tree doesnt need any newlines
				fprintf(stdout,"\n");
				BST->DisplayTree();
				fprintf(stdout,"\n");
		}
	}//end of while

   
	if(!DisplayFlag){
		BST->DisplayTree();
		fprintf(stdout,"\n");
	}
	
	
	
	#ifdef DEBUG1
		fprintf(stdout,"EOF REACHED\n");
		fprintf(stdout,"total number of nodes in the tree = %d\n",count);
	#endif

return BST;
}






void BSTUsage() {
	fprintf(stderr,"Usage:\n");
	fprintf(stderr,"\thw3 bst [-displayall] [file] \n");
	fprintf(stderr,"\thw3 bst_delete [-displayall] string1 string2 ... stringN file\n");
	exit(-1);
}









