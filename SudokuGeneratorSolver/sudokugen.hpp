#ifndef _SUDOKUGEN_H
#define _SUDOKUGEN_H


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
#include "util.hpp" 
#include "cs570.h"
#include "my570list.hpp"

using namespace std;  

#define MAXBUFSIZE 5096

typedef struct Cell{
	int value;
	int row;
	int col;
}CELL;
void sudokugen(int argc, char* argv[]); 

void sudokugenUsage(); 
void processcmdline_gen(int argc, char* argv[]);
void readFile_gen(istream& in);
void processoption_gen(char* optarg);
void processFile_rndfile();
void initialize_solved_puzzle();
void print_int_sudoku(int arr[][MAXCOL]);
My570List* ShuffleList(My570List *input);
My570ListElem *getElem(My570List *list,int R);
void OnRandomShuffle();
int RandomIndex(int L);
void updateChar();
bool BoardGeneration(int input[][MAXCOL]);
void GenerateRegularSudoku();
int getRowofCell(int cellnum);
int getColofCell(int cellnum);
int getRnd(int L);
void displaylist(My570List* templist);
void copymatrix(int src[][MAXCOL], int dest[][MAXCOL]);
bool solve_by_backtrack_gen(int input[][MAXCOL]);
#endif








