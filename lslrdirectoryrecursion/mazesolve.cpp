#include "mazesolve.hpp"
#include "mazegen.hpp"
#include "util.hpp"
#include "my570list.hpp"
using namespace std;  




#define FIRSTLINE 0
#define LASTLINE 1
#define INFINITY 9999999
#define my_UNDEFINED -99999;

extern int w;					///< width of the maze
extern int h;					///< height of the maze
extern int top;				///< if none specifed  -top=1 and -bottom=w [1 to 64]
extern int bottom;				///< if none specifed  -top=1 and -bottom=w [1 to 64]
extern int lefto;				///< [1 to 32]
extern int righto;				///< [1 to 32]
extern char* file; 			///< will be rndfile for mazegen and file input for mazesolve; IF NULL then = stdin
extern bool infoFlag;		///< is true  if -info argument is set


extern WALL *Walls[4*32*65];
extern ROOM *Rooms[32*64*2];


int AdjMatrix[64*32*2][64*32*2];
int totalnumofrooms = 0;
int totalnumofwalls = 0;
int wallindex = 0;						///<this will become same as totalnumofwalls
int startroomnum = -1;
int endroomnum = -1;
int startroomrow = -1;
int startroomcol = -1;
int endroomrow  = -1;
int endroomcol = -1;
int BFS_shortestpath_cost = INFINITY;
int Dijkstra_shortestpath_cost = INFINITY;
int BFS_maxlevel = 0;
char linebuf[MAXBUFSIZE][MAXBUFSIZE]; 	///< to prevent reading file many times Lets read it once for all into a 2 dimensional buffer?
int linebufcount = 0;
int totalwallsremoved = 0;				///<for BFS totalwallsremoved = (wxh)-1 exactly and for Dijkstra its >(wxh)-1
bool isBFS = true;						///if false use Dijkstra
bool startfound = false;
bool endfound = false;

bool FoundnonzerocostinTYPE1 = false;

struct specialroom {
	char type[10];
	int num;  ///< this num varies from [0 to w-1]
};
struct specialroom startroom;
struct specialroom endroom;



/* 
 *    USAGE:
	* hw4 mazegen w h [-info] [-top=tpos] [-bottom=bpos] [-left=lpos] [-right=rpos] [rndfile]
	* hw4 mazesolve [file]
	* Square bracketed items are optional. If rndfile or file is not specified, your program should read from stdin. 
	* You must follow the UNIX convention that commandline options can come in any order. 
	*(Note: a commandline option is a commandline argument that begins with a - character in a commandline syntax specification.)
	* Unless otherwise specified, output of your program must go to stdout and error messages must go to stderr.
**/
void mazesolve(int argc, char* argv[]) {
	processargs_ms(argc,argv);		///< process if a filename has been specified or not
	processInput();					///< process the file input or stdin input and check for erroneous inputs
	buildWalls();					///< linbuf[] and linebufcount contains all input of file so build walls 	
	initializeRooms();
	connectWalls();
	#ifdef DEBUG0
	fprintf(stdout,"w=%d, h=%d, numofrooms=%d, startroom=%s=%d endroom=%s=%d\n",w,h,totalnumofrooms,startroom.type,startroom.num,endroom.type,endroom.num);
	fprintf(stdout,"totalnumofwalls=%d == %d = (w-1)h+ h-1*w\n",wallindex,((w-1)*h + (h-1)*w));
	fprintf(stdout,"startroomnum = %d, endroomnum=%d\n",startroomnum,endroomnum);
	fprintf(stdout,"totalwallsremoved = %d \n",totalwallsremoved);
	fprintf(stdout,"wxh-1=%d\n",((w*h)-1));
	if(isBFS)
		fprintf(stdout,"TYPE1: BFS\n");
	else
		fprintf(stdout,"TYPE2: DIJKSTRA\n");
	#endif


	if(isBFS) {
		mazesolvebyBFS();
	}else {
		mazesolvebyDijkstra();
	}
	displaymaze_solve();

}


bool isConnected(int room1index,int room2index) {

	if(AdjMatrix[room1index][room2index]==INFINITY)
		return false;	///meaning not connected meaning no wall between them
	else
		return true;	/// meaning connected
}


/*
 * Begin code I did not write.
 * This code is derived after looking at the pseudo code in wikipedia: http://en.wikipedia.org/wiki/Breadth-first_search
 * 
 */
void mazesolvebyBFS() {

/* Pseudo code from wikipedia 
==========================================
1  procedure BFS(Graph,source):
2      create a queue Q
3      enqueue source onto Q
4      mark source
5      while Q is not empty:
6          dequeue an item from Q into v
7          for each edge e incident on v in Graph:
8              let w be the other end of e
9              if w is not marked:
10                  mark w
11                  enqueue w onto Q
===========================================
*/
	BFS_shortestpath_cost = 0;
	BFS_maxlevel=0;
	My570List *BFSlist=new My570List();
	(void)BFSlist->Append((void*)Rooms[startroomnum]);
	Rooms[startroomnum]->isvisited = true;
	My570ListElem *elem=NULL;
	struct Room* poppedroom = NULL;
	while(!BFSlist->Empty()) {
	///< 6.
		elem = BFSlist->First();
		poppedroom = (struct Room*)(elem->Obj());
		BFSlist->Unlink(elem);
	///< 7.
		bool atleastoneaddedtoQue = false;
		for(int i=0;i<totalnumofrooms;i++) {
			if(isConnected(poppedroom->index,Rooms[i]->index)) {
				if(!Rooms[i]->isvisited) {
					Rooms[i]->isvisited=true;
					Rooms[i]->parent = poppedroom;
					(void)BFSlist->Append((void*)Rooms[i]);
					atleastoneaddedtoQue = true;
				}
			}
		}
	}
	struct Room* LastroominBFS = poppedroom;
	while( LastroominBFS->index !=startroomnum) {
		LastroominBFS = LastroominBFS->parent;
		BFS_maxlevel++;
	}
	struct Room* curroom = Rooms[endroomnum];
	while( curroom->index !=startroomnum) {
		/*if(curroom->parent == curroom) {
			no solution
		}*/
		updateWallwithApple(curroom,curroom->parent); //the wall between cur room and its parent should be an apple.
		BFS_shortestpath_cost += AdjMatrix[curroom->index][curroom->parent->index];
		curroom->apple = '@';
		curroom = curroom->parent;
	}
	Rooms[startroomnum]->apple='@';///coz curroom->index == startroomnum
}
/*
 * End code I did not write.
 */	



void updateWallwithApple(struct Room * room, struct Room * parentroom) {
	int i=0;
	for(i=0;i<totalnumofwalls;i++) {
		TWOROOMS pairofroom = getAdjacentRooms(Walls[i]);
		if((pairofroom.room2==room && pairofroom.room1==parentroom) || (pairofroom.room1==room && pairofroom.room2==parentroom))
			break;	
	}	
	Walls[i]->isalongpath = true;
}



My570ListElem * getMinDistRoom(My570List *list) {
	My570ListElem *elem=NULL;
	int mindi = INFINITY+1;
	struct Room *temp = NULL;
	My570ListElem *minroomElem=NULL;
	for (elem=list->First(); elem != NULL; elem=list->Next(elem)) {
        temp =(struct Room*)(elem->Obj());
		#ifdef DEBUG2
		fprintf(stdout,"%d\n",temp->index); 	
		#endif
		
		if(temp->dist < mindi) {
			mindi = temp->dist;
			minroomElem = elem;
		}
	}
	
	return minroomElem;
}

/**wiki source : pseudo code: http://en.wikipedia.org/wiki/Dijkstra's_algorithm
 1  function Dijkstra(Graph, source):
 2      for each vertex v in Graph:            // Initializations
 3          dist[v] := infinity ;              // Unknown distance function from source to v
 4          previous[v] := undefined ;         // Previous node in optimal path from source
 5      end for ;
 6      dist[source] := 0 ;                    // Distance from source to source
 7      Q := the set of all nodes in Graph ;
        // All nodes in the graph are unoptimized - thus are in Q
 8      while Q is not empty:                  // The main loop
 9          u := vertex in Q with smallest distance in dist[] ;
10          if dist[u] = infinity:
11              break ;                        // all remaining vertices are inaccessible from source
12          end if ;
13          remove u from Q ;
			if(u == endroom) break... found
14          for each neighbor v of u:          // where v has not yet been removed from Q. ///meaning already visited.
15              alt := dist[u] + dist_between(u, v) ;
16              if alt < dist[v]:              // Relax (u,v,a)
17                  dist[v] := alt ;
18                  previous[v] := u ;
19                  decrease-key v in Q;       // Reorder v in the Queue, - remove  v from queue and mark it as visited.
20              end if ;
21          end for ;
22      end while ;
23      return dist[] ;
24  end Dijkstra.
**/

/*
 * Begin code I did not write.
 * This code is derived after looking at the pseudo code in wikipedia: http://en.wikipedia.org/wiki/Dijkstra's_algorithm
 * 
 */
void mazesolvebyDijkstra() {  

	My570List *Dijkstralist=new My570List();	///<push everything into a queue
	for(int i=0;i<totalnumofrooms;i++){
		Rooms[i]->parent = NULL;
		(void)Dijkstralist->Append((void*)Rooms[i]);

	}
	Rooms[startroomnum]->dist = 0;					///> distance from source to source
	while(!Dijkstralist->Empty()) {
		My570ListElem *elem = getMinDistRoom(Dijkstralist);	///complexity here is o(n);
		struct Room* mindistroom = (struct Room*)(elem->Obj()); 
		if(elem==NULL) {
			fprintf(stderr,"(maze has no solution) :queu returned back NULL meaning que is empty\n");
			exit(-1);
		}
		if(mindistroom->dist==INFINITY) {
			break;	///< all remaining vertices are inaccessible from source.
		}
		Dijkstralist->Unlink(elem); ///remove min dist element from queue.
		mindistroom->isvisited = true;
		if(mindistroom->index==endroomnum) {	
			break;
		}
		for(int v=0;v<totalnumofrooms;v++){
			if(isConnected(mindistroom->index,Rooms[v]->index)) {
				if(!Rooms[v]->isvisited){
					int alt = mindistroom->dist + AdjMatrix[mindistroom->index][v];
					if(alt < Rooms[v]->dist) {
						Rooms[v]->dist = alt;
						Rooms[v]->parent = mindistroom;
					}
				}
			}	
		}
	}
	Dijkstra_shortestpath_cost = 0;
	struct Room* curroom = Rooms[endroomnum];
	while(curroom->parent!=NULL) {
		updateWallwithApple(curroom,curroom->parent); //the wall between cur room and its parent should be an apple.
		curroom->apple = '@';
		Dijkstra_shortestpath_cost +=  AdjMatrix[curroom->index][curroom->parent->index];
		curroom = curroom->parent;
		}
	Rooms[startroomnum]->apple='@';///coz curroom->index == startroomnum
}
/*
 * End code I did not write.
 */	



//Build a roomXroom matrix connecting each using union find.
void connectWalls() {
	for(int i=0;i<totalnumofwalls;i++) {
		#ifdef  DEBUG3
		fprintf(stdout,"i=%d, totalnumofwalls=%d",i,totalnumofwalls);
		#endif
		if(Walls[i]->isremoved) {
			TWOROOMS pairofroom = getAdjacentRooms(Walls[i]);
			AdjMatrix[pairofroom.room1->index][pairofroom.room2->index] = Walls[i]->cost;
			AdjMatrix[pairofroom.room2->index][pairofroom.room1->index] = Walls[i]->cost;
		}
	}
	#ifdef DEBUG1
	printAdjMatrix();
	#endif
}

void initializeRooms() {
	int totalrooms = (w*h);
	for(int i = 0;i< totalrooms;i++) {
		Rooms[i] = new struct Room;
		Rooms[i]->index  = i;
		Rooms[i]->parent = Rooms[i];
		Rooms[i]->rank = 0;
		Rooms[i]->apple = ' ';
		Rooms[i]->isvisited = false;
		Rooms[i]->dist = INFINITY;
	}
	for(int i=0; i<totalrooms;i++) {
		for(int j=0;j<totalrooms;j++) {
			if(i==j) {
				AdjMatrix[i][j] = INFINITY;///u cant reach a node from itself.. if u make the cost high then you can prevent a check in bfs
			}else {
				AdjMatrix[i][j] = INFINITY;
			}
		}
	}
	#ifdef DEBUG4
	int tonws= (w-1)*h + (h-1)*w;
	fprintf(stdout,"totalnumofwalls = %d = w-1*h h-1*w = %d = wallindex = %d\n",totalnumofwalls,tonws,wallindex);
	int m;
	cin>>m;
	#endif	
	
	
	updateStartRoomNum();
	updateEndRoomNum();
	
	#ifdef DEBUG4
		Rooms[startroomnum]->apple = '@';
		Rooms[endroomnum]->apple = '@';
	#endif
}

void updateStartRoomNum() {
	if(!strcmp( startroom.type,"top")) {
		startroomnum = startroom.num;
		startroomrow = 1;
		startroomcol = startroom.num+1;
	}else if(!strcmp( startroom.type,"bottom")) {
		startroomnum = ((h-1)*w) + startroom.num;
		startroomrow = h;
		startroomcol = startroom.num+1;
	}else if(!strcmp(startroom.type,"left")) {
		startroomnum = startroom.num*w; ///coz if left then num will hold col num so if col = 0 its 0 if col num =1 then its 1xw 2Xw so on..
		startroomrow = startroom.num+1;
		startroomcol = 1;
	}else if(!strcmp(startroom.type,"right")) {
		startroomnum = (startroom.num*w) + (w-1);
		startroomrow = startroom.num+1;
		startroomcol = w;
	}else {
		fprintf(stderr, "(malformed input)startroom, endroom not updated\n");
		exit(-1);
	}
}

void updateEndRoomNum() {
	if(!strcmp( endroom.type,"top")) {
		endroomnum = endroom.num;
		endroomrow = 1;
		endroomcol = endroom.num+1;
	}else if(!strcmp( endroom.type,"bottom")) {
		endroomnum = ((h-1)*w) + endroom.num;
		endroomrow = h;
		endroomcol = endroom.num+1;
	}else if(!strcmp(endroom.type,"left")) {
		endroomnum = endroom.num*w; ///coz if left then num will hold col num so if col = 0 its 0 if col num =1 then its 1xw 2Xw so on..
		endroomrow = endroom.num+1;
		endroomcol = 1;	
	}else if(!strcmp(endroom.type,"right")) {
		endroomnum = (endroom.num*w) +(w-1);
		endroomrow = endroom.num+1;
		endroomcol = w;
	}else {
		fprintf(stderr, "(malformed input)startroom, endroom not updated\n");
		exit(-1);
	} 
}

void printAdjMatrix() {
	int totalrooms = (w*h);
	for(int i=0; i<totalrooms;i++) {
		fprintf(stdout,"Room:%d ",i);
		for(int j=0;j<totalrooms;j++) {
			if(AdjMatrix[i][j]!=INFINITY)
				fprintf(stdout,"[%d]:%d ",j,AdjMatrix[i][j]);
		}
		fprintf(stdout,"\n");
	}
}

void buildWalls() {
/*
    + +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    | |     |       | | |       |   |
    + + + +-+-+ +-+ + + + + + +-+ +-+
    | | | |       | | |   | |   |   |
    + +-+ + + + + +-+ + +-+-+-+ +-+ +
    |       | | |       |           |
    + +-+-+ + + +-+ +-+ +-+-+-+-+ + +
    | |     | |   | |       |     | |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ +
	
    +@+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |@|     |       | | |@@@@@  |   |
    +@+ + +-+-+ +-+ + + +@+ +@+-+ +-+
    |@| | |@@@@@@@| | |@@@| |@@@|   |
    +@+-+ +@+ + +@+-+ +@+-+-+-+@+-+ +
    |@@@@@@@| | |@@@@@@@|      @@@@@|
    + +-+-+ + + +-+ +-+ +-+-+-+-+ +@+
    | |     | |   | |       |     |@|
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+@+
    Starts in room at row = 1 and column = 1.
    Ends in room at row = 4 and column = 16.
    Cost of shortest path = 24.
    Maximum level = 27.

*/
	#ifdef DEBUG1
	for(int i=0;i<linebufcount;i++) {
		fprintf(stdout,"%s\n",linebuf[i]);
	}
	#endif
	

	for(int i=0;i<linebufcount;i++) {
		#ifdef DEBUG1
		fprintf(stdout,"strlen(line:%d):%d -----> %s\n",i,strlen(linebuf[i]),linebuf[i]);
		#endif
		// ==============================================PROCESS 1st line==========================
		if(i==0) {
			searchLineforStartEnd(FIRSTLINE);
			continue;
		}else if(i==linebufcount-1) {
			searchLineforStartEnd(LASTLINE);	
			continue;
		}else {
			processmidwalls(i);
			continue;
		}
	}
	if(startfound==false || endfound==false) {
		fprintf(stderr,"(malformed input file):Less than 2 openings specified , Exactly 2 openings have to be specified\n");
		exit(-1);
	}
	totalnumofwalls = wallindex;	///update total num of walls which is (wxh-1) + W-1*h
	#ifdef DEBUG1
	int tonws= (w-1)*h + (h-1)*w;
	fprintf(stdout,"totalnumofwalls = %d = w-1*h h-1*w = %d = wallindex = %d\n",totalnumofwalls,tonws,wallindex);
	int m;
	cin>>m;
	#endif
	if(totalwallsremoved==((w*h)-1)) {
		isBFS = true;
		if(FoundnonzerocostinTYPE1) {
			fprintf(stderr,"(malformed input) Type2 maze, more than %d walls need to be removed\n",(w*h)-1);
			exit(-1);
		}
	}else if(totalwallsremoved >((w*h)-1)) {
		isBFS = false;
	}else {
		fprintf(stderr,"MAZE has no solution since atleast (wxh)-1 = (%d x %d)-1 = %d walls has to be removed\n",w,h,((w*h)-1));
		exit(-1);
	}
}
/*
    + +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    | |     |       | | |       |   |
    + + + +-+-+ +-+ + + + + + +-+ +-+
    | | | |       | | |   | |   |   |
    + +-+ + + + + +-+ + +-+-+-+ +-+ +
    |       | | |       |           |
    + +-+-+ + + +-+ +-+ +-+-+-+-+ + +
    | |     | |   | |       |     | |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ +
*/

///< i varies from [1 to linebufcount-2] since 0th and linebufcount-1th row are already processed in different ifelse block
void processmidwalls(int i) {
	int linelen = strlen(linebuf[i]);
	bool isvertical;
	///< i=1,3 5 7 9 ... all odd line numbers will contains | | | | | | vertical walls
	///< i=2,4 6 8 then it contains +-+-+-
	if(i%2==0)  { /// i = even number so it contains horizontal lines so verticalflag = false
		isvertical=false;
	}else {
		isvertical = true;
	}
	for( int j=0;j<linelen;j++) {
		#ifdef DEBUG2
		fprintf(stdout,"%c",linebuf[i][j]);
		#endif
		if(j==0) { ///< check if the wall on the left is opening or not
			if(linebuf[i][j] ==' ') {
				if(startfound) {
					if(endfound) {
						fprintf(stderr,"(malformed input file) More than 2 openings!!\n");
						exit(-1);
					}
					endfound = true;
					strcpy(endroom.type,"left");
					endroom.num = (i-1)/2;		///< always index,rownum minus 1 divided by 2 will give u the row num of room or wall.
				}else {
					startfound = true;
					strcpy(startroom.type,"left");
					startroom.num= (i-1)/2;				
				}
			}else if(linebuf[i][j]=='|' || linebuf[i][j]=='+') {
				if(isvertical) {
					if(linebuf[i][j]!='|') {
						fprintf(stderr,"(malformed input)- leftmost boundary isnt  |\n");
						exit(-1);
					}
				}else {
					if(linebuf[i][j]!='+') {
						fprintf(stderr,"(malformed input)- leftmost boundary isnt +\n");
						exit(-1);
					}				
				}
			}else {
				fprintf(stderr,"(malformed input)- leftmost boundary isnt + or |\n");
				exit(-1);
			}///<else do nothing since its really not a numbered wall.
		}else if(j==linelen-1) {///< check if the wall on the right is an opening or not
			if(linebuf[i][j] ==' ') {
				if(startfound) {
					if(endfound) {
						fprintf(stderr,"(malformed input file) More than 2 openings!!\n");
						exit(-1);
					}
					endfound = true;
					strcpy(endroom.type,"right");
					endroom.num = (i-1)/2;		///< always index,rownum minus 1 divided by 2 will give u the row num of room or wall.
				}else {
					startfound = true;
					strcpy(startroom.type,"right");
					startroom.num= (i-1)/2;				
				}				
			}else if(linebuf[i][j]=='|' || linebuf[i][j]=='+') {
				if(isvertical) {
					if(linebuf[i][j]!='|') {
						fprintf(stderr,"(malformed input)- rightmost boundary isnt  |\n");
						exit(-1);
					}
				}else {
					if(linebuf[i][j]!='+') {
						fprintf(stderr,"(malformed input)- rightmost boundary isnt +\n");
						exit(-1);
					}				
				}			
			}else {
				fprintf(stderr,"(malformed input)- rightmost boundary isnt + or |\n");
				exit(-1);
			}
		}else {///< for inner walls that stretch from.. j = [1 to linebuf-2]  01234
			///< VERTICAL WALL ROW CASE!!! | | | | | | | | | || | |  | | | | | | | | | | | | | | || | | | | | | | | |
			if(isvertical) {///if i=1 3 5 7 it contains:| | | | | | | | vertical walls
				///< note that walls are present at j = 012345678
				///< | | | | | 
				///< 012345678
				///so you see above that even num has walls and odd has spaces.. even num will have space or number or wall
				if(j%2==0) { // these are wall positions | these can be empty for type1 and these can be numbers for type 2
					if(linebuf[i][j]==' '){ //empty meaning its type 1
						int cost = 1;
						Walls[wallindex] = new struct Wall; 
						Walls[wallindex]->index = wallindex;	
						Walls[wallindex]->isvertical = true;
						Walls[wallindex]->isremoved = true;
						Walls[wallindex]->cost = cost;	
						Walls[wallindex]->isalongpath = false;
						wallindex++;
						totalwallsremoved++;
					}else if(linebuf[i][j]=='|') { ///< meaning the wall isnt removed...
						Walls[wallindex] = new struct Wall;
						Walls[wallindex]->index = wallindex;	
						Walls[wallindex]->isvertical = true;
						Walls[wallindex]->isremoved = false;
						Walls[wallindex]->cost = INFINITY;	
						Walls[wallindex]->isalongpath = false;
						wallindex++;	
					}else {/// type2 ///< may be we shud check if its a number!!
						char a[3];
						a[0]=linebuf[i][j];
						a[1]='\0';
						int cost = atoi(a);
						Walls[wallindex] = new struct Wall;
						Walls[wallindex]->index = wallindex;	
						Walls[wallindex]->isvertical = true;
						Walls[wallindex]->isremoved = true;
						Walls[wallindex]->cost = cost;	
						Walls[wallindex]->isalongpath = false;
						wallindex++;	
						totalwallsremoved++;
						FoundnonzerocostinTYPE1 = true;
					}
				}else { ///these MUST BE empty positions because in vertical-wall-row odd j value will have spaces...if not then throw error
					if(linebuf[i][j]!=' ') {
						fprintf(stderr,"(malformed input)- A room in the input file wasnt empty when it was expected to be\n");
						exit(-1);
					}
				}		
			}else {///<   HORIZONTAL WALL ROW CASE!!-------------if i = 2 4 6 8 then it contains +-+-+-
				///< +-+-+-+-+
				///< 012345678
				///< so u see that even has + and odd has walls '-' or ' ' (Type1) or 'cost'(type2)
				if(j%2==0) {
					if(linebuf[i][j]!='+') {
						fprintf(stderr,"(malformed input)- A room in the input file dint contain + when it was expected to have it\n");
						exit(-1);					
					}	
				}else{///if odd then check for ' ' '-' or 'cost'
					if(linebuf[i][j]==' ') {
						int cost = 1;
						Walls[wallindex] = new struct Wall;
						Walls[wallindex]->index = wallindex;	
						Walls[wallindex]->isvertical = false;
						Walls[wallindex]->isremoved = true;
						Walls[wallindex]->cost = cost;	
						Walls[wallindex]->isalongpath = false;
						wallindex++;					
						totalwallsremoved++;
					}else if(linebuf[i][j]=='-') {
						Walls[wallindex] = new struct Wall;
						Walls[wallindex]->index = wallindex;	
						Walls[wallindex]->isvertical = false;
						Walls[wallindex]->isremoved = false;
						Walls[wallindex]->cost = INFINITY;	
						Walls[wallindex]->isalongpath = false;
						wallindex++;					
					}else { ///< TYPE2 MAZE with cost----may be you should do a check if its a number only!!
						char a[10];
						a[0]=linebuf[i][j];
						a[1]='\0';
						int cost = atoi(a);
						Walls[wallindex] = new struct Wall;
						Walls[wallindex]->index = wallindex;	
						Walls[wallindex]->isvertical = false;
						Walls[wallindex]->isremoved = true;
						Walls[wallindex]->cost = cost;	
						Walls[wallindex]->isalongpath = false;
						wallindex++;		
						totalwallsremoved++;
						FoundnonzerocostinTYPE1 = true;
					}
				}
			}
		}
	}
}

void searchLineforStartEnd(int linetype) {
	char typestr[100];
	int i;
	if(linetype==FIRSTLINE) {
		strcpy(typestr,"top");
		i=0;
	}else if(linetype==LASTLINE) {
		strcpy(typestr,"bottom");
		i =linebufcount-1;
	}else {
		fprintf(stderr,"void searchLineforStartEnd(%d):invalid line type passed...should be 0 or 1",linetype);
		exit(-1);
	}

	int dashwallcount = 0;

	for(unsigned int j=0;j<strlen(linebuf[i]);j++) {
		#ifdef DEBUG2
		fprintf(stdout,"%c",linebuf[i][j]);
		#endif
		if(linebuf[i][j]=='+') {
			continue;
		}else if(linebuf[i][j]=='-') {
			dashwallcount++;
			continue;
		}else if(linebuf[i][j]==' ') {
			if(startfound) {
				if(endfound) {
					fprintf(stderr,"(malformed input file) More than 2 openings!!\n");
					exit(-1);
				}
				endfound = true;
				strcpy(endroom.type,typestr);
				endroom.num= dashwallcount;
			} else {
				startfound = true;
				strcpy(startroom.type,typestr);
				startroom.num= dashwallcount;						
			}		
			dashwallcount++;
			continue;
		}else  {
			fprintf(stderr,"(malformed input)- %s line has invalid char other than + and - \n",typestr);
			exit(-1);
		}
	}
	#ifdef DEBUG1
	fprintf(stdout,"dashwallcount=%d\n",dashwallcount);
	#endif	
	
	if(dashwallcount!=w) {
		fprintf(stderr,"(malformed input file)- total number of - walls on %s line of input :%d is not matching width %d\n",typestr,dashwallcount,w);
		exit(-1);
	}

}


void processInput() {
	if(file==NULL){
		readFile(cin);///< read from standard input
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
		readFile(in);
		in.close();		
	}
}

/***
IMP::
maze:
- It must have odd number of lines. 
- Each line in the input must have odd number of characters and all lines must have the same length.
- Finally, it has to be a valid maze in the sense that all rooms are connected. 
  Once you convert an input into a maze, the size of the maze must be within the limits of the mazegen command.

*/
void readFile(istream& in) {
	
	
	char* buf = new char[MAXBUFSIZE];
	in.getline(buf,MAXBUFSIZE);
	int linelength = strlen(buf);
	while (!in.eof()) {
	
		#ifdef DEBUG1
		fprintf(stdout,"line read: %s\n",buf);
		#endif
		
		strcpy(linebuf[linebufcount++],buf);
		in.getline(buf,MAXBUFSIZE);
		int len = strlen(buf);
		if(!in.eof()) {
			if(linelength!=len) {
				fprintf(stderr,"(malformed input file)-Not all lines are of the same length in input file %d != %d\n",linelength,len);
				exit(-1);
			}	
		}
	}
	#ifdef DEBUG1
	fprintf(stdout,"EOF REACHED\n");
	#endif

	
	if(linebufcount%2==0) {
		fprintf(stderr,"(malformed input file): even number of lines found\n");
		exit(-1);
	}
	
	if(linebufcount==1) {
		fprintf(stderr,"(malformed input file): just one line found in input\n");
		exit(-1);
	}
	
	int widthtest = strlen(linebuf[0]);
	if(widthtest%2==0) {
		fprintf(stderr, "(malformed input file):even number of characters found on the input line\n");
		exit(-1);
	}
	updateheightwidth();
}


void updateheightwidth() {
	unsigned int linewidth = strlen(linebuf[0]);
	w = (linewidth-1)/2;
	h = (linebufcount-1)/2;
	totalnumofrooms = w*h;

	strcpy(startroom.type,"UNDEFINED");
	startroom.num = -1;
	strcpy(endroom.type,"UNDEFINED");
	endroom.num = -1;	
}

/****Some simple explainations and design rules for maze solving:
1. The open end and the close end walls will have an apple so its easy to print it [may have to modify display funciton a bit]
2. we already have the function tat returns two rooms given a wall. so we know which two rooms are connected (We don need the othe way around1!)
3. since we need to traverse bfs, we first need  a graph, so we first need a totalroomsX totalrooms matrix which says which all rooms are connected. 
with the connection having a cost value. ( so we would update the array while reading the input file itself as to whats the cost of connecting two rooms
4.when reading file:
keep going and checking which wall is empty or not so in one traversal of the file we know the whole wall contents.
now use this wall info to update all the totatlroomXtotalroom matrix.. since for a given wall we know what room it connects 
5. note that for MAZETYPE2: all costs are between [0-9] so instead of empty space you get a number
REmemebr that room-struct has not much info except that given an index, we know if it contains an apple or not.
we don really need to store nething else.


IMP::
maze:
- It must have odd number of lines. 
- Each line in the input must have odd number of characters and all lines must have the same length.
- Finally, it has to be a valid maze in the sense that all rooms are connected. 
  Once you convert an input into a maze, the size of the maze must be within the limits of the mazegen command.

so our code would be like.

build walls
initialize rooms to -'noapple'
build matrix
displaymaze - unsolved
solvemaze
displaymazeagain.
***/

void processargs_ms(int argc, char* argv[]) {

		if (argc > 3) { ///< max is 8 args eg: hw4 mazegen 2 1 -info -top=1 -bottom=1 rndfilename
			fprintf(stderr,"(malformed command) - too many  arguments\n");
			mazesolveUsage();
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
}


void mazesolveUsage() {
	fprintf(stderr,"Usage:\n");
	fprintf(stderr,"\thw4 mazesolve [file]\n");
	exit(-1);	
}



void displaymaze_solve() {
	/*----------------------------------Print the first line---------------------------------------*/
	//print the first line remove the horizontal wall if top is start.top [1-h] ( i=0 to w +-
	for(int i = 0;i<w;i++) {
		fprintf(stdout,"+");
		if(!strcmp(startroom.type,"top") || !strcmp(endroom.type,"top") ) {
		
			if(!strcmp(startroom.type,"top")){
				if((startroom.num)==i)
					fprintf(stdout,"@");
				else
					fprintf(stdout,"-");
			}
			if(!strcmp(endroom.type,"top") ) {
				if((endroom.num)==i)
					fprintf(stdout,"@");
				else
					fprintf(stdout,"-");			
			}
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

			if(!strcmp(startroom.type,"left") || !strcmp(endroom.type,"left") ) {
			
				if(!strcmp(startroom.type,"left")) {
					if(startroom.num==colcount) {
						fprintf(stdout,"@");
					} else {
						fprintf(stdout,"|");
					}
				}	
				if(!strcmp(endroom.type,"left")) {
					if(endroom.num==colcount) {
						fprintf(stdout,"@");
					} else {
						fprintf(stdout,"|");
					}
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
				fprintf(stdout,"%c",s_Wallchar(Walls[k]));
				counter++;
				k++;
			}
			fprintf(stdout,"%c",Rooms[roomnum++]->apple);
			if(!strcmp(startroom.type,"right") || !strcmp(endroom.type,"right") ) {
			
				if(!strcmp(startroom.type,"right")) {	
					if(startroom.num==colcount) {
						fprintf(stdout,"@");
					} else {
						fprintf(stdout,"|");
					}
				}
				if(!strcmp(endroom.type,"right")) {	
					if(endroom.num==colcount) {
						fprintf(stdout,"@");
					} else {
						fprintf(stdout,"|");
					}
				}				
				
				
			} else {
				fprintf(stdout,"|");
			}
			fprintf(stdout,"\n");
			//k=k+w;
			
			colcount++;
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
				fprintf(stdout,"%c",s_Wallchar(Walls[k]));
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
		if(!strcmp(startroom.type,"bottom") || !strcmp(endroom.type,"bottom") ) {
		
			if(!strcmp(startroom.type,"bottom")) {	
				if((startroom.num)==i)
					fprintf(stdout,"@");
				else
					fprintf(stdout,"-");
			}
			if(!strcmp(endroom.type,"bottom")) {	
				if((endroom.num)==i)
					fprintf(stdout,"@");
				else
					fprintf(stdout,"-");
			}			
		} else {
			fprintf(stdout,"-");
		}
	}
	fprintf(stdout,"+\n");
	/*--------------------------------------------------------------------------------------------*/
	
	if(isBFS) {
		fprintf(stdout,"Starts in room at row = %d and column = %d.\n",startroomrow,startroomcol);
		fprintf(stdout,"Ends in room at row = %d and column = %d.\n",endroomrow,endroomcol);
		fprintf(stdout,"Cost of shortest path = %d.\n",BFS_shortestpath_cost);
		fprintf(stdout,"Maximum level = %d.\n",BFS_maxlevel);
	}else {
		fprintf(stdout,"Starts in room at row = %d and column = %d.\n",startroomrow,startroomcol);
		fprintf(stdout,"Ends in room at row = %d and column = %d.\n",endroomrow,endroomcol);
		fprintf(stdout,"Cost of shortest path = %d.\n",Dijkstra_shortestpath_cost);

	}

}


char s_Wallchar(WALL *temp) {
	if(temp == NULL) {
		fprintf(stderr,"s_Wallchar(NULL): A NULL STRUCT PASSED TO FUNCTION!\n");
		exit(-1);
	}

	if(temp->isremoved || temp->isalongpath) {
		if(temp->isalongpath)
			return '@';
		else
			return ' ';
	}
	if(temp->isvertical) {
		return '|';
	} else 
		return '-';
}


