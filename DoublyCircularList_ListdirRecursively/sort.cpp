#include "sort.hpp"
#include "cs570.h"

#include "my570list.hpp"
#include <stdlib.h>
///////////////////////////////// 




struct record * isRecordOk(char* buf,int count) {

	char* tempbuf = new char[MAXBUFSIZE];
	strcpy(tempbuf,buf);

	if(strlen(buf)>1024) {
		#ifdef DEBUG0
		fprintf(stdout,"strlen(buf)=%d",strlen(buf));
		#endif
		fprintf(stderr,"Record# %d is Malformed: input file is not in the right format\n",count);
		exit(-1);
	}
	
	/***Tokenize strings to check their validity using strtok****/
	char *fields;
	fields = strtok(buf,"\t");
	int numoftokens=0;
	while(fields!=NULL) {
		#ifdef DEBUG1
		fprintf (stdout,"%s\n",fields);
		#endif
		fields = strtok (NULL, "\t");
		numoftokens++;
	}	
	///< Check if there are no more or no less fields than 4
	if(numoftokens!=4) {
		#ifdef DEBUG0
		fprintf (stdout,"too less tokens in record#%d",count);
		#endif
		fprintf(stderr,"Record# %d is Malformed: input file is not in the right format\n",count);
		exit(-1);
	}	
	
	///<check each record for its validity ====================================
	char *tokens = strtok(tempbuf,"\t");
	char leftofdecimal[20];
	char rightofdecimal[20];
	int len;
	int k = 0;
	int i = 0;
	numoftokens=0;
	struct record* temp_str = new struct record;
	int dotflag = 0;
	while(tokens!=NULL) {
		#ifdef DEBUG1
		fprintf (stdout,"%s\n",tokens);
		#endif
		switch(numoftokens) {
			case 0:	///< sign should be exactly of len 1
					if(strlen(tokens)!=1) {
						#ifdef DEBUG0
							fprintf(stdout,"SIGN IS NOT of size 1 in record %d\n",count);
						#endif
						fprintf(stderr,"Record# %d is Malformed: input file is not in the right format\n",count);
						exit(-1);
					}
					if( !( !strcmp(tokens,"+") || !strcmp(tokens,"-") ) ) {
						#ifdef DEBUG0
							fprintf(stdout,"SIGN IS NOT + or - but something else in record %d\n",count);
						#endif
						fprintf(stderr,"Record# %d is Malformed: input file is not in the right format\n",count);
						exit(-1);
					}
					strcpy(temp_str->sign,tokens);									///< UPDATE STRUCT SIGN
					break;
			case 1: ///< TIMESTAMP should be of size 1 to 10 
					if( strlen(tokens)>=11 || strlen(tokens)<=0)  {
						#ifdef DEBUG0
							fprintf(stdout,"TIMESTAMP IS TOO BIG OR LESS in record %d\n",count);
						#endif
						fprintf(stderr,"Record# %d is Malformed: input file is not in the right format\n",count);
						exit(-1);
					}
					if(!isInputanInteger(tokens)) {
						#ifdef DEBUG0
							fprintf(stdout,"TIMESTAMP contains alphabets and special chars %d\n",count);
						#endif
						fprintf(stderr,"Record# %d is Malformed: input file is not in the right format\n",count);
						exit(-1);
					}
					temp_str->timestamp = atoi(tokens);								///< UPDATE STRUCT Timestamp
					break;
			case 2:///< Amount on the left side of . should be of size 1 to 7
					if(strlen(tokens)>=13 or strlen(tokens)<=0 ) {
						#ifdef DEBUG0
							fprintf(stdout,"Amount is too big\n");
						#endif					
						fprintf(stderr,"Record# %d is Malformed: input file is not in the right format\n",count);
						exit(-1);
					}
					len = strlen(tokens);
					dotflag = 0; 	///< bug fix
					for(i =0;i<len;i++) {
						char a[2];
						leftofdecimal[i] = tokens[i];
						a[0] = tokens[i];
						a[1] = '\0';
						if(!strcmp(a,".") ){
							leftofdecimal[i] = '\0';
							i++;
							dotflag = 1;	///< bug fix for 
							break;
						}
					}
					///< bug fix
					if(dotflag ==0) {
						////< dot was not found!!!
						#ifdef DEBUG0
						fprintf(stdout,"dot was not found in the input Amount\n");
						#endif					
						fprintf(stderr,"Record# %d is Malformed: input file is not in the right format\n",count);
						exit(-1);
					}
					
					k=0;
					for(int j = i; j<len; j++) {
						rightofdecimal[k++]=tokens[j];
					}
					rightofdecimal[k]='\0';

					if(strlen(leftofdecimal)>=10 ||strlen(leftofdecimal)<=0 || /*strlen(rightofdecimal)<=0||*/strlen(rightofdecimal)>=3) {
						#ifdef DEBUG0
							fprintf(stdout,"too many numbers right of decimal or left of decimal\n");
						#endif					
						fprintf(stderr,"Record# %d is Malformed: input file is not in the right format\n",count);
						exit(-1);	
					}
					
					
					if(!isInputanInteger(leftofdecimal)) {
						#ifdef DEBUG0
							fprintf(stdout,"Amount (left of decimal) contains alphabets and special chars %d\n",count);
						#endif
						fprintf(stderr,"Record# %d is Malformed: input file is not in the right format\n",count);
						exit(-1);
					}
					
					if(!isInputanInteger(rightofdecimal)) {
						#ifdef DEBUG0
							fprintf(stdout,"Amount (right of decimal)  contains alphabets and special chars %d\n",count);
						#endif
						fprintf(stderr,"Record# %d is Malformed: input file is not in the right format\n",count);
						exit(-1);
					}
					strcpy(temp_str->leftamountString,leftofdecimal);
					strcpy(temp_str->rightamountString,rightofdecimal);
					temp_str->amount = atof(tokens);								///< UPDATE STRUCT AMOUNT
					break;
			case 3: ///< DESCRIPTION SIZE SHOULD BE 24 MAX so truncate everything else
					if(strlen(tokens)<=0) {
						#ifdef DEBUG0
							fprintf(stdout,"description is too big or less in record# %d\n",count);
						#endif
						fprintf(stderr,"Record# %d is Malformed: input file is not in the right format\n",count);
						exit(-1);					
					}
					
					if(strlen(tokens)<25) {
						strcpy(temp_str->description,tokens);						///< UPDATE STRUCT DESCRIPTION
					} else {
						for(i =0;i<24;i++) {
							temp_str->description[i]=tokens[i];
						}
						temp_str->description[i]='\0';
					}
					break;
			default: break;
		}		
		tokens = strtok (NULL, "\t");
		numoftokens++;
	}		
	
	return temp_str;
}

/*
 * Begin code I did not write.
 * This code is partly derived from http://merlot.usc.edu/cs570-f11/homeworks/prog-faq/#cpp_process
 * If the source code requires you to include copyright, put copyright here.
 */
void Process(istream& in) {
	int count = 0;
	char* buf = new char[MAXBUFSIZE];
	in.getline(buf,MAXBUFSIZE);
	if(strlen(buf)==0) {
		fprintf(stderr,"Input/File is empty\n");
		exit(-1);
	}
	
	My570List *list=new My570List();				///< CREATING LIST to push all the struct elems 
	while (!in.eof()) {
		count++;									///< Count all records
		struct record *temp_str;					///< Create record pointer
		temp_str = isRecordOk(buf,count);			///< Validate BUFFER and get back record
		//(void)list->Append((void*)temp_str);		///< Push records into the LIST unsorted
		SortedInsert(list,temp_str);				///< push records into the list in a sorted manner
		#ifdef DEBUG1
		fprintf(stdout,"%s %d %0.2f %s\n",temp_str->sign,temp_str->timestamp,temp_str->amount,temp_str->description); ///< display record
		#endif		
		
		#ifdef DEBUG1
			fprintf(stdout,"%s\n",buf);				///< PRINT THE RAW BUFFER
		#endif 
		in.getline(buf,MAXBUFSIZE);					///< GET THE NEXT LINE
	}
	
	#ifdef DEBUG0
	fprintf(stdout,"*******************SORTED DISPLAY*******************\n");
	displayRecords(list);							///< DISPLAY THE RECORDS from list
	#endif
	
	
	tableDisplay(list);								///< DISPLAY FORMATTED OUTPUT!!!! THIS WILL GIVE ME MARKS !!!! :--/ @#@!#!
	
	#ifdef DEBUG1
		fprintf(stdout,"EOF REACHED\n");
	#endif
	
	#ifdef DEBUG0
		fprintf(stdout,"total number of records processed = %d\n",count);
	#endif
	
}
/*
 * End code I did not write.
 */	

void tableDisplay(My570List *list) {

/*
    +-----------------+--------------------------+----------------+----------------+
    |       Date      | Description              |         Amount |        Balance |
    +-----------------+--------------------------+----------------+----------------+
    | Thu Aug 21 2008 | ...                      |      1,723.00  |      1,723.00  |
    | Wed Dec 31 2008 | ...                      | (       45.33) |      1,677.67  |
    | Mon Jul 13 2009 | ...                      |     10,388.07  |     12,065.74  |
    | Sun Jan 10 2010 | ...                      | (      654.32) |     11,411.42  |
    +-----------------+--------------------------+----------------+----------------+
Each line is exactly 80 characters long (followed by a single "\n" character).
The Date field spans characters 3 through 17. Please use ctime() to format the timestamp 
and remove unnecessary characters to make it look like what's in the table above. 
The Description field spans characters 21 through 44. (If a description is too long,
you must truncate it.) The Amount field spans characters 48 through 61. For a withdrawal,
a pair of paranthesis must be used as indicated. If the amount of a transaction
is more than or equal to 10 million, please print ?,???,???.?? (or (?,???,???.??))
in the Amount field. The Balance field spans characters 65 through 78.
If a balance is negative, a pair of paranthesis must be used.
If the absolute value of a balance is more than or equal to 10 million, please print ?,???,???.?? (or (?,???,???.??)) in the Balance field.
 */
	double balance = 0;
 	fprintf(stdout,"+-----------------+--------------------------+----------------+----------------+\n");
	fprintf(stdout,"|       Date      | Description              |         Amount |        Balance |\n");
	fprintf(stdout,"+-----------------+--------------------------+----------------+----------------+\n");
	My570ListElem *elem=NULL;
	int recnum = 0;
	for (elem=list->First(); elem != NULL; elem=list->Next(elem)) {
        struct record *temp =(struct record*)(elem->Obj());
		temp->recordnum =recnum;
		recnum++;

		fprintf(stdout,"| ");
		displayDate(temp->timestamp); 		///<will add a space at the end ---TIMESTAMP
		
		fprintf(stdout,"| ");				///< add a | on same line		---DESCRIPTION
		displayDescription(temp->description);
		
		fprintf(stdout,"| ");				///< add a | on same line		---AMOUNT
		displayAmount(temp);
		
		fprintf(stdout,"| ");				///< add a | on same line		---AMOUNT
		balance = displayBalance(balance,temp);
		
		fprintf(stdout,"|");
		fprintf(stdout,"\n");
    }
	fprintf(stdout,"+-----------------+--------------------------+----------------+----------------+\n");	
}
/*
 * Begin code I did not write.
 * This code is derived from http://www.koders.com/c/fid8679A6044F747F9240EE95A9C00821C820012EF3.aspx?s=search
 * Copyright (C) 2004-2006  Autodesk, Inc.
 */
char *nls_ftoa(double number, int width, int precision)
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
 


void displaybal(double balance) {

	int negative = 0;
	char left[256];
	char right[256];
	char *token;
	
	if(balance <0 ) {
		negative = 1;
		balance = balance * (-1);
	} else if(balance >=0 ) {
		negative = 0;
	}	
	
	char* buf = nls_ftoa(balance,7,2);
	token = strtok(buf," .");
	int counttok = 0;
	while(token!=NULL){
		switch(counttok) {
			case 0: 
					strcpy(left,token);
					//cout<<"len="<<strlen(left);
					break;
			case 1: 
					strcpy(right,token);
					break;
		}
		token = strtok(NULL," .");
		counttok++;
	}
	int leftlen = strlen(left);
	int rightlen = strlen(right);
	int commacount	= addCommas(left);

	if(negative==1) {	
	
		if ( balance >= 10000000) {
			cout<<"(?,???,???.??\b ) ";
		} else {
			fprintf(stdout,"(");
			displaySpace(16-leftlen-rightlen-1-1-1-1-1-commacount);
			fprintf(stdout,"%s",left);
			fprintf(stdout,".");
			fprintf(stdout,"%s",right);
			fprintf(stdout,") ");
		}
	}
	
	if(negative==0) {
		if ( balance >= 10000000) {
			fprintf(stdout," ?,???,???.??  ");
		} else {
			displaySpace(16-leftlen-rightlen-1-1-1-1-commacount);
			fprintf(stdout,"%s",left);
			fprintf(stdout,".");
			fprintf(stdout,"%s",right);
			displaySpace(2);
		}
	}
}


double displayBalance(double balance,struct record* temp) {
	char* sign = temp->sign;
	double amount = temp->amount;

	if(temp->recordnum == 0) {		
		if(!strcmp(sign,"-")) {
			balance = -1 * amount;
		}
		if(!strcmp(sign,"+")) {
			balance = 1 * amount;
		}
		displaybal(balance);
		return balance;
	}
	
	if(!strcmp(sign,"+")) {
		#ifdef DEBUG1
			fprintf(stdout,"%f+%f = %f",balance,amount,(balance+amount));
		#endif
		displaybal(balance+amount);
		return (balance+amount);
	}
	if(!strcmp(sign,"-")) {
		#ifdef DEBUG1 
			fprintf(stdout,"%f-%f =%f",balance,amount, (balance-amount));
		#endif	
		displaybal(balance-amount);
		return (balance-amount);
	}
	fprintf(stderr, "sign is neither + nor - so exiting!!\n");
	exit(-1);
}


void displayLeft(char* left) {
	fprintf(stdout,"%s",left);
}


//takes a string and converts it into numeric format for eg: 2323232 is converted to 2,323,232 and 
//returns the num of commas added
int addCommas(char* amountstr) {
	char newstr[256];
	int oldlen = strlen(amountstr);
	
	if(oldlen<4) 
		return 0;
	
	if(oldlen>=4 && oldlen<7 ) {
		int j=0;
		int k=oldlen-1+1; //coz max one comma always since max size of left is 7 digits meaning max one comma
		newstr[oldlen+1] = '\0';
		for(int i=oldlen-1;i>=0;i--) {
			newstr[k--]=amountstr[i];
			j++;
			if(j==3)
				newstr[k--]=',';
		}
		strcpy(amountstr,newstr);	
		return 1;
	}
	if(oldlen>=7) {
		int j=0;
		int k=oldlen-1+1+1; //coz max one comma always since max size of left is 7 digits meaning max one comma
		newstr[oldlen+1+1] = '\0';
		for(int i=oldlen-1;i>=0;i--) {
			newstr[k--]=amountstr[i];
			j++;
			if(j==3)
				newstr[k--]=',';
			if(j==6)
				newstr[k--]=',';
		}
		strcpy(amountstr,newstr);
		return 2;
	}
	return 0;
}
void displayAmount(struct record* temp) {

	char* sign = temp->sign;
	int rightlen = strlen(temp->rightamountString);
	int leftlen = strlen(temp->leftamountString);
	
	int commacount = 0;
	commacount = addCommas(temp->leftamountString);
	if(!strcmp(sign,"+")) {
	
	
		if ( temp->amount >= 10000000) {
			cout<<" ?,???,???.??  ";
		} else {	
			displaySpace(16-leftlen-rightlen-1-1-1-1-commacount);
			if(strlen(temp->rightamountString)>0) {
				displayLeft(temp->leftamountString);
				fprintf(stdout,".");
				fprintf(stdout,"%s",temp->rightamountString);
			}	
			if(strlen(temp->rightamountString)==0) {
				displayLeft(temp->leftamountString);
			}		
			displaySpace(2);
		}
	}

	if(!strcmp(sign,"-")) {
	
		if ( temp->amount >= 10000000) {
			cout<<"(?,???,???.??\b ) ";
		} else {	
			fprintf(stdout,"(");
			displaySpace(16-leftlen-rightlen-1-1-1-1-1-commacount);
			if(strlen(temp->rightamountString)>0) {
				displayLeft(temp->leftamountString);
				fprintf(stdout,".");
				fprintf(stdout,"%s",temp->rightamountString);
			}
			if(strlen(temp->rightamountString)==0) {
				displayLeft(temp->leftamountString);
			}		
			fprintf(stdout,")");
			displaySpace(1);
		}
	}
}



void displayDate(unsigned int timestamp) {
	char stringDate[256] = {0};
	time_t timeval = timestamp;
	strcpy(stringDate, ctime (&timeval));
	char* fields = strtok(stringDate," ");
	int numoftokens=0;
	while(fields!=NULL) {
		if(numoftokens!=3) { ///print all except 4th field...
			if(numoftokens ==4) {	///print date seperately.. it has newline and whatnot.
				fprintf(stdout,"%c",fields[0]);
				fprintf(stdout,"%c",fields[1]);
				fprintf(stdout,"%c",fields[2]);
				fprintf(stdout,"%c ",fields[3]);
			} 
			else if(numoftokens ==2) //if num is of size 1 then add 2 space else 1 space
			{
				if(strlen(fields)==2) {
					fprintf(stdout,"%s ",fields);
				} else if(strlen(fields)==1) {
					fprintf(stdout," %s ",fields);
				}
			} else {
				fprintf(stdout,"%s ",fields);
			}
		}
		fields = strtok (NULL," ");
		numoftokens++;
	}
}

void displayDescription(char * description) {
	fprintf(stdout,"%s",description);
	displaySpace(24-strlen(description)+1); 
}

void displaySpace(int num) {
	for(int i =0;i<num;i++) {
		fprintf(stdout," ");
	}
}
void SortedInsert(My570List *list,struct record* new_str) {
	if(list->Empty()) {
		#ifdef DEBUG1
			fprintf(stdout,"list initially empty\n");
		#endif
		(void)list->Append((void*)new_str);
		return;
	}

	My570ListElem *cur_elem = list->First();
	struct record *cur_str;
	while(cur_elem!=NULL) {
		cur_str =(struct record*)(cur_elem->Obj());
		if(cur_str->timestamp == new_str->timestamp) {
			#ifdef DEBUG0
				//fprintf(stdout,"Two records with same timestamp is not acceptable\n");
			#endif					
			fprintf(stderr,"(Malformed input):Input contains Two records with same timestamp\n");
			exit(-1);	
		}
	
		if( new_str->timestamp < cur_str->timestamp) { ///< 1 2 3 4 5  if new is 6
			 (void)list->InsertBefore(new_str, cur_elem);
			 return;
		}
		cur_elem = list->Next(cur_elem);
	}
	
	if(cur_elem == NULL) { ///< meaning reached the end of the list
		(void)list->InsertAfter(new_str,cur_elem);
	}
}

void displayRecords(My570List * list) {		
	My570ListElem *elem=NULL;
	for (elem=list->First(); elem != NULL; elem=list->Next(elem)) {
        struct record *temp =(struct record*)(elem->Obj());
		fprintf(stdout,"%s %d %0.2f %s\n",temp->sign,temp->timestamp,temp->amount,temp->description); 	
    }
}

void readfromStdin() {
	 Process(cin);
}

/*
 * Begin code I did not write.
 * This code is partly derived from http://stackoverflow.com/questions/4989431/how-to-use-s-isreg-and-s-isdir-posix-macros
 * If the source code requires you to include copyright, put copyright here.
 */
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



void Sort(int argc, char* argv[]) {
	if(argc==2) {
		readfromStdin();
		return;
	}
	
	if(argc==3) {
		readfromTfile(argv[2]);		///<argv[2] should be a filename;
		return;
	}
	SortUsage();
}


void SortUsage() {
	fprintf(stderr, "(malformed command)\n");
	fprintf(stderr,"Usage:\n");
	fprintf(stderr,"\thw2 sort [tfile] \n");
	fprintf(stderr,"\tif no tfile specified read from standard input\n");
	exit(-1);

}

void readfromTfile(char *filename) {

	ifstream in;
	ProcessFilenameFailure(filename);
	in.open(filename);
	if (in.fail()) {
		fprintf(stderr, "(Cannot open %s) \n",filename);
		exit(-1);
	}
	
	if(isDir(filename)) {
		fprintf(stderr,"(input file: %s is a directory)\n",filename);
		exit(-1);
	}
	Process(in);
	in.close();
}

/**
	 * Do checks for:
					 ./hw2 sort -x
						(malformed command)
					./hw2 sort /usr/bin/xyzz
						(input file /usr/bin/xyzz does not exist)
					./hw2 sort /etc/sysidcfg
						(input file /etc/sysidcfg cannot be opened - access denies)
					./hw2 sort /etc/inet/secret/xyzz
						(input file /etc/inet/secret/xyzz cannot be opened - access denies)
					./hw2 sort /etc
						(input file /etc is a directory)
					./hw2 sort /etc/motd
						(input file is not in the right format)
					./hw2 sort ~/.login
						(input file is not in the right format)
	 */	
void ProcessFilenameFailure(char * filename) {

	if(strlen(filename)!=0) {
	
		if(filename[0]=='-') {
			fprintf(stderr,"(malformed command)\n");
			exit(-1);
		}
	}
	
	if( access( filename, F_OK ) != -1 ) {
		#ifdef DEBUG1
		fprintf(stdout,"file exist\n");
		#endif
	} else {
		fprintf(stderr,"input file/path %s does not exist\n",filename);
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
	
	if(isDir(filename)) {
		fprintf(stderr,"(input file: %s is a directory)\n",filename);
		exit(-1);
	}
}




/*!
 * This Function will check if the passed string is an integer or not.
 * @return 0 if there are alphabets or special chars in input.
 * @param1: String 
 * returns 0 or 1 depending on whether the atol(param1) is an integer or not
 * 00000 is considered 0
 */
int isInputanInteger(char num[])
{
	int len = strlen(num);
	int i=0;	
	char a[2];

	/*Special case of if argument is $$ then argument becomes 20921 WEIRD!!  Am not taking care of it anyways :--/ */
	if(atol(num)==0) ///<atol returns 0 if its special char or alphabet
	{
		/*The number can now be a special character or 0 or 00000 */
		for(i=0;i<len;i++)
		{
			if(num[i]!='0') {
			#ifdef DEBUG0
				fprintf(stdout,"isInputanInteger():DEBUG:atol(%s):%ld \n",num,atol(num));
			#endif
				return 0;
			}
		}	
	}

	for(i=0;i<len;i++)
	{
		a[0]=num[i];
		a[1]='\0';
		if(num[i]!='0' && atol(a)==0) {
			#ifdef DEBUG0
				fprintf(stdout,"isInputanInteger():DEBUG:atol(%s):%ld \n",num,atol(num));
			#endif
			return 0;
		}
	}
	return 1;
}
