#ifndef _MAZEGEN_H
#define _MAZEGEN_H


#include <string.h>
#include <stdint.h>
#include <fstream>
#include <sys/time.h>
#include <dirent.h>


#include <sys/stat.h>
#include "cs570.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
using namespace std;  

#define MAXBUFSIZE 5096 

typedef struct Room {
	struct Room * parent;
	int rank;
	int index;
	char apple;
	bool isvisited;
	int dist;
}ROOM;



typedef struct Wall {
	bool isvertical;
	int index;
	bool isremoved;
	int cost ;
	bool isalongpath;
}WALL;

///< PairofRoom is a struct that is needed to be returned when you have 
///< to find which two rooms a wall is
///< connecting 
typedef struct PairofRoom {
	ROOM * room1;
	ROOM * room2;
}TWOROOMS;

void mazegen(int argc, char* argv[]);
void mazegenUsage();
bool isInfoSet(int argc, char* argv[]);
void processArgs(int argc, char* argv[]);
bool isarganoption(char * arg);
int processoption(char* optarg);
void checkboundsforStartEnd();

void initializeWallsRooms();
void readRndFile();	
void test_removeRandomWalls();
void displaymaze();

struct Room* Find(struct Room* x);
bool isUnion(struct Room* x, struct Room* y);

///<Misc functions
char Wallchar(struct Wall *temp);
struct PairofRoom getAdjacentRooms(struct Wall* w);
int getrownum(int wallnum);
int getleftroom(int wallnum); ///< returns the left room num for a given vertical hall
int getupperroom(int wallnum);///< returns the room on top of the wall wallnum


#endif








