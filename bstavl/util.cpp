#include "util.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <dirent.h>
#include <sys/stat.h>



/*
 * Begin code I did not write.
 * This code is partly derived from http://stackoverflow.com/questions/4989431/how-to-use-s-isreg-and-s-isdir-posix-macros
 * If the source code requires you to include copyright, put copyright here.
 */
void ProcessFilenameFailure(char * filename) {

	if(strlen(filename)!=0) {
	
		if(filename[0]=='-') {
			fprintf(stderr,"(malformed command)\n");
			exit(-1);
		}
	}
	
	if( access( filename, F_OK ) != -1 ) {
		#ifdef DEBUG2
		fprintf(stdout,"file exist\n");
		#endif
	} else {
		fprintf(stderr,"input file/path %s does not exist\n",filename);
		exit(-1);
	}
	
	if( access( filename, R_OK ) != -1 ) {
		#ifdef DEBUG2
		fprintf(stdout,"file has Read permissions\n");
		#endif
	} else {
		fprintf(stderr,"input file/path %s cannot be opened - access denied\n",filename);
		exit(-1);
	}
	
	if(isDir(filename)) {
		fprintf(stderr,"(input file: %s is a directory)\n",filename);
		exit(-1);
	}
}
/*
 * End code I did not write.
 */	

/*
 * Begin code I did not write.
 * This code is partly derived from http://stackoverflow.com/questions/4989431/how-to-use-s-isreg-and-s-isdir-posix-macros
 * If the source code requires you to include copyright, put copyright here.
 */ 
 /*	#ifdef DEBUG2
	char a[100];
	strcpy(a,"");
	cout<<a<<endl;
	myTrim(a);
	#endif
	*/
 
void myTrailTrim(char* buf) {
	///< trailing spaces
	///<"1234chetan" where 1234 are 4 spaces.
	/////0123456789 len = 10 numoftrailspace = 4 
	 char* ptr2 = buf;
	 int i=0;
	 int len = strlen(buf);
	 while((isspace(ptr2[i]) || ptr2[i]=='\t' || ptr2[i]=='\r' ) && i!=len) {
		i++;
	 }
	 if(i==len) {
		ptr2[0]='\0';
		return;
	 }
	 
	// int numoftrailingspace = i;
	 int k=0;
	 while(buf[i]!='\0') {
		ptr2[k] = buf[i];
		i++; k++;
	 }
	 ptr2[k]='\0';
	#ifdef DEBUG2
	cout<<"---"<<buf<<"---"<<endl;
	#endif

}
void myTrim(char* buf) {

	int templen = strlen(buf);
	if(templen>0) {
		if(buf[templen-1]=='\r') {
			buf[templen-1]='\0';	
			#ifdef DEBUG2
				cout<<"removed a carriage"<< buf<<" \n";
			#endif 
		}
	}


	int len = strlen(buf);
	#ifdef DEBUG2
	cout<<"len="<<len<<endl;
	#endif
	if(len==0) return;
	///< leading spaces
	char* ptr1 = buf;
	int i=len-1;
	#ifdef DEBUG2
	cout<<i<<endl;
	#endif
	
	///< for single char 
	if(i==0 && !(isspace(ptr1[i]) || ptr1[i]=='\t' || ptr1[i]=='\r' ) ) {
		#ifdef DEBUG2
		cout<<"single char\n";
		#endif
		myTrailTrim(buf);
		return;
	}
	while( (isspace(ptr1[i]) || ptr1[i]=='\t' || ptr1[i]=='\r' ) && i!=0) {
		ptr1[i]='\0';
		i--;
		#ifdef DEBUG2
		cout<<"trimmed space\n";
		#endif
	}
	
	/*
	if(i==0) {	
		ptr1[i]='\0';
		return;
	}*/
 
	#ifdef DEBUG2
	cout<<"---"<<buf<<"---"<<endl;
	#endif
	
	myTrailTrim(buf);
 
}
int isDir(char* str) {
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
 
 
void displaySpace(int num) {
	for(int i =0;i<num;i++) {
		fprintf(stdout," ");
	}
}

/*
 * Begin code I did not write.
 * This code is derived from http://www.koders.com/c/fid8679A6044F747F9240EE95A9C00821C820012EF3.aspx?s=search
 * Copyright (C) 2004-2006  Autodesk, Inc.
 */
char* mydtoa(double number, int width, int precision)
{
	static char buffer[10][32];
	static int current = 0;
	char temp[32];
	char *ptr;

	if (width > 0) {
		if (precision > 0)
		{
			sprintf(temp, "%%%d.%df", width, precision);
		}
		else {
			sprintf(temp, "%%%df", width);

		}
	} else {
		if (precision > 0)
		{
			sprintf(temp, "%%.%df", precision);
		}
		else {
			sprintf(temp, "%%f");
		}
	}
	ptr = buffer[current];
	sprintf(ptr, temp, number);
	current++;
	current %= 10;
	return ptr;
}
/*
 * End code I did not write.
 */



/**
param1: 123 
return “123”
*/
char* myItoa(int num) {
	
	char *str = new char [32];
	int isnegative = 0;
	if(num == 0 ) {
		strcpy(str,"0");
		return str;
	}
	if(num<0) {
		num = num*(-1);
		isnegative = 1;
	}

	
	while(num!=0) {
		int rdigit = num%10;		///< rdigit holds 3 of 123
		num = num / 10;			///< update number 123 becomes 12
		str = prepend(str,rdigit);	
	}
	
	if(isnegative) {
		char* minstr = new char[32]; 
		strcpy(minstr,"-");
		strcat(minstr,str);
		delete str;
		return minstr;
	} 
	return str;
}
// prepend(“abc”,4 ) returns “4abc”
char* prepend(char *str, int num) {
	
	char *temp = new char[32];
	char a[2];
	char allnum[] = "0123456789";
	a[1] = '\0';
	a[0] = allnum[num];
	strcpy(temp,a);
	//fprintf(stdout,"-%s --%s  %d",str,temp,num);
	strcat(temp,str);
	delete str;
	return temp;
}


/* myatoi_failsonfirstnondigit: */
int myatoi(char *s)
{
    int i=0, n=0;
    while(s[i]==' ' || s[i]=='\t' || s[i]=='\n')
    {
   	 ++i;
    }

    for (; s[i] >= '0' && s[i] <= '9'; ++i)
   	 n = 10 * n + (s[i] - '0');
    return n;
}












