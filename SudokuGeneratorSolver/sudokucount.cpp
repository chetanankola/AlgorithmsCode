#include "sudokugen.hpp"
#include "sudokusolve.hpp"
#include "sudokucount.hpp"
#include "util.hpp"
#include "my570list.hpp"
#define MAXMAX -912799
using namespace std;  

extern char* file;
extern bool isPrintallSet;
extern bool ismaxSet;
extern int maxn; ///<default value
extern char linebuf[MAXBUFSIZE][MAXBUFSIZE]; 
extern int linebufcount;
extern char in_sudoku[MAXROW][MAXCOL];
extern int in_sudoku_int[MAXROW][MAXCOL];

int numofsolutions = 0;
bool AtleastOneSolutionWasFound = false;
/**
count:  	
Read the input and build a 9-by-9 sudoku puzzle. Solve the puzzle, 
backtrack through all solutions, print the total number of solutions. If the -printall 
commandline option is specified, print all the solutions. If the -max commandline option is specified, 
you should stop looking for solutions after n solutions have been found. If n is zero 
([BC: updated 11/16/2011] which is the default), you should explore all possible solutions to the specified puzzle.
*/

void sudokucount(int argc, char* argv[]) {
	
	processcmdline_count(argc,argv);
	processFile();
	readValidLines();
	checkvalidity(in_sudoku);
	copySudokuInputtoIntArr();
	if(maxn==MAXMAX) {
		maxn=0;
	}
	/*if(!solve_by_backtrack_count(in_sudoku_int) && !AtleastOneSolutionWasFound) {
		fprintf(stdout,"0 solutions found\n");
		exit(-1);
	}else {
		fprintf(stdout,"%d solutions found\n",numofsolutions);
	}*/
	solve_by_backtrack_count(in_sudoku_int);
	if(numofsolutions==1){
		fprintf(stdout,"%d solution found\n",numofsolutions);
	}else{
		fprintf(stdout,"%d solutions found\n",numofsolutions);
	}
	////////////////////////////////////////////////PRINT INFO////////////////////////////
	#ifdef DEBUG4
	fprintf(stdout,"-printall=%d\n",isPrintallSet);
	fprintf(stdout,"-max=%d\n",maxn);
	if(file==NULL){
		fprintf(stdout,"file:Stdin\n");
	}else{
		fprintf(stdout,"file:%s\n",file);
	}
	#endif
	//////////////////////////////////////////////////////////////////////////////////////
	//displaysudoku_int(in_sudoku_int);
}

bool solve_by_backtrack_count(int input[][MAXCOL]) {
	int row, col;
	if(!FindFreeSlots(input,&row,&col)) {// meaning there are no free slots left meaning no free dots
		numofsolutions++;
		if(isPrintallSet) {
			displaysudoku_int(in_sudoku_int);
			fprintf(stdout,"\n");
		}
		if(maxn==0) {
			AtleastOneSolutionWasFound=true;
		}
		if(numofsolutions==maxn && maxn!=0) {
			return true;
		}
		return false;
		
	}
	
	for(int num =1;num<=9;num++) { ///< iteratively choose one of the numbers from 1 to 9
		if(NoConflict(input,row,col, num)) { //go ahead and assign coz its ok!!
			input[row][col]=num;
			if(solve_by_backtrack_count(input)) return true;
			input[row][col] = DOT;
		}
	}
	return false; ///meaning you did not find a suitable num and u do backtracking to earlier solution
	
}

void processFile(){

	if(file==NULL){
		readFile_count(cin);///< read from standard input
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
		readFile_count(in);
		in.close();		
	}
}



/*
    hw5 solve [file]
    hw5 count [-printall] [-max=n] [file]
    hw5 gen [-info] [rndfile]
*/
void readFile_count(istream& in) {

	#ifdef DEBUG3
	if(file==NULL)
		fprintf(stdout, "read file from stdin\n");
	else
		fprintf(stdout, "read file from %s\n",file);
	#endif
	char* buf = new char[MAXBUFSIZE];
	in.getline(buf,MAXBUFSIZE);
	while (!in.eof()) {
		#ifdef DEBUG3
		fprintf(stdout,"line read: %s\n",buf);
		#endif
		if(linebufcount>=MAXBUFSIZE){
			fprintf(stderr,"(malformed Input) - input file %s is not a sudoku puzzle)\n",file);
			exit(-1);
		}
		strcpy(linebuf[linebufcount++],buf);
		in.getline(buf,MAXBUFSIZE);
	}
	#ifdef DEBUG3
	fprintf(stdout,"EOF REACHED\n");
	#endif
	if(linebufcount==1 || linebufcount==0 ||linebufcount<9) {
		fprintf(stderr,"(malformed input file):less than 9 rows of input..!!\n");
		exit(-1);
	}
}
void processcmdline_count(int argc, char* argv[]) {
	maxn = MAXMAX;	//initialize it to some junk to detect dual occurrence of the same option
	if(argc>5) {
		fprintf(stderr,"(malformed command) - too many arguments\n");
		sudokucountUsage();
	}
	if(argc==2) {
		maxn = 0;
		ismaxSet = false;
		isPrintallSet = false;
		file=NULL;
		return;
	}
	if(argc<=5){
		int filecount = 0;
		for(int i=2;i<argc;i++) {
			if(isarganoption(argv[i])) {
				processoption(argv[i]);
			} else {
				file = argv[i];
				filecount++;
			}
		}
		if(filecount>1) {
			fprintf(stderr,"(malformed command)\n");
			fprintf(stderr, "Error in arguments: Found more than 1 argument that looks like a filename\n");
			sudokucountUsage();
		}
	}
}


bool isarganoption(char * arg) {
	if(arg[0]=='-')
		return true;
	return false;
	
}
//function will update the top left right bottom
int processoption(char* optarg) {
	if(!strcmp(optarg,"-printall")) {
		isPrintallSet = true;
		return 1;
	}

	int len = strlen(optarg);
	char temp[10]; //-max=n
	if(len<6) {
		fprintf(stderr,"(malformed command) - unknown argument specified\n");
		exit(-1);	
	}
	
	int i=0;
	for(i=0;i<5;i++){
		temp[i]=optarg[i];
	}
	temp[i]= '\0';
	if(strcmp(temp,"-max=")!=0){
		fprintf(stderr,"(malformed command) - unknown argument specified\n");
		exit(-1);		
	}
	
	int k=0;
	char temp2[10];
	for(i=5;i<len;i++) {
		temp2[k] = optarg[i];
		if(!is_digit(optarg[i])) {
			fprintf(stderr,"(malformed command) - value for max should be a digit\n");
			exit(-1);
		}
		k++;
	}
	if(k==0){
		fprintf(stderr,"(malformed command) - no value specified for -max\n");
		exit(-1);	
	}
	if(maxn!=MAXMAX){
		fprintf(stderr,"(malformed command) -max options specified twice\n");
		exit(-1);				
	}else{
		maxn = atoi(temp2);
	}
	return 1;
}


void sudokucountUsage() {
	fprintf(stderr,"Usage:\n");
	fprintf(stderr,"hw5 count [-printall] [-max=n] [file]\n");
	exit(-1);	
}


///< TODO: check the maximum maxn VALUE FOR N and check it..
