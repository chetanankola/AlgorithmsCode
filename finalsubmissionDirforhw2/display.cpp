#include "display.hpp"
#include "cs570.h"
#include "my570list.hpp"
#include <stdlib.h>
#include "sort.hpp"

/**
Algorithm:
Traverse(path,depth) {
	if(path has no more dir) 
		print all the files
			return;
			
	while(end of dir path)
		print dir;
		Traverse(dirpath,depth+1)
}
**/

/**
 * Function will take the @param1 and prepend "+-- " to @param1 if @param2 = 0
 * function will return strcat("+-- ",@param1) or strcat("/-- ",@param1)
 */
char* prependstyle(char* entry,int islast) {
	if( strlen(entry)== 0 ) {
		#ifdef DEBUG0
			fprintf(stdout,"char* appendstyle(char* entry): strlen(entry) = 0\n");
			exit(-1);
		#endif
	}
	char* newstr = new char[255];
	if( islast ) {
		strcpy(newstr,"\\-- "); 
	} else {
		strcpy(newstr,"+-- "); 
	}
	strcat(newstr, entry);
	return newstr;

}

int hasReadAccess(char* path) {
	//Check permissions here
	if( access( path, R_OK ) == -1 ) {
		#ifdef DEBUG1
		fprintf(stdout,"file/path %s has no Read permissions\n",path);
		#endif		
		return 0;
	} 
	return 1;
}

//the path here always has a leading slash
void TraverseDir(char* path, int depth, int lastdircount,char* formatstr) {
	char temppath[MAXPATHLENGTH];
	strcpy(temppath,path);
	DIR *dir;
	struct dirent *str_tempdir;

	dir = opendir(path);	
	My570List *list=new My570List();
	while ((str_tempdir = readdir(dir)) != NULL) {
		if(!strcmp(str_tempdir->d_name,".") || !strcmp(str_tempdir->d_name,"..")){
			continue;
		}
		char *tempstr = new char[256]; 		///< TODO RELEASE MEMORY AFTER PRINTING IS DONE
		strcpy(tempstr,str_tempdir->d_name);
		SortedInsertAscend(list,tempstr);
	}
	closedir(dir);

	My570ListElem *elem=NULL;
	for (elem=list->First(); elem != NULL; elem=list->Next(elem)) {
		int entrytype = NOTLASTENTRY;
		
		if(elem == list->Last()) {
			entrytype = LASTENTRY; 						///< LASTENTRY  = 1;
		}
        char *temp_name =(char*)(elem->Obj());
		char *fullpath = buildfullpath(path,temp_name);
		if(isDirectory(fullpath)) {						///< IF DIRECTORY			
			strcat(temp_name,"/");						///< catenate / to make it dir
			temp_name = prependstyle(temp_name,entrytype );
			//DisplayEntry(temp_name,depth);
			//temp_name = addFormat(temp_name,depth,entrytype,lastdircount);
			if(!hasReadAccess(fullpath)) {				///< IF NO ACCESS PERMS 
				strcat(temp_name,"/");
				fprintf(stdout," %s%s\n",formatstr,temp_name);	
				continue;
			}
			fprintf(stdout," %s%s\n",formatstr,temp_name);								//////<PRINT
			///< RECURSE AGAIN
			if(entrytype == LASTENTRY) {
				char fivespace[256];
				strcpy(fivespace,"     ");
				char* finalstr = new char[256];
				strcpy(finalstr,formatstr);
				strcat(finalstr,fivespace);
				TraverseDir(fullpath,depth+1,lastdircount+1,finalstr);
			} else {
				char fourslash[256];
				strcpy(fourslash,"|    ");
				char* finalstr = new char[256];
				strcpy(finalstr,formatstr);
				strcat(finalstr,fourslash);
				TraverseDir(fullpath,depth+1,lastdircount,finalstr);
			}
		} else {																///< IF FILE
			temp_name = prependstyle(temp_name,entrytype );
			//DisplayEntry(temp_name,depth);
			//temp_name = addFormat(temp_name,depth,entrytype,lastdircount);
			fprintf(stdout," %s%s\n",formatstr,temp_name);									//////<PRINT
		}
	}
}

/// prepend(abc, ankola)   returns ankolaabc
char* myprefix(char* str, char* prependstr) {
	char * tmp = new char[256];

	strcpy(tmp,prependstr);
	strcat(tmp,str);
	return tmp;
}
///< the string that comes here is of the type
char* addFormat(char* entry,int depth,int islast,int lastdircount) {

cout<<"~~"<<depth<<"~~"<<lastdircount;
	char fourspace[256];
	char fivespace[256];
	char slash[10];
	char slashfour[256];
	
	strcpy(fourspace,"    ");
	strcpy(fivespace,"     ");
	strcpy(slash,"|");
	strcpy(slashfour,"");
	strcat(slashfour,slash);
	strcat(slashfour,fourspace);
	entry = prependstyle(entry,islast );	///< adds "+-- "
	if( strlen(entry)== 0 ) {
		#ifdef DEBUG0
			fprintf(stdout,"char* addFormat(char* entry,int depth,int islast): strlen(entry) = 0\n");
			exit(-1);
		#endif
	}
	char* newstr = new char[255];
	strcpy(newstr," ");

	if(depth==0) {
		strcat(newstr, entry);
		return newstr;
	}
	
/*	if(islast) {
		for(int i=0;i<depth-lastdircount;i++ ) {
			strcat(newstr,slashfour);
		}		
		for(int i=0;i<lastdircount;i++ ) {
			strcat(newstr,fivespace);
		}
	} else*/	
	if(lastdircount==0){
		for(int i=0;i<depth-lastdircount;i++ ) {
			strcat(newstr,slashfour);
		}
		for(int i=0;i<lastdircount;i++ ) {
			strcat(newstr,fivespace);
		}		
	}
	
	if(lastdircount>0){
		for(int i=0;i<lastdircount;i++ ) {
			strcat(newstr,fivespace);
		}
		for(int i=0;i<lastdircount-1;i++ ) {
			strcat(newstr,slashfour);
		}
	}
	strcat(newstr, entry);
	return newstr;
}



void ProcessDisplay(char* path) {
	ProcessPathFailure(path);		///<will exit in case of any arguments errors
	#ifdef DEBUG1
	if(pathhasfSlash(path) )
		fprintf(stdout,"path %s has trailing forward slash\n",path);
	#endif	
	
	if( !pathhasfSlash(path) ) {
		int len = strlen(path);
		if(len>0){
			if(  !((strcmp(path,".")==0 ||strcmp(path,"..")==0) ) )  {
				strcat(path,"/");
			}
		}	
	}
	fprintf(stdout,"%s\n",path);
	#ifdef DEBUG1
	////< TESTING  getLastEntrynamefromPath(path) function
	char* lasttoken = getLastEntrynamefromPath(path);
	fprintf(stdout,"lasttoken = %s\n",lasttoken);
	#endif

	
	#ifdef DEBUG1
	////< TESTING appendstyle(path,islast) function
	char* styletoken1 = 	appendstyle(path,0);	///< islast = false
	fprintf(stdout,"newstyle =|%s|\n", styletoken1);
	char* styletoken2 = 	appendstyle(path,1);
	fprintf(stdout,"newstyle =|%s|\n", styletoken2);///<islast = true;
	#endif
	
	char emptystr[256];
	strcpy(emptystr,"");
	TraverseDir(path,0,0,emptystr);			///< first time passs the depth as zero
}

void DisplayEntry(char* entry, int depth) {
	displaySpace(1);	
	for(int i = 0;i<depth;i++) {
		
		fprintf(stdout,"|");
		displaySpace((5)-1);
	}
	fprintf(stdout,"%s\n",entry);
}


char* getLastEntrynamefromPath(char* path) {
	if(strlen(path)<=0) {
		fprintf(stderr,"getEntrynamefromPath(path): strlen(path) = 0 ..Exit\n");
		exit(-1);
	}
	
	char* LastToken = new char[256];
	char* temppath = new char[256];
	strcpy(temppath,path);

	char* token = strtok(temppath,"/");
	int count = 0;
	while(token!=NULL) {
		#ifdef DEBUG1
		fprintf(stdout,"DEBUG:%s",token);
		#endif
		strcpy(LastToken,token);
		token = strtok(NULL,"/");
		count++;
	}
	if(count<=0) {
		fprintf(stderr,"getEntrynamefromPath(path):No tokens found for strtok(path,/) ..Exit\n");
		exit(-1);
	}	
	return LastToken;
}

void SortedInsertAscend(My570List *list,char* new_str) {
	if(list->Empty()) {
		#ifdef DEBUG1
			fprintf(stdout,"Emptylist:%s was inserted first\n",new_str);
		#endif
		(void)list->Append((void*)new_str);
		return;
	}
	My570ListElem *cur_elem = list->First();
	char *cur_str;
	while(cur_elem!=NULL) {
		cur_str =(char *)(cur_elem->Obj());
		#ifdef DEBUG1
			fprintf(stdout,"cur_str=%s\n",cur_str);
		#endif			
		if( strcmp(new_str,cur_str)<0){ 	///< 1 2 3 4 5  if new is 6
			 (void)list->InsertBefore(new_str, cur_elem);
			 	#ifdef DEBUG1
					fprintf(stdout,"%s was inserted before %s\n",new_str,cur_str);
				#endif	
			 return;
		}
		cur_elem = list->Next(cur_elem);
	}
	if(cur_elem == NULL) { 					///< meaning reached the end of the list
		(void)list->InsertAfter(new_str,cur_elem);
		#ifdef DEBUG1
			fprintf(stdout,"string %s was inserted at the end\n",new_str);
		#endif	
	}
}



char* buildfullpath(char* path, char* str) {

	char* newpath = new char[MAXPATHLENGTH];	
	strcpy(newpath,path);
	///< add a slash if the path has no slash 
	if(!pathhasfSlash(path)) {
		strcat(newpath,"/");
	}
	strcat(newpath,str);
	return newpath;
}

int pathhasfSlash(char* str) {
	
	int len = strlen(str);
	if(len == 0 ) 
		return 0;
	if(str[len-1]=='/') {
		return 1;
	} else {
		return 0;
	}
}



void Display(int argc, char* argv[]) {
	if(argc==2) {
		fprintf(stderr,"(too less arguments)\n");
		exit(-1);
	}
	
	if(argc==3) {
		ProcessDisplay(argv[2]);		///<argv[2] should be a filename;
		return;
	}
	DisplayUsage();
}
/*
 * Begin code I did not write.
 * This code is partly derived from http://stackoverflow.com/questions/4989431/how-to-use-s-isreg-and-s-isdir-posix-macros
 * If the source code requires you to include copyright, put copyright here.
 */
int isDirectory(char* str) {
	struct stat stat_str;
	stat( str, &stat_str);
	if(S_ISDIR( stat_str.st_mode )) {
		return 1;		
	} else {
		return 0;
	}
}
/*
 * End code I did not write.
 */	

void DisplayUsage() {
	fprintf(stderr, "(malformed command)\n");
	fprintf(stderr,"Usage:\n");
	fprintf(stderr,"\thw2 display topdir \n");
	fprintf(stderr,"\twhere topdir is the path name of the directory to be recursed\n");
	exit(-1);
}



void ProcessPathFailure(char * filename) {
	if(strlen(filename)!=0) {
		if(filename[0]=='-') {
			fprintf(stderr,"(malformed command)\n");
			exit(-1);
		}
	}
	
	if(!isDirectory(filename)) {
		fprintf(stderr,"(input file: %s is not a directory)\n",filename);
		exit(-1);
	}
	
	if( access( filename, F_OK ) != -1 ) {
		#ifdef DEBUG1
		fprintf(stdout,"File exist\n");
		#endif
	} else {
		fprintf(stderr,"Input file/path %s does not exist\n",filename);
		exit(-1);
	}

	if( access( filename, R_OK ) != -1 ) {
		#ifdef DEBUG1
		fprintf(stdout,"file has Read permissions\n");
		#endif
	} else {
		fprintf(stderr,"input file/path %s cannot be opened - access denied\n",filename);
		exit(-1);
	}

}
