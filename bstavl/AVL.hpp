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



void AVL(int argc, char* argv[],int operation);
void processAVLArguments(int argc,char* argv[],int operation);
void AVLUsage();
void AVL_Insertion(int argc,char* argv[]);
void AVL_Deletion(int argc,char* argv[]);
extern bool isDisplaySet(int argc, char* argv[]);
extern void processError_lessargs(int argc, char* argv[]);
void AVLInsertfromStdin();
void AVLInsertfromFile(char* filename);

My570BST* ProcessAVLInsertion(istream& in);

void AVLDeletionFileInput(char *filename,char deletionstrArr[][MAXBUFSIZE],int numofdelstr);
My570BST* ProcessAVLDeletion(istream& in); 



