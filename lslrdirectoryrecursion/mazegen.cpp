#include "mazesolve.hpp"
#include "mazegen.hpp"
#include "util.hpp"
#include "my570list.hpp"

using namespace std;  



WALL *Walls[4*32*65];
ROOM *Rooms[32*64*2];

extern int MYNUM;
extern int w;					///< width of the maze
extern int h;					///< height of the maze
extern int top;					///< if none specifed  -top=1 and -bottom=w [1 to 64]
extern int bottom;				///< if none specifed  -top=1 and -bottom=w [1 to 64]
extern int lefto;				///< [1 to 32]
extern int righto;				///< [1 to 32]
extern char* file; 				///< will be rndfile for mazegen and file input for mazesolve; IF NULL then = stdin
extern bool infoFlag;			///< is true  if -info argument is set
extern bool isStartset;
extern char start[10];			///< these are just char that tells if top or bottom or right or left 
extern char end[10];



/* 
 *    USAGE:
	* hw4 mazegen w h [-info] [-top=tpos] [-bottom=bpos] [-left=lpos] [-right=rpos] [rndfile]
	* hw4 mazesolve [file]
	* Square bracketed items are optional. If rndfile or file is not specified, your program should read from stdin. 
	* You must follow the UNIX convention that commandline options can come in any order. 
	*(Note: a commandline option is a commandline argument that begins with a - character in a commandline syntax specification.)
	* Unless otherwise specified, output of your program must go to stdout and error messages must go to stderr.
**/

void mazegen(int argc, char* argv[]) {
	///< build structures from arguments passed
	processArgs(argc,argv);
	
	#ifdef DEBUG0
	fprintf(stdout,"height=%d\nwidth=%d\n-info = %d\n",h,w,infoFlag);
	if(file==NULL) {
		fprintf(stdout,"Rndfile:stdin\n");
	} else {
		fprintf(stdout,"Rndfile:%s\n",file);
	}
	fprintf(stdout,"top=%d left=%d right=%d bottom=%d\n",top,lefto,righto,bottom);
	fprintf(stdout,"StartWall=%s, EndWall=%s\n",start,end);
	#endif
	

	initializeWallsRooms();
	readRndFile();	
	#ifdef DEBUG1
	test_removeRandomWalls();
	#endif
	displaymaze();
}




void initializeWallsRooms() {
	
	int totalnumofWalls = ((h-1)*w) + ((w-1)*h);
	bool inVerticalWallrow = true;
	int j = 0;
	while(j!=totalnumofWalls) { ///< wall spans from h-1)
			if(inVerticalWallrow) {
				for(int i = j; i <(j+w-1);i++){
					Walls[i] = new struct Wall;	
					Walls[i]->index = i;	
					Walls[i]->isvertical = true;
					Walls[i]->isremoved = false;
					Walls[i]->cost = 1;
				}
				inVerticalWallrow = false;
				j = j+w-1;
			}else {
				for(int i = j; i <(j+w);i++){
					Walls[i] = new struct Wall;	
					Walls[i]->index = i;
					Walls[i]->isvertical = false;
					Walls[i]->isremoved = false;
					Walls[i]->cost = 1;
				}
				inVerticalWallrow = true;
				j = j+w;
			}
	}
	int totalrooms = (w*h);
	for(int i = 0;i< totalrooms;i++) {
		Rooms[i] = new struct Room;
		Rooms[i]->index  = i;
		Rooms[i]->parent = Rooms[i];
		Rooms[i]->rank = 0;
		Rooms[i]->apple = ' ';
	}
	#ifdef DEBUG1
	for(int i = 0; i <totalnumofWalls ;i++) {
		//fprintf(stdout,"walltype[%d]:%c ",Walls[i]->index,Wallchar(Walls[i]));
		//fprintf(stdout,"row(%d)=%d",Walls[i]->index,getrownum(Walls[i]->index));
		//fprintf(stdout,"leftroom(%d)=%d",Walls[i]->index,getleftroom(Walls[i]->index));
		//fprintf(stdout,"upperroom(%d)=%d\n",Walls[i]->index,getupperroom(Walls[i]->index));

		TWOROOMS pairofroom = getAdjacentRooms(Walls[i]);

		if(Walls[i]->isvertical){
			fprintf(stdout,"[room:%d]",pairofroom.room1->index);
			fprintf(stdout,"[%d|]",Walls[i]->index);
			fprintf(stdout,"[room:%d]",pairofroom.room2->index);
		}
		else{
			fprintf(stdout,"[room:%d]",pairofroom.room1->index);
			fprintf(stdout,"[%d--]",Walls[i]->index);
			fprintf(stdout,"[room:%d]",pairofroom.room2->index);		
		}
		
		fprintf(stdout,"\n");
		

		}
	#endif
}
 	/* w=4 h =3 
		+-+-+-+-+
		|R0   0    R1    1     R2    2   R3  |
		+3+       +4+          +5+       +6+
		|R4   7    R5    8     R6    9   R7  |
		+10+      +11+        +12+      +13+
		|R8   14   R9    15    R10   16  R11 |
		+-+-+-+-+

	 w=5 h=3	
		+-+-+-+-+
		|R0  0   R1  1    R2   2   R3    3     R4
		+4+     +5+      +6+       +7+        +8+
		|R5  9   R6  10   R7   11   R8    12    R9
		+13+     +14+    +15+      +16+       +17+
		|R10  18  R11 19  R12   20  R13   21    R14
		+-+-+-+-+		
0 1 2 3 4 5 6
7 8 9 0 1 2 3
4 5 6 7 8 9 0
upperroom calculation==============================================================		
w=5 h=3
	wall: 4 5 6 7 8 13 14 15 16 17
	uroom:0 1 2 3 4  5  6  7  8  9
	DIF=  4 4 4 4 4  4*2

w=4 h=3
	wall	3	4	5	6	10	11	12	13
	room	0	1	2	3	4	5	6	7
			
		3-0 =3
		4-1 =3
		5-2 =3
		6-3 =3
		
		10-4 = 3*2
		11-5 = 3*2
		12-6 = 3*2 

		wall - room = (w-1) * (row+1)
		
		room = wallnum - ((w-1)*(getrownum(wallnum)+1));
		horizontal wall-room = h* 
===================================================================================

		
left room calculation	===================================================
	 w=5 h=3	
		+-+-+-+-+
		|R0  0   R1  1    R2   2   R3    3     R4
		+4+     +5+      +6+       +7+        +8+
		|R5  9   R6  10   R7   11   R8    12    R9
		+13+     +14+    +15+      +16+       +17+
		|R10  18  R11 19  R12   20  R13   21    R14
		+-+-+-+-+	
		
w=5 h=3
		wall:0 1 2 3 4 		9	10	11	12	18	19	20	21	
		room:0 1 2 3 4		5	6	7	8	10	11	12	13
diff		 0 0 0 0 0 		4	4	4	4	8 	8	 8	 8
			
		wall - room = row*(w-1)
		room = wallnum- [getrownum(wallnum)*(w-1)]

		0   1    2   7  8  9   14    15     16
		0   1    2   4  5  6   8      9     10
		
==============================================================================================
ROW CALCULATION:==========================================================
		14 = row*(w-1) +row*w
		14 = row(w-1+w)
		row = 14/(2w-1);
		/// row(7) =((7 -h)/w) =1
		///i.e (7-3)/4 = 1
		/// row(15) = (15-3)/4 = 3	

=============================================================================
	*/


///Row number makes sense only to  a vertical wall since if wall 7 is in row 1 then the room num = 
int getrownum(int wallnum) {

	/*if(!Walls[wallnum]->isvertical) {
		//note that this is wall num only for the vertical walls...
		#ifdef DEBUG0
			return -1;
		#endif
		fprintf(stderr,"Error: A horizontal wall should not be passed here\n");
		exit(-1);
	}*/
	return ((wallnum)/((2*w)-1));
}

int getleftroom ( int wallnum) {
	//return ( wallnum-(h*getrownum(wallnum)) );
	return wallnum- (getrownum(wallnum)*(w-1));

	}

int getupperroom( int wallnum) {
	return (wallnum - ((w-1)*(getrownum(wallnum)+1)));
}

void readRndFile() {
	///< OPEN THE FILE FIRST
	FILE* fptr;
	if(file==NULL) {
		//meaning no file was specified and hence has to be read from stdin
		fptr=stdin;
	} else { ///< a valid file name is specified
		ProcessFilenameFailure(file);
		fptr = fopen(file, "rb");
		if (fptr==0) {
			fprintf (stderr,"Unable to open Rndfile %s\n",file); 
			exit (-1);
		}
	}
	
///Push all the walls into a list; ---------------------------------------------------------
	int totalwalls = ((w-1)*h) + ((h-1)*w);
	int minwallstoremove = (w*h)-1;
	My570List *list=new My570List();
	for(int i=0;i<totalwalls;i++) {
		(void)list->Append((void*)Walls[i]);
	}	
	#ifdef DEBUG1
	///< to check what was pushed into the list really went in.
	My570ListElem *elem=NULL;
	for (elem=list->First(); elem != NULL; elem=list->Next(elem)) {
        struct Wall *temp =(struct Wall*)(elem->Obj());
		fprintf(stdout,"%d\n",temp->index); 	
    }
	#endif
	
///< -----------------------------------------------------------------------------------------
	int iteration=1;
	while(minwallstoremove!=0) {
		int listlen = 0;
		listlen = list->Length();
		unsigned char buf[4] ;
		if(fread (buf,1,4,fptr)!=4){	
			if(file==NULL)
				fprintf(stderr,"Random input ran out of bytes\n"); 
			else
				fprintf(stderr,"Rnd input file: %s ran out of bytes\n",file);
			
			exit(-1); 
		}
		///////TODO: mark the ones removed as "has been inspected!!"
		unsigned int rmwallnum=0;
		memcpy(&rmwallnum, buf, 4);
		int walltopushtolist = rmwallnum % (list->Length());

		/*
		My570ListElem *elem = list->Find((void*)Walls[walltopushtolist]);
		if(elem==NULL) {
			fprintf(stderr,"Elem popped from list is NULL \n");
			exit(-1);
		}
		*/
		///< Iterate to the random number'th element in the list and unlink it first.
		My570ListElem *elem = NULL;
		elem=list->First();
		struct Wall *temp =(struct Wall*)(elem->Obj());
		for (int i=0;i<walltopushtolist;i++){
			elem=list->Next(elem);
			temp =(struct Wall*)(elem->Obj());
		} 			
		///< TO CHECK> should the wall be inspected if it is to be removed again?
		list->Unlink(elem);
		
		TWOROOMS pairofroom = getAdjacentRooms(temp);
		

				
		#ifdef DEBUG1
		if(temp->isvertical)
			fprintf(stdout,"[room:%d][%d|]room:%d]",pairofroom.room1->index,temp->index,pairofroom.room2->index);
		else
			fprintf(stdout,"[room:%d][%d--]room:%d]",pairofroom.room1->index,temp->index,pairofroom.room2->index);
	
		fprintf(stdout,"\n");
		#endif

		
		if(isUnion(pairofroom.room1,pairofroom.room2)) {
		#ifdef DEBUG0
		fprintf(stdout,"iteration:%d \t Listlen:%d \t R:%d \taction: wall not removed %d\n",iteration,listlen,walltopushtolist,temp->index);
		#endif			
		} else {
			minwallstoremove--;
			temp->isremoved=true;
			#ifdef DEBUG0
			fprintf(stdout,"iteration:%d \t Listlen:%d \t R:%d \taction: remove wall %d\n",iteration,listlen,walltopushtolist,temp->index);
			#endif
			if(infoFlag){
				fprintf(stdout,"Wall %d removed.\n",temp->index);
			}
		}
		

		///This operation is done only if the wall is removable else you dont decrement it..
		
		iteration++;
	}	
}

TWOROOMS getAdjacentRooms(WALL* wall) {
 	/* w=4 h =3 
		+-+-+-+-+
		|R0  0  R1 1 R2  2  R3 |
		+3+    +4+   +5+    +6+
		|R4  7  R5 8 R6  9  R7 |
		+0+    +1+   +2+    +3+
		|R8  4  R9 5 R10 6  R11 |
		+-+-+-+-+
		
	 w=5 h=3	
		+-+-+-+-+
		|R0  0   R1  1    R2   2   R3    3     R4
		+4+     +5+      +6+       +7+        +8+
		|R5  9   R6  10   R7   11   R8    12    R9
		+13+     +14+    +15+      +16+       +17+
		|R10  18  R11 19  R12   20  R13   21    R14
		+-+-+-+-+		
	*/
	TWOROOMS s_room;
	if(wall==NULL) {
		fprintf(stderr," getAdjacentRooms(NULL): NULL WALL WAS PASSED!!! \n");
		exit(-1);
	}
	int roomindex=0;
	int nextroom =0;
	if(wall->isvertical) {
		roomindex = getleftroom(wall->index); 
		s_room.room1 = Rooms[roomindex]; /// for eg: R0 and R1
		nextroom = roomindex+1;
		s_room.room2 = Rooms[nextroom]; 
	} else {
		roomindex = getupperroom(wall->index); //wall is horizontal
		s_room.room1 = Rooms[roomindex];
		nextroom = roomindex+w;
		s_room.room2 = Rooms[nextroom]; /// for eg: R0 and R4 , R5 and R9
	}
	#ifdef DEBUG3
	fprintf(stderr,"nextroomindex=%d  ",nextroom);
	fprintf(stderr,"totalnumofrooms=[0 to %d]",(w*h)-1);
	#endif
	return s_room;
}


/*
 * Begin code I did not write.
 * This code is derived after looking at the pseudo code in wikipedia: http://en.wikipedia.org/wiki/Disjoint-set_data_structure
 * 
 */
/**
 * returns true if X and Y belong to the same set.
 * returns false if X and Y belong to different set, and then X AND Y ARE MERGED TO THE SAME SET
 */
bool isUnion(ROOM* x, ROOM* y) {

    ROOM* xRoot = Find(x);
    ROOM* yRoot = Find(y);
    if(xRoot == yRoot)
         return true;

    // x and y are not already in same set. Merge them.
    if(xRoot->rank < yRoot->rank) {
         xRoot->parent = yRoot;
	} else if(xRoot->rank > yRoot->rank) {
         yRoot->parent = xRoot;
	} else {
         yRoot->parent = xRoot;
         xRoot->rank = xRoot->rank + 1; 
	}
	return false;	 
}
ROOM* Find(ROOM* x) {
	if(x==NULL) {
		fprintf(stderr, "Find(NULL): NULL ROOM passed to FIND\n");
		exit(-1);
	}
	//ROOM* temp = x->parent;
     if (x->parent == x)
        return x;
     else
        //temp = Find(x->parent);
        return Find(x->parent);
}
/*
 * End code I did not write.
 */	
 
 
void test_removeRandomWalls(){
  int a[]= {16,17,18,19,20,90,5,61,83,39,64,7,8,9,10,11,12,13,14};
	for(unsigned int i=0;i< (sizeof(a)/sizeof(a[0]));i++) {
		Walls[a[i]]->isremoved=true;
	}
}


void displaymaze() {
	// for w=4 h=3  we have 4X3 = 12 rooms i.e 3 rows and 4 colums.. 
	// (width represents cols, height represents rows)

	//There are (w-1) × h removable vertical walls
	//there are w × (h-1) removable horizontal walls
	//so Walls are numbered from 1 to totalWalls =  (width-1)*height + (height-1)*width  only inner walls
	/// u need to push these walls into a list 
	/* w=4 h =3 
		+-+-+-+-+
		| 0 1 2 |
		+3+4+5+6+
		| 7 8 9 |
		+-+-+-+-+
		| | | | |
		+-+-+-+-+
	*/

	/*----------------------------------Print the first line---------------------------------------*/
	//print the first line remove the horizontal wall if top is start.top [1-h] ( i=0 to w +-
	for(int i = 0;i<w;i++) {
		fprintf(stdout,"+");
		if(!strcmp(start,"top") || !strcmp(end,"top") ) {
			if((top-1)==i)
				fprintf(stdout," ");
			else
				fprintf(stdout,"-");
		} else {
			fprintf(stdout,"-");
		}
	}
	fprintf(stdout,"+\n");
	/*--------------------------------------------------------------------------------------------*/

	/*--------------------------------------------------------------------------------------------*/	
	/* w=4 h =3 
		+-+-+-+-+
		| 0 1 2 |
		+3+4+5+6+
		| 7 8 9 |
		+-+-+-+-+
		| | | | |
		+-+-+-+-+
	*/
	//next for i = 0 to h*2-1 print all walls
	int colcount =0;
	int i = 0;
	int k = 0;
	int roomnum=0;
	for(i =0;i<(h*2)-1;i++) {
		//For vertical wall'ed row 
		if(i%2==0){
			/*------prints the first verti wall-------*/
			colcount++;
			if(!strcmp(start,"left") || !strcmp(end,"left") ) {
				if(lefto==colcount) {
					fprintf(stdout," ");
				} else {
					fprintf(stdout,"|");
				}
			} else {
				fprintf(stdout,"|");
			}
			/*------------------------------------------*/
			//k is the number that tracks the wall k goes from 0 to w-1 and then w to k+w
			int counter=0;
			while( counter!=(w-1)) {
				//printf("%d ",k);

				fprintf(stdout,"%c",Rooms[roomnum++]->apple);
				fprintf(stdout,"%c",Wallchar(Walls[k]));
				counter++;
				k++;
			}
			fprintf(stdout,"%c",Rooms[roomnum++]->apple);
			if(!strcmp(start,"right") || !strcmp(end,"right") ) {
				if(righto==colcount) {
					fprintf(stdout," ");
				} else {
					fprintf(stdout,"|");
				}
			} else {
				fprintf(stdout,"|");
			}
			fprintf(stdout,"\n");
			//k=k+w;
		} else {
				/* w=4 h =3 
					+-+-+-+-+
					| 0 1 2 |
					+3+4+5+6+
					| 7 8 9 |
					+-+-+-+-+
					| | | | |
					+-+-+-+-+
				*/
			int counter=0;
			while( counter!=(w)) {
				
				fprintf(stdout,"+");
				//printf("%d", k);
				fprintf(stdout,"%c",Wallchar(Walls[k]));
				counter++;
				k++;
			}
			fprintf(stdout,"+");
			fprintf(stdout,"\n");
		}
	}//end of for h*2-1
	
	/*---------------------------------PRINT LAST LINE---------------------------*/
	for(int i = 0;i<w;i++) {
		fprintf(stdout,"+");
		if(!strcmp(start,"bottom") || !strcmp(end,"bottom") ) {
			if((bottom-1)==i)
				fprintf(stdout," ");
			else
				fprintf(stdout,"-");
		} else {
			fprintf(stdout,"-");
		}
	}
	fprintf(stdout,"+\n");
	/*--------------------------------------------------------------------------------------------*/
}

/*
w = 4 h = 3
there are 9 + 8 walls = 17
	  0 1 2
	3  4  5  6
	 7  8  9
	10  11  12  13
	 14  15  16
*/
char Wallchar(WALL *temp) {
	if(temp == NULL) {
		fprintf(stderr,"Wallchar(NULL): A NULL STRUCT PASSED TO FUNCTION!\n");
		exit(-1);
	}

	if(temp->isremoved) {
		return ' ';
	}
	if(temp->isvertical) {
		return '|';
	} else 
		return '-';
}


/**
 * process all the arguments return on errors and build and update all variables needed .
**/
void processArgs(int argc, char* argv[]) {
		if (argc > 8) { ///< max is 8 args eg: hw4 mazegen 2 1 -info -top=1 -bottom=1 rndfilename
			fprintf(stderr,"(malformed command) - too many  arguments for mazegen\n");
			mazegenUsage();
		}
		if (argc < 4) {
			fprintf(stderr,"(malformed command)\n");
			//exit(-1);
			//fprintf(stderr,"(malformed command) - too less arguments for mazegen\n");
			mazegenUsage();
		}
		w = atoi(argv[2]) ;
		h = atoi(argv[3]) ;
		if ( w < 1 || w > 64) {
			//fprintf(stderr,"(malformed command)\n");
			//fprintf(stderr,"Error Input: maze width out of range should be [1,64] \n");
			fprintf(stderr,"(invalid width)\n");
			exit(-1) ;
		}
		if (h < 1 || h > 32) {
			//fprintf(stderr,"(malformed command)\n");
			//fprintf(stderr,"Error Input: maze height out of range should be [1,32] \n");
			fprintf(stderr,"(invalid height)\n");
			exit(-1) ;
		}
		if( argc == 4) {
			infoFlag = false;
			file = NULL; ///< set random file to standard input
			top = 1;
			bottom = w;
			strcpy(start,"top");
			strcpy(end,"bottom");
			return; 
		}
		if(argc > 4 ) {
			int count=0;		
			int filecount = 0; ///< this will keep track of number of filenames exceeding 1..
			for(int i=4;i<argc;i++) {
				if(isarganoption(argv[i])) {
					count = count + processoption(argv[i]);
				} else {
					file = argv[i];
					filecount++;
				}
			}
			if(filecount>1) {
				fprintf(stderr,"(malformed command)\n");
				fprintf(stderr, "Error in arguments: Found more than 1 argument that looks like a filename\n");
				mazegenUsage();
			}
			if(count ==0 || count==2) {
				if(count==0) {
					top = 1;
					bottom = w;
					strcpy(start,"top");
					strcpy(end,"bottom");
				}	
			} else {
				//fprintf(stderr, "Error in arguments: Either 0 or 2 of top left right bottom should be specified\n");
				fprintf(stderr,"(malformed command, need to specify two openings)\n");
				mazegenUsage();
			}
		}
		
		checkboundsforStartEnd();

}

void checkboundsforStartEnd() {

	if(!strcmp(start,"top") || !strcmp(end,"top")) {
		if (top > w || top < 1) {
			fprintf(stderr,"Error in input: top is out of bounds! should be [1,%d]\n",w);
			exit(-1);
		}	
	}
	if(!strcmp(start,"right") || !strcmp(end,"right")) {	
		if (righto > h || righto < 1) {
			fprintf(stderr,"Error in input :right is out of bounds! should be [1,%d]\n",h);
			exit(-1);		
		}
	}
	if(!strcmp(start,"bottom") || !strcmp(end,"bottom")) {
		if (bottom > w || bottom < 1) {
			fprintf(stderr,"Error in input : bottom is out of bounds! should be [1,%d]\n",w);
			exit(-1);
		}
	}
	if(!strcmp(start,"left") || !strcmp(end,"left")) {
		if (lefto > h || lefto < 1) {
			fprintf(stderr,"Error in input: left is out of bounds! should be [1,%d]\n",h);
			exit(-1);		
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
	if(!strcmp(optarg,"-info")) {
		infoFlag = true;
		return 0;
	}
	char* temp = new char[MAXBUFSIZE];
	strcpy(temp,optarg);
	char* token = strtok(temp,"=");
	while(token!=NULL) {
		#ifdef DEBUG0
		fprintf(stdout,"DEBUG:%s\n",token);
		#endif
		if(!strcmp(token,"-top")) {
			token = strtok(NULL,"=");
			if(top!=MYNUM)  {
				fprintf(stderr,"Error in arguments:cannot specify -top option more than once \n");
				exit(-1);
			}
			top=atoi(token);
			if(isStartset) {
				strcpy(end,"top");
			}else {
				isStartset=true;
				strcpy(start,"top");
			}
		} else if(!strcmp(token,"-right")) {
			token = strtok(NULL,"=");
			if(righto!=MYNUM)  {
				fprintf(stderr,"Error in arguments:cannot specify -right option more than once \n");
				exit(-1);
			}
			righto=atoi(token);
			if(isStartset) {
				strcpy(end,"right");
			}else {
				isStartset=true;
				strcpy(start,"right");
			}
		} else if(!strcmp(token,"-bottom")) {
			token = strtok(NULL,"=");
			if(bottom!=MYNUM)  {
				fprintf(stderr,"Error in arguments:cannot specify -bottom option more than once \n");
				exit(-1);
			}
			bottom=atoi(token);
			if(isStartset) {
				strcpy(end,"bottom");
			}else {
				isStartset=true;
				strcpy(start,"bottom");
			}
		} else if(!strcmp(token,"-left")) {
			token = strtok(NULL,"=");
			if(lefto!=MYNUM)  {
				fprintf(stderr,"Error in arguments:cannot specify -left option more than once \n");
				exit(-1);
			}
			lefto=atoi(token);
			if(isStartset) {
				strcpy(end,"left");
			}else {
				isStartset=true;
				strcpy(start,"left");
			}
		} else {
			fprintf(stderr,"unknown option in argument !!!\n");
			exit(-1);
		}
		token = strtok(NULL,"=");
	}
	
	return 1;
}

bool isInfoSet(int argc, char* argv[]) {

	bool flag = false;
	for(int i=0;i<argc;i++ ) {
		if(!strcmp(argv[i],"-info")) {
			flag = true;
			return flag;
		}
	}
	return flag;
}



void mazegenUsage() {
	fprintf(stderr,"Usage:\n");
	fprintf(stderr,"\thw4 mazegen w h [-info] [-top=tpos] [-bottom=bpos] [-left=lpos] [-right=rpos] [rndfile] \n");
	exit(-1);	
}


