#include <vector>
#include <queue>
#include "sudokugen.hpp"
#include "sudokusolve.hpp"
#include "sudokucount.hpp"
#include "util.hpp"
#include "my570list.hpp"
using namespace std;  


#define INFINITY 9999999
#define my_UNDEFINED -99999;


extern char* file;
extern char linebuf[MAXBUFSIZE][MAXBUFSIZE]; 
extern int linebufcount;
extern int validrowcount;
extern char in_sudoku[MAXROW][MAXCOL];
extern char out_sudoku[MAXROW][MAXCOL];
extern int in_sudoku_int[MAXROW][MAXCOL];
extern int out_sudoku_int[MAXROW][MAXCOL];

char allnumber[]={'0','1','2','3','4','5','6','7','8','9'};
/*

bool SolveSudoku(Grid<int> &grid)
{
  int row, col;
  if (!FindUnassignedLocation(grid, row, col)) 
    return true; // all locations successfully assigned!
  for (int num = 1; num <= 9; num++) { // options are 1-9
    if (NoConflicts(grid, row, col, num)) { // if # looks ok
grid(row, col) = num;        // try assign #
   if (SolveSudoku(grid)) return true;   // recur if succeed stop
   grid(row, col) = UNASSIGNED;       // undo & try again
    }
  }
  return false; /
 }
*/
void sudokusolve(int argc, char* argv[]) {

	processInput_solve(argc,argv); 
	readValidLines();			///<  linebuf[][] is updated
	checkvalidity(in_sudoku);	///< check for repeated numbers in rows
	
	
	//displaysudoku(in_sudoku);	///<  in_sudoku[][] is updated with minor validations eg: 9 rows 9 cols of valid dots	
	copySudokuInputtoIntArr();
	#ifdef DEBUG0
	displaysudoku(in_sudoku);
	fprintf(stdout,"***************************************\n");
	#endif
	
	if(solve_by_backtrack(in_sudoku_int)) {
		displaysudoku_int(in_sudoku_int);
	}else {
		fprintf(stderr,"(malformed input): No solution found for input!\n");
		exit(-1);
	}
	/*if(isSolved(in_sudoku_int)) {
		displaysudoku_int(in_sudoku_int);
	}else {
		fprintf(stderr,"(malformed input): No solution found for input!\n");
		exit(-1);
	}
	*/
}


bool is_Assigned(int num) {
	if(num==DOT) 
		return false;
	else 
		return true;
}

bool NoConflict(int input[][MAXCOL], int row, int col,int num) {
	for (int j = 0; j < MAXCOL; j++)	{	
		if (num == input[row][j]) {
			return false;
		}
	}
	for (int i = 0; i < MAXROW; i++)	{	
		if (num == input[i][col]) {
			return false;
		}
	}	
/*
NEEd to check which box the row and col belogns to
0	0 1 2  3 4 5  6 7 8
1	0 1 2  3 4 5  6 7 8
2	0 1 2  3 4 5  6 7 8

3	0 1 2  3 4 5  6 7 8
4	0 1 2  3 4 5  6 7 8
5	0 1 2  3 4 5  6 7 8

6	0 1 2  3 4 5  6 7 8
7	0 1 2  3 4 5  6 7 8
8	0 1 2  3 4 5  6 7 8

given a row u want the top most row num
for eg: if row = 3 or 4 or 5 we want to start from 3
        if col = 3 or 4 or 5 we want to start from 3
*/	
	int startrownumofbox = (row/3)*3;
	int startcolnumofbox = (col/3)*3;
	int x=0,y=0;
	for (x = startrownumofbox; x < startrownumofbox+3; x++)	{	
		for(y=startcolnumofbox;y<startcolnumofbox+3;y++) {
			if (num == input[x][y]) {
				return false;
			}
		}
	}	
	return true;
}

bool FindFreeSlots(int input[][MAXCOL],int *row,int *col) {
	int i=0,j=0;
	for(i=0;i<MAXROW;i++) {
		for(j=0;j<MAXCOL;j++) {
			if(input[i][j]==DOT) {
				*row = i; *col = j;
				#ifdef DEBUG4
				fprintf(stdout,"i=%d j=%d\n",*row,*col);
				#endif
				return true;
			}
		}
	}
	return false;
}
bool solve_by_backtrack(int input[][MAXCOL]) {
	int row, col;
	#ifdef DEBUG3
	 while(1) { //THIS IS TO TEST FINDFREESLOTS! FUNCTION
		if(FindFreeSlots(input,&row,&col)){
			input[row][col] = 0;/*meaning filled*/
			displaysudoku_int(input);
		}else{
			displaysudoku(in_sudoku);
			break;
		}
	}
	#endif
	if(!FindFreeSlots(input,&row,&col)) {// meaning there are no free slots left meaning no free dots
		return true;
	}
	
	for(int num =1;num<=9;num++) { ///< iteratively choose one of the numbers from 1 to 9
		if(NoConflict(input,row,col, num)) { //go ahead and assign coz its ok!!
			input[row][col]=num;
			if(solve_by_backtrack(input)) return true;
			input[row][col] = DOT;
		}
	}
	return false; ///meaning you did not find a suitable num and u do backtracking to earlier solution
	
}



void checkvalidity(char arr[][MAXCOL]) {

	int i=0;
	int j=0;
	int count[10];

	for(i=0;i<MAXROW;i++) {
		for(int m=1;m<=MAXCOL;m++) {
			count[m]=0;
		}		
		for(j=0;j<MAXCOL;j++) {
			if(is_digit(arr[i][j])) {	
				int num = getmyint(arr[i][j]);
				count[num]++;
				if(count[num]>1){
					fprintf(stderr,"(malformed input) - duplicates numbers found along row in input	%d %d\n",num,count[num]);
					exit(-1);
				}
			}
		}
	}
	for(i=0;i<MAXROW;i++) {
		for(int m=1;m<=MAXROW;m++) {
			count[m]=0;
		}		
		for(j=0;j<MAXCOL;j++) {
			if(is_digit(arr[j][i])) {	
				int num = getmyint(arr[j][i]);
				count[num]++;
				if(count[num]>1){
					fprintf(stderr,"(malformed input) - duplicates numbers found along col in input	%d %d\n",num,count[num]);
					exit(-1);
				}
			}
		}
	}	
}

void readValidLines() {
	//total num of valid lines shud be 9
	//a line is valid if it contains 9 number/dots together..
	//use  bool is_digit_or_dot (char chr) to check if the char is a digit or a dot.
	//there can be a case where u have .2.4.4. on line one this shud be ignored since its count is less.
	int validrowcount=0;
	int validcolcount =0;
	int i=0;
	int j=0;
	int k=0;
	for(i=0;i<linebufcount;i++) {
		int linelen = strlen(linebuf[i]);
		validcolcount = 0;
		for(j=0;j<linelen;j++) {
			if(is_digit_or_dot(linebuf[i][j])){
				if(validcolcount<MAXCOL){
					in_sudoku[k][validcolcount]=linebuf[i][j];
				}
				validcolcount++;

			}
			#ifdef DEBUG2
				fprintf(stdout,"%c",linebuf[i][j]);
			#endif
		}
		if(validcolcount==MAXCOL) {
			k++;
			validrowcount++;
		}
		#ifdef DEBUG2
				fprintf(stdout,"\n");
		#endif
	}
	
	if(validrowcount!=MAXROW) {
		fprintf(stderr,"(malformed input) - Need atleast 9 valid rows with dots and digits \n");
		exit(-1);
	}
	
}
void copySudokuInputtoIntArr(){
	int i=0;int j=0;
	for(i=0;i<MAXROW;i++) {
		for(j=0; j<MAXCOL;j++) {
			if(in_sudoku[i][j]=='.'){
				in_sudoku_int[i][j]= DOT;
			}else {
				in_sudoku_int[i][j]=getmyint(in_sudoku[i][j]);
			}
		}
	}
}

bool isSolved(int arr[][MAXCOL]) {
	int i=0;int j=0;
	for(i=0;i<MAXROW;i++) {	
		for(j=0; j<MAXCOL;j++) {
			if(arr[i][j]==DOT){
				if(in_sudoku[i][j]=='.') {///meaning solution also has a dot not filled up.
					return false;
				}
			
			}
			/*else {
				if(getmyint(in_sudoku[i][j])==arr[i][j]) {///meaning solution has same num as input.
					return false;
				}
			
			}*/
			
		}
	}
	
	return true;
}
void displaysudoku_int(int arr[][MAXCOL]){
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
				if(in_sudoku[i][j]!='.') 
					fprintf(stdout,".");
				else
					fprintf(stdout,"%d",arr[i][j]);
			}
		}
		fprintf(stdout,"|\n");
	}
	fprintf(stdout,"+---+---+---+\n");
}

void displaysudoku(char arr[][MAXCOL]){
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
			fprintf(stdout,"%c",arr[i][j]);
		}
		fprintf(stdout,"|\n");
	}
	fprintf(stdout,"+---+---+---+\n");
}

void readFile_solve(istream& in) {
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
		//fprintf(stdout,"strlen=%d\n",strlen(buf));
		/*int len = strlen(buf);
		if(len==0){
			in.getline(buf,MAXBUFSIZE);
			continue;
		}*/
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
 
void processInput_solve(int argc,char* argv[]) {

		if (argc > 3) { ///< max is 3 args 
			fprintf(stderr,"(malformed command) - too many  arguments\n");
			sudokusolveUsage();
		}
		if (argc==3) {
			file = argv[2];
			#ifdef DEBUG0
			fprintf(stdout,"filename:%s\n",file);
			#endif
		} else {
			file = NULL;
			#ifdef DEBUG0
			fprintf(stdout,"filename:NULL\n");
			#endif
		}
		if(file==NULL){ 
			readFile_solve(cin);///< read from standard input
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
			readFile_solve(in);
			in.close();		
		}
}

/*
    hw5 solve [file]
    hw5 count [-printall] [-max=n] [file]
    hw5 gen [-info] [rndfile]
*/
void sudokusolveUsage() {
	fprintf(stderr,"Usage:\n");
	exit(-1);	
}

