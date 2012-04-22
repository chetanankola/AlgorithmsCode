#include "util.hpp"
#include "main.hpp"
#include "mazegen.hpp"
#include "mazesolve.hpp"

using namespace std;  
/* 
	* USAGE:
	* hw4 mazegen w h [-info] [-top=tpos] [-bottom=bpos] [-left=lpos] [-right=rpos] [rndfile]
	* hw4 mazesolve [file]
	* Square bracketed items are optional. If rndfile or file is not specified, your program should read from stdin. 
	* You must follow the UNIX convention that commandline options can come in any order. 
	*(Note: a commandline option is a commandline argument that begins with a - character in a commandline syntax specification.)
	* Unless otherwise specified, output of your program must go to stdout and error messages must go to stderr.
**/
int MYNUM = -1234;
int w = 0;					///< width of the maze
int h = 0;					///< height of the maze
int top = MYNUM;				///< if none specifed  -top=1 and -bottom=w [1 to 64]
int bottom = MYNUM;				///< if none specifed  -top=1 and -bottom=w [1 to 64]
int lefto = MYNUM;				///< [1 to 32]
int righto = MYNUM;				///< [1 to 32]
char* file = NULL; 			///< will be rndfile for mazegen and file input for mazesolve; IF NULL then = stdin
bool infoFlag = false;		///< is true  if -info argument is set

bool isStartset = false;
char start[10];
char end[10];

int startnum = 0;
int endnum = 0;

int main (int argc, char *argv[]) {

	if(argc<2) {
		fprintf(stderr,"(malformed command)\n");
		GUsage();
	}
	
	if(	!strcmp(argv[1],"mazegen") ) {
		#ifdef DEBUG0
			fprintf(stdout,"----------mazegen----------------------\n");
		#endif
		mazegen(argc,argv); ///<generate a maze
		return 1;
	}

	if(!strcmp(argv[1],"mazesolve") ) {
		#ifdef DEBUG0
			fprintf(stdout,"---------mazesolve-------------\n");
		#endif 
		mazesolve(argc,argv); ///< solve a maze
		return 1;
	}
	fprintf(stderr, "(malformed command)\n");
	GUsage();
	return 1;
}

/* 
 *    USAGE:
	* hw4 mazegen w h [-info] [-top=tpos] [-bottom=bpos] [-left=lpos] [-right=rpos] [rndfile]
	* hw4 mazesolve [file]
	* Square bracketed items are optional. If rndfile or file is not specified, your program should read from stdin. 
	* You must follow the UNIX convention that commandline options can come in any order. 
	*(Note: a commandline option is a commandline argument that begins with a - character in a commandline syntax specification.)
	* Unless otherwise specified, output of your program must go to stdout and error messages must go to stderr.
**/
void GUsage() {
	fprintf(stderr,"Usage:\n");
	fprintf(stderr,"\thw4 mazegen w h [-info] [-top=tpos] [-bottom=bpos] [-left=lpos] [-right=rpos] [rndfile] \n");
	fprintf(stderr,"\thw4 mazesolve [file]\n");
	exit(-1);
}


