#ifndef _MAZESOLVE_H
#define _MAZESOLVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <dirent.h>
#include <sys/stat.h>
#include "cs570.h"
#include "my570list.hpp"

using namespace std; 

void mazesolve(int argc, char* argv[]);
void mazesolveUsage();
void processargs_ms(int argc,char* argv[]);
void readFile(istream& in);
void processInput();
void buildWalls();
void updateheightwidth();
void searchLineforStartEnd(int linetype); 
void processmidwalls(int i);
void displaymaze_solve();
void initializeRooms();
void printAdjMatrix();
void connectWalls();
void updateStartRoomNum();
void updateEndRoomNum();
void mazesolvebyBFS();
void mazesolvebyDijkstra();
bool isConnected(int room1index,int room2index);
char s_Wallchar(struct Wall* temp);
void updateWallwithApple(struct Room * room, struct Room * parentroom);

My570ListElem * getMinDistRoom(My570List *list);


#endif

