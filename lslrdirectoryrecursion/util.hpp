#ifndef _MY_UTIL_H_
#define _MY_UTIL_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;

char* prepend(char *str, int num);
char* myItoa(int num);
int myatoi(char *s);
void displaySpace(int num);
void ProcessFilenameFailure(char * filename);
int isDir(char* str);
void myTrim(char* buf); 
void myTrailTrim(char* buf);





#endif

