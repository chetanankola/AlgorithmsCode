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
#include "util.hpp" 

using namespace std; 

void sudokusolve(int argc, char* argv[]);
void sudokusolveUsage();
void processInput_solve(int argc, char*argv[]);	///< check if input file exists if so read from file else from stdinput
void readFile_solve(istream& in);	///puts input file into filebuf[][]
void readValidLines();
void displaysudoku(char arr[][MAXCOL]);
void displaysudoku_int(int arr[][MAXCOL]);
void checkvalidity(char arr[][MAXCOL]);
void copySudokuInputtoIntArr();
bool is_Assigned(int num);
bool NoConflict(int input[][MAXCOL], int row, int col,int num);
bool solve_by_backtrack(int input[][MAXCOL]);
bool FindFreeSlots(int input[][MAXCOL],int *row,int *col);
bool isSolved(int arr[][MAXCOL]);
void GenerateBoard();
/*
My570ListElem * getMinDistRoom(My570List *list);
*/

#endif

