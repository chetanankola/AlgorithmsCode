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
#include "my570BST.hpp"
using namespace std;  

#define MAXBUFSIZE 5096 

void BST(int argc, char* argv[],int operation);
void processBSTArguments(int argc,char* argv[],int operation);
void BSTUsage();
extern bool isDisplaySet(int argc, char* argv[]);
extern void processError_lessargs(int argc, char* argv[]);


void BST_Insertion(int argc,char* argv[]);
void BSTInsertfromStdin();
void BSTInsertfromFile(char* filename); 

void BST_Deletion(int argc,char* argv[]);
void BSTDeletionFileInput(char *filename,char deletionstrArr[][MAXBUFSIZE],int numofdelstr);
My570BST* ProcessBSTDeletion(istream& in);

void processError_lessargs(int argc, char* argv[]);
My570BST* ProcessBSTInsertion(istream& in);





