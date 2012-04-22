#ifndef _SUDOKUCOUNT_H
#define _SUDOKUCOUNT_H


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
void sudokucountUsage(); 
void sudokucount(int argc, char* argv[]);
void processcmdline_count(int argc, char* argv[]);
void readFile_count(istream& in);
bool isarganoption(char * arg);
int processoption(char* optarg);
void processFile();
bool solve_by_backtrack_count(int input[][MAXCOL]);

#endif
