#include "util.hpp"
#include "main.hpp"
#include "sudokugen.hpp"
#include "sudokusolve.hpp"
#include "sudokucount.hpp"
using namespace std;  


char* file = NULL; 				///< filename that contains input file or rndfile
bool isInfoSet = false;
bool isPrintallSet = false;
bool ismaxSet = false;
int maxn = 0; ///<default value
char linebuf[MAXBUFSIZE][MAXBUFSIZE]; 	///< to prevent reading file many times Lets read it once for all into a 2 dimensional buffer?
int linebufcount = 0;
char in_sudoku[MAXROW][MAXCOL];
char out_sudoku[MAXROW][MAXCOL];
int in_sudoku_int[MAXROW][MAXCOL];
int out_sudoku_int[MAXROW][MAXCOL];

/*
    hw5 solve [file]
    hw5 count [-printall] [-max=n] [file]
    hw5 gen [-info] [rndfile]
*/
int main (int argc, char *argv[]) {

	if(argc<2) {
		fprintf(stderr,"(malformed command)\n");
		GUsage();
	}
	
	if(	!strcmp(argv[1],"solve") ) {
		#ifdef DEBUG4
			fprintf(stdout,"----------Sudoku solve----------------------\n");
		#endif
		sudokusolve(argc,argv); ///<solve a sudoku
		return 1;
	}

	if(!strcmp(argv[1],"count") ) {
		#ifdef DEBUG4
			fprintf(stdout,"---------Sudoku count-------------\n");
		#endif 
		sudokucount(argc,argv); ///< count
		return 1;
	}
	if(!strcmp(argv[1],"gen") ) {
		#ifdef DEBUG4
			fprintf(stdout,"---------Sudoku gen-------------\n");
		#endif 
		sudokugen(argc,argv); ///< count
		return 1;
	}
	fprintf(stderr, "(malformed command)\n");
	GUsage();
	return 1;
}

/*
    hw5 solve [file]
    hw5 count [-printall] [-max=n] [file]
    hw5 gen [-info] [rndfile]
*/
void GUsage() {
	fprintf(stderr,"Usage:\n");
	fprintf(stderr,"\thw5 solve [file]\n");
	fprintf(stderr,"\thw5 count [-printall] [-max=n] [file]\n");
	fprintf(stderr,"\thw5 gen [-info] [rndfile]\n");
	exit(-1);
}


