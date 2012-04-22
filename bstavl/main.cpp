#include "util.hpp"
#include "main.hpp"
#include "BST.hpp"
#include "AVL.hpp"

bool DisplayFlag = false;
bool AVLFlag = false;
/* 
 *    USAGE:
 *    hw3 bst [-displayall] [file]
 *    hw3 bst_delete [-displayall] string1 string2 ... stringN file
 *    hw3 avl [-displayall] [file]
 *    hw3 avl_delete [-displayall] string1 string2 ... stringN file
 *
**/
int main (int argc, char *argv[]) {

	if(argc<2) {
		fprintf(stderr,"(malformed command)\n");
		GUsage();
	}
	
	if(	!strcmp(argv[1],"bst") ) {
		#ifdef DEBUG1
			fprintf(stdout,"----------BST----------------------\n");
		#endif
		BST(argc,argv,0); ///<int operation = 0 = INSERT
		return 1;
	}

	if(!strcmp(argv[1],"bst_delete") ) {
		#ifdef DEBUG1
			fprintf(stdout,"----------BST-------------\n");
		#endif 
		BST(argc,argv,1); ///< int operation = 1 = DELETE
		return 1;
	}
	
	if(	!strcmp(argv[1],"avl") ) {
		#ifdef DEBUG1
			fprintf(stdout,"----------AVL----------------------\n");
		#endif
		AVL(argc,argv,0);
		return 1;
	}

	if(!strcmp(argv[1],"avl_delete") ) {
		#ifdef DEBUG1
			fprintf(stdout,"----------AVL-------------\n");
		#endif 
		AVL(argc,argv,1);		///< 1 is delete operation
		return 1;
	}
	///< TEST THE TREES
	#ifdef DEBUG0
	if(!strcmp(argv[argc-1],"-LTest")) {
		int choice;
		cout<<"Enter some test case"<<endl;
		cin>>choice;
		switch(choice)
		{
			//case 1: cout<<"Test:CreateList and print"<<endl;test1(); break;//Just a createlist and print
			default:cout<<"please enter a valid testcase number"<<endl;break;
		}
		return 1;
	}
	#endif
	fprintf(stderr, "(malformed command)\n");
	GUsage();
	return 1;
}

/* 
 *    USAGE:
 *    hw3 bst [-displayall] [file]
 *    hw3 bst_delete [-displayall] string1 string2 ... stringN file
 *    hw3 avl [-displayall] [file]
 *    hw3 avl_delete [-displayall] string1 string2 ... stringN file
 *
**/
void GUsage() {
	fprintf(stderr,"Usage:\n");
	fprintf(stderr,"\thw3 bst [-displayall] [file] \n");
	fprintf(stderr,"\thw3 bst_delete [-displayall] string1 string2 ... stringN file\n");
	fprintf(stderr,"\thw3 avl [-displayall] [file] \n");
	fprintf(stderr,"\thw3 avl_delete [-displayall] string1 string2 ... stringN file\n");
	exit(-1);
}

bool isDisplaySet(int argc, char* argv[]) {

	bool flag = false;
	for(int i=0;i<argc;i++ ) {
		if(!strcmp(argv[i],"-displayall")) {
			flag = true;
			return flag;
		}
	}
	return flag;
}



void processError_lessargs(int argc, char* argv[]) {
	/////ERROR CONDITIONS
	if(DisplayFlag) {
		if(argc==3) {
			fprintf(stderr,"(malformed command)\n");
			fprintf(stderr,"No Input file specified for TREE\n");
			fprintf(stderr,"Atleast 1 string should be specified for deletion\n");
			exit(-1);
		}
		if(argc==4) {
			fprintf(stderr,"(malformed command)\n");
			fprintf(stderr,"Atleast 1 string should be specified for deletion\n");
			exit(-1);
		}

	} else {
		if(argc==2) {
			fprintf(stderr,"(malformed command)\n");
			fprintf(stderr,"No Input file specified for TREE\n");
			fprintf(stderr,"Atleast 1 string should be specified for deletion\n");
			exit(-1);
		}
		if(argc==3) {
			fprintf(stderr,"(malformed command)\n");
			fprintf(stderr,"Atleast 1 string should be specified for deletion\n");
			exit(-1);
		}
	}
}

/******************************************************TESTING LISTS***********************************************************************/
#ifdef DEBUG0

#endif
