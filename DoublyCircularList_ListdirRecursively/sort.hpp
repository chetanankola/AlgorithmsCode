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

#define MAXBUFSIZE 5096

struct record {
	char sign[2];
	unsigned int timestamp;
	double amount;
	char leftamountString[256];
	char rightamountString[256];
	char description[256];		///< MAX SHOULD BE 24
	int recordnum;
};


void Sort(int, char**);
void SortUsage();
void readfromStdin();
void readfromTfile(char *filename);	
void ProcessFilenameFailure(char * filename);
void Process(istream& in);
int isDir(char* str);
struct record* isRecordOk(char* buf,int count);
int isInputanInteger(char []);
void displayRecords(My570List * list);
void SortedInsert(My570List *list,struct record* temp_str);	
void tableDisplay(My570List *list);
void displayDate(unsigned int timestamp);
void displayDescription(char * description);
void displaySpace(int num);
void displayAmount(struct record* temp);
double displayBalance(double balance,struct record* temp);
void displayLeft(char* left);
int addCommas(char* amountstr);
void displaybal(double balance);
char *nls_ftoa(double number, int width, int precision);
















