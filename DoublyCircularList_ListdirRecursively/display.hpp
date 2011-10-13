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

#define NOTLASTENTRY 0
#define LASTENTRY 1

using namespace std;

void Display(int,char**);
void ProcessPathFailure(char *);
void DisplayUsage();
void ProcessDisplay(char*);
int  isDirectory(char*);
void TraverseDir(char* path,int depth,int lastdircount,char* formatstr);
int pathhasfSlash(char* str);
char* buildfullpath(char* path, char* str);
void SortedInsertAscend(My570List *list,char* new_str);
char* getLastEntrynamefromPath(char* path);
void DisplayEntry(char* entry, int depth);
char* prependstyle(char* entry,int islast);
char* addFormat(char* entry,int depth,int islast,int lastdircount);
int hasReadAccess(char* path);
char* myprefix(char* str, char* prependstr);


