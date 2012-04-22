#include "sudokugen.hpp"
#include "sudokusolve.hpp"
#include "sudokucount.hpp"
#include "util.hpp"
#include "my570list.hpp"

using namespace std;  


extern char* file;
extern bool isInfoSet;

extern char in_sudoku[MAXROW][MAXCOL];
extern int in_sudoku_int[MAXROW][MAXCOL];
extern int out_sudoku_int[MAXROW][MAXCOL];
FILE* fptr=NULL;
FILE* fptr2 = NULL;
My570List *list[MAXROW];
My570List *list2;
int numofsolutions_gen = 0;

void sudokugen(int argc, char* argv[]){
	processcmdline_gen(argc,argv);
	processFile_rndfile();
	initialize_solved_puzzle();
	#ifdef DEBUG4
	print_int_sudoku(in_sudoku_int);
	#endif
	OnRandomShuffle();	
	fprintf(stdout,"Full board generated:\n");
	BoardGeneration(in_sudoku_int);
	fprintf(stdout,"\n");
	print_int_sudoku(in_sudoku_int);
	GenerateRegularSudoku();
	//print_int_sudoku(in_sudoku_int);
	//displaysudoku(in_sudoku);
	
	
	#ifdef DEBUG4
	fprintf(stdout,"-info=%d\n",isInfoSet);
	if(file==NULL){
		fprintf(stdout,"rndfile:Stdin\n");
	}else{
		fprintf(stdout,"rndfile:%s\n",file);
	}
	#endif
}

void displaylist(My570List* templist) {
		My570ListElem *elem =NULL;
		for (elem=templist->First(); elem != NULL; elem=templist->Next(elem)) {
				fprintf(stdout,"%d ",(int)elem->Obj());
		}
		fprintf(stdout,"\n");
}
void GenerateRegularSudoku() {
	//push all solved solution into a list. in row major order.
	list2=new My570List();
	for(int i=0;i<MAXROW;i++) {
		for (int j=0;j<MAXCOL;j++) {
				CELL *c = new struct Cell;
				c->value = in_sudoku_int[i][j];
				c->row = i;
				c->col = j;
				(void)list2->Append((void*)c);
		}
	}	
	int L = list2->Length();
	int numofunassignedvalues =0;
	if(isInfoSet){
		fprintf(stdout,"\n");
	}
	while(L!=0){
		int rnd = RandomIndex(L);		
		int count = rnd;
		My570ListElem *elem=NULL;
		for (elem=list2->First(),count=0; elem != NULL; elem=list2->Next(elem),count++){
			if(count==rnd)
				break;	
		}
		int currow = ((struct Cell*)(elem->Obj()))->row;
		int curcol = ((struct Cell*)(elem->Obj()))->col;		

		
		copymatrix(in_sudoku_int,out_sudoku_int);

		//print_int_sudoku(out_sudoku_int);
		in_sudoku_int[currow][curcol]=DOT;

		//print_int_sudoku(in_sudoku_int);
		solve_by_backtrack_gen(in_sudoku_int);
		if(numofsolutions_gen>1) {
			if(isInfoSet){
				fprintf(stdout,"Unassigning row %d column %d.  2 solutions found.\n",currow+1,curcol+1);
				fprintf(stdout,"Row %d column %d restored.\n",currow+1,curcol+1);	
			}
			fprintf(stdout,"\n");
			fprintf(stdout,"Solution (after %d values unassigned):\n\n",numofunassignedvalues);
			break;
		}
	
		if(isInfoSet)
			fprintf(stdout,"Unassigning row %d column %d...\n",currow+1,curcol+1);

		numofsolutions_gen = 0;
		numofunassignedvalues++;
		list2->Unlink(elem);
		L = list2->Length();
	}
	print_int_sudoku(out_sudoku_int);
}



bool solve_by_backtrack_gen(int input[][MAXCOL]) {
	int row, col;
	if(!FindFreeSlots(input,&row,&col)) {// meaning there are no free slots left meaning no free dots
		numofsolutions_gen++;
		/*if(isPrintallSet) {
			displaysudoku_int(in_sudoku_int);
			fprintf(stdout,"\n");
		}
		if(maxn==0) {
			AtleastOneSolutionWasFound=true;
		}
		if(numofsolutions_gen==maxn && maxn!=0) {
			return true;
		}*/
		return false;	
	}
	
	for(int num =1;num<=9;num++) { ///< iteratively choose one of the numbers from 1 to 9
		if(NoConflict(input,row,col, num)) { //go ahead and assign coz its ok!!
			input[row][col]=num;
			if(solve_by_backtrack_gen(input)) return true;
			input[row][col] = DOT;
		}
	}
	return false; ///meaning you did not find a suitable num and u do backtracking to earlier solution
	
}

void copymatrix(int src[][MAXCOL], int dest[][MAXCOL]) {
		for(int i=0;i<MAXROW;i++) {
			for(int j=0;j<MAXCOL;j++) {
				dest[i][j] = src[i][j];
			}
		}
}



int getRnd(int L) {
	#ifdef DEBUG0
	fprintf(stdout,"listlength:%d\n",L);
	#endif
	unsigned char buf[4] ;
	if(fread (buf,1,4,fptr)!=4){	
		if(file==NULL)
			fprintf(stderr,"Random input ran out of bytes\n"); 
		else
			fprintf(stderr,"Rnd input file: %s ran out of bytes\n",file);
		exit(-1); 
	}
	unsigned int rndnum=0;
	memcpy(&rndnum, buf, 4);
	int finalrndnum = rndnum % L;
	#ifdef DEBUG2
		fprintf(stdout,"[%d]\n",finalrndnum);
	#endif
	return finalrndnum;
}

int getRowofCell(int cellnum) {
	return (cellnum/MAXCOL); //81 is cell 72-80= = 9 63-71 = 7
}
int getColofCell(int cellnum) {
	return (cellnum%MAXROW);   //80%9 = 8  ; //79%9 = 7  
}

bool BoardGeneration(int input[][MAXCOL]) {
	int row, col;
	if(!FindFreeSlots(input,&row,&col)) {// meaning there are no free slots left meaning no free dots
		return true;
	}	
	
	My570ListElem *elem=NULL;
	for (elem=list[row]->First(); elem != NULL; elem=list[row]->Next(elem)){	
		int num = (int)elem->Obj();
		//fprintf(stdout,"%d ",num);
		if(NoConflict(input,row,col, num)) { //go ahead and assign coz its ok!!
			input[row][col]=num;
			if(BoardGeneration(input)) return true;
			input[row][col] = DOT;
		}
	}
	return false; ///meaning you did not find a suitable num and u do backtracking to earlier solution
	
}
void OnRandomShuffle() {
	

	for(int i=0;i<MAXROW;i++) {
		list[i]=new My570List();
		for(int j=0;j<MAXCOL;j++) {
			(void)list[i]->Append((void*)in_sudoku_int[i][j]);
		}
		list[i]=ShuffleList(list[i]);
	}
	if(isInfoSet) {
		fprintf(stdout,"Random shuffle results:\n\n");
	}
	int k=0;
	for(int i=0;i<MAXROW;i++) {
		My570ListElem *elem=NULL;
		int j=0;
		for (elem=list[i]->First(); elem != NULL; elem=list[i]->Next(elem)) {
			if(isInfoSet) {
				fprintf(stdout,"%d",(int)elem->Obj());
			}
			in_sudoku_int[i][j] = DOT;
			//updateChar();
			k++;
			j++;
		}
		if(isInfoSet) {
			fprintf(stdout,"\n");
		}
	}
	if(isInfoSet) {
		fprintf(stdout,"\n");
	}
}

My570List* ShuffleList(My570List *input_list){ 
	
	My570List *output_list = new My570List();
	while (!input_list->Empty()) {
       int L = input_list->Length();
       int R = RandomIndex(L);
       My570ListElem *E = getElem(input_list,R);
	   int obj = (int)E->Obj();
	   input_list->Unlink(E);
	   (void)output_list->Append((void*)obj);
   }
   delete input_list;
   return output_list;	
}
/*
The RandomIndex(L) function with L > 0 returns a 
value R such that 0 ≤ R < L using bytes from a source of random bytes (i.e, rndfile or stdin) as follows. 
First you read 4 bytes from the input to form a 32-bit unsigned integer X. 
The first byte you read goes into the most-significant byte of X, the 2nd byte
 you read goes into the 2nd most-significant byte of X, the 3rd byte you read 
 goes into the 3rd most-significant byte of X, and the 4th byte you read goes 
 into the least-significant byte of X. Then you calculate R = X % L where % is 
 the modular division operator (i.e., R is the remainder if you divide X by L).
 R should be used as a zero-based index to select the Rth element from 
the input list for removal. Please note that even if L = 1,
 you must still consume 4 bytes of data from the input.
*/
int RandomIndex(int L) {

	
	//int listlen = L;
	//listlen = list->Length();
	unsigned char buf[4] ;
	if(fread (buf,1,4,fptr)!=4){	
		if(file==NULL)
			fprintf(stderr,"Random input ran out of bytes\n"); 
		else
			fprintf(stderr,"Rnd input file: %s ran out of bytes\n",file);
		
		exit(-1); 
	}
	///////TODO: mark the ones removed as "has been inspected!!"
	unsigned int rndnum=0;
	memcpy(&rndnum, buf, 4);
	int finalrndnum = rndnum % L;
	#ifdef DEBUG3
		fprintf(stdout,"[%x]\n",rndnum);
	#endif
	return finalrndnum;
}

My570ListElem *getElem(My570List *list,int R){
	My570ListElem *elem = NULL;
	int count = 0;
	if(R<0 ||R>=list->Length()) {
		fprintf(stderr,"Random number has to be between 0 and Listlen : %d",list->Length());
		exit(-1);
	}
	for (elem=list->First(); elem != NULL; elem=list->Next(elem)) {
			if(count==R){
				return elem;
			}
			count++;
		}
	return NULL;
}
void print_int_sudoku(int arr[][MAXCOL]){
	int i=0;int j=0;
	for(i=0;i<MAXROW;i++) {
		if(i%3==0){
			fprintf(stdout,"+---+---+---+\n");
		}	
		for(j=0; j<MAXCOL;j++) {
		//fprintf(stdout,"%c",arr[i][j]);
			if(j%3==0){
				fprintf(stdout,"|");
			}
			if(arr[i][j]==DOT){
				fprintf(stdout,".");
			}else {
					fprintf(stdout,"%d",arr[i][j]);
			}
		}
		fprintf(stdout,"|\n");
	}
	fprintf(stdout,"+---+---+---+\n");
}
void initialize_solved_puzzle() {
	for(int i=0;i<MAXROW;i++) {
		int num = 1;
		for(int j=0;j<MAXCOL;j++) {
			in_sudoku_int[i][j] = num;
			in_sudoku[i][j] = getmychar(num);
			num++;
		}
	}
}
void updateChar() {
	for(int i=0;i<MAXROW;i++) {
		for(int j=0;j<MAXCOL;j++) {
			if(in_sudoku[i][j]==DOT) {
				in_sudoku_int[i][j]='.';
			}else {
				in_sudoku[i][j] = getmychar(in_sudoku_int[i][j] );
			}
		}
	}
}

void readFile_gen(istream& in){
	//return value is between 0<=R<L   
	//FILE* fptr;
	if(file==NULL) {
		fptr=stdin;
	} else { ///< a valid file name is specified
		fptr = fopen(file, "rb");
		if (fptr==0) {
			fprintf (stderr,"Unable to open Rndfile %s\n",file); 
			exit (-1);
		}
	}
}	


/*
    hw5 solve [file]
    hw5 count [-printall] [-max=n] [file]
    hw5 gen [-info] [rndfile]
*/

void processFile_rndfile() {

	if(file==NULL){
		readFile_gen(cin);///< read from standard input
	} else {
		ifstream in;
		ProcessFilenameFailure(file);
		in.open(file);
		if (in.fail()) {
			fprintf(stderr, "(Cannot open %s) \n",file);
			exit(-1);
		}
		if(isDir(file)) {
			fprintf(stderr,"(input file: %s is a directory)\n",file);
			exit(-1);
		}
		readFile_gen(in);
		in.close();		
	}
}
void sudokugenUsage() {
	fprintf(stderr,"Usage:\n");
	exit(-1);	
}

void processcmdline_gen(int argc, char* argv[]) {
	if(argc>4) {
		fprintf(stderr,"(malformed command) - too many arguments\n");
		sudokugenUsage();
	}
	if(argc==2) {
		isInfoSet = false;
		file = NULL;
		return;
	}
	if(argc<=4){
		int filecount = 0;
		for(int i=2;i<argc;i++) {
			if(isarganoption(argv[i])) {
				processoption_gen(argv[i]);
			} else {
				file = argv[i];
				filecount++;
			}
		}
		if(filecount>1) {
			fprintf(stderr,"(malformed command)\n");
			fprintf(stderr, "Error in arguments: Found more than 1 argument that looks like a rnd-filename\n");
			sudokugenUsage();
		}
	}
}

//function will update the top left right bottom
void processoption_gen(char* optarg) {
	if(!strcmp(optarg,"-info")) {
		isInfoSet = true;
		return ;
	}else{
		fprintf(stderr,"(malformed command)- unknown option/argument specified\n");
		exit(-1);
	}

}

