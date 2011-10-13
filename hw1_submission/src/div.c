#include "../headers/div.h"
#include "../headers/mul.h"


/*!
 * Function divided two unsigned 32 bit integers and returns a unsigned 32 bit integer
 * Function also displays the result in formatted manner.
 * @param1: n1 : unsigned 32 bit integer - Dividend
 * @param2: n2 : unsigned 32 bit integer - Divisor
 * return: result of long Division type: unsigned 32 bit integer
            2022
        /-------
    409 | 827395
          818   
          ------
            93  
             0  
           -----
            939 
            818 
            ----
            1215
             818
             ---
             397
 */
	   //////<  dividend, divisor
void printSpaces(int num)
{
	int i = 0;
	for(i=0;i<num;i++)
		fprintf(stdout," ");
}

void printDashes(int num)
{
	int i = 0;
	for(i=0;i<num;i++)
		fprintf(stdout,"-");
}

void printTab()
{
	fprintf(stdout,"\t");
}

void printNewline()
{
	fprintf(stdout,"\n");
}
uint32_t LongDiv(uint32_t n1,uint32_t n2)
{

	uint32_t quotient = 0;
	uint32_t tempdividend = n1;
	uint32_t curdividend = 0;
	uint32_t powervalue = 0;
	uint32_t curquotient = 1;
	uint32_t curremainder = 0;
	uint32_t tens = 1;
	uint32_t msb = 0;
	uint32_t curquo[255]   = {0};
	int curquoCount = 0;
	uint32_t intermed[255] = {0};
	int intermedCount = 0;
	uint32_t finalremainder = 0;
	uint32_t dummy = 0;
	uint32_t predummy = 0;
	uint32_t arrcurdiv[255] = {0};
	uint32_t tempdividendlen = 0;
	
	//uint32_t singledigit= 0;
	int formatlen = 0;  			///< always == len(divisor) + 1 + 1 + 1 +len(dividend)
	int divisorlen = 0;
	int dividendlen = 0;
	int curdividendlen = 0;
	int curquotientlen = 0;
	int i =0;
	int counter = 0;
	int divCount = 0;
	


	dividendlen = getlen(n1);
	divisorlen  = getlen(n2);
	formatlen   = divisorlen + 3 + dividendlen;
	curdividendlen = dividendlen;


	#ifdef DEBUG0
	/// < if n1 or n2 is 4294967295 it will show -1 
	fprintf(stdout,"DEBUG: LongDiv:");   ///<if number is 4294967295 it becomes -1 if not typecasted
	displayNumber(1,n1);	
	displayString(1,"/");
	displayNumber(1,n2);
	fprintf(stdout, " : len(n1) = %d len(n2)= %d formatlen = %d\n",dividendlen,divisorlen,formatlen);
	#endif	
	
	if(n1==n2)
	{
		quotient = 1;
		#ifdef DEBUG0
		fprintf(stdout,"DEBUG: n1==n2== ");
		displayNumber(0,n1);
		fprintf(stdout," Quotient=1\n");
		#endif
		

		///n2  + 1 + | + 1 + n1 = formatlen
		printTab();						///< tab 1
		displayNumber(formatlen-getlen(quotient),quotient);     ///<   
		printNewline(); 					///<  \n
		printTab();	
		displayString(getlen(n2)+1,"/");			
		for(i=0;i<getlen(n1)+1;i++)
			fprintf(stdout,"-");
		printNewline();
		printTab();						///< 11 | 11
		displayNumber(0,n2);
		displayString(1,"|");
		displayNumber(1,n1);
		printNewline();
		
		//display 2nd num
		printTab();	
		displayNumber(formatlen-getlen(n1),n1);
		printNewline();

		//display reduced dash
		printTab();
		printSpaces(getlen(n2)+3);
		for(i=0;i<getlen(n1)+0;i++)
			fprintf(stdout,"-");
		printNewline();
		printTab();
		printSpaces(formatlen-1);
		displayNumber(0,0);
		printNewline();
		return quotient;

	}
	if(n2>n1)  ///< the dividend being zero case should be taken care here since any number will be greater than zero.
	{
		quotient = 0;
		#ifdef DEBUG0
		fprintf(stdout,"DEBUG: n2>n1 ");
		fprintf(stdout,"Quotient=0 \n");
		#endif

		/*
			   0
			/---
		    999 | 11
			   0
			  --
			  11
		*/
		///n2  + 1 + | + 1 + n1 = formatlen
		printTab();						///< tab 1
		displayNumber(formatlen-getlen(quotient),quotient);     ///<   
		printNewline(); 					///<  \n
		printTab();	
		displayString(getlen(n2)+1,"/");			
		for(i=0;i<getlen(n1)+2-1;i++)
			fprintf(stdout,"-");
		printNewline();
		printTab();						///< 11 | 11
		displayNumber(0,n2);
		displayString(1,"|");
		displayNumber(1,n1);
		printNewline();
		
		//display 2nd num
		printTab();	
		displayNumber(formatlen-getlen(quotient),quotient);
		printNewline();

		//display reduced dash
		printTab();
		printSpaces(getlen(n2)+2+1);
		for(i=0;i<getlen(n1)+2-2;i++)
			fprintf(stdout,"-");
		printNewline();
		printTab();
		printSpaces(formatlen-getlen(n1));
		displayNumber(0,n1);
		printNewline();
		return quotient;	
	}
/*
          2022
        /-------
    409 | 827395
          818   
          ------
            93  
             0  
           -----
            939 
*/
	///80000/10pow(5-1) will give 8
	///< get the first number greater than n2
	tens = 1;
	curdividend = 0;
	dummy = tempdividend;
	predummy = 0;
	tempdividendlen = getlen(tempdividend);
	for(i=tempdividendlen-1;i>=0;i--)
	{
		arrcurdiv[i] =  dummy % 10;
		dummy = dummy/10;
		//fprintf(stdout,"%d",arrcurdiv[i]);
	}

	/*for(i=0;i<tempdividendlen;i++)
	{
		fprintf(stdout,"%d",arrcurdiv[i]);
		printNewline();
	}*/


	powervalue = 0;
	counter = 1;
	i=0;
	while(n2>curdividend)
	{		
			
		//fprintf(stdout,"%d",arrcurdiv[i]);
		curdividend = (curdividend*10) + arrcurdiv[i++]; 
		curdividendlen--;
		#ifdef DEBUG0
		fprintf(stdout," curdiv=");
		displayNumber(0,curdividend);
		printNewline();
		#endif
		/*
		counter++;	
		fprintf(stdout,"dummy:%d",dummy);	
		powervalue = mypow(10,getlen(tempdividend)-1);
					
		curdividend = (curdividend * tens) + (tempdividend/powervalue);
		tens = 10;
		#ifdef DEBUG0
		
		fprintf(stdout,"DEBUG0: tempdividend=");
		displayNumber(0,tempdividend);
		#endif
						
		////1024
		tempdividend = tempdividend%powervalue;
		////024
		
		

		#ifdef DEBUG0			
		fprintf(stdout," curdiv=");
		displayNumber(0,curdividend);
		fprintf(stdout," currentdividendlen = ");
		displayNumber(0,curdividendlen);
		fprintf(stdout," tempdividend=");
		displayNumber(0,tempdividend);
		fprintf(stdout,"\n");
		#endif
		curdividendlen--;
		continue;
		*/

	}	

	divCount = i; /// < update the current count of having moved ahead in the originaldivided array : arrcurdiv
	#ifdef DEBUG0		
	fprintf(stdout,"====================\n\n");
	#endif
	curquotient = curdividend/n2;
	curquotientlen = getlen(quotient);					
	quotient = (quotient * 10) + curquotient;   ///< updates the final quotient     
	curremainder = curdividend % n2;
	if(curdividendlen==0)
	{
		//////////////////////////////////////////////////////
		printTab();						///< tab 1
		displayNumber(formatlen-getlen(quotient),quotient);     ///<   
		printNewline(); 					///<  \n
		printTab();	
		displayString(getlen(n2)+1,"/");			
		for(i=0;i<getlen(n1)+2-1;i++)
			fprintf(stdout,"-");
		printNewline();
		printTab();						///< 11 | 11
		displayNumber(0,n2);
		displayString(1,"|");
		displayNumber(1,n1);
		printNewline();
		////////////////////////////////////////////////////
		
		//display 2nd num and dash start from same place
		printTab();	
		//printSpaces(getlen(n2)+3+0);
		printSpaces(formatlen - getlen(curquotient*n2));
		displayNumber(0,curquotient*n2);   			/////////////INTERMEDIATE RESULT:
		printNewline();

		//display reduced dash and 2nd num start from same place
		printTab();
		printSpaces(getlen(n2)+3+0);
		for(i=0;i<getlen(n1)+2-2;i++)
			fprintf(stdout,"-");
		printNewline();
		printTab();
		printSpaces(formatlen-getlen(curremainder));  		///////////////////LAST CASE IS REMAINDER 

		//displayNumber32(0,curdividend);
		//printSpaces(4);
		//displayNumber32(0,(n2*curquotient));
		//printSpaces(4);
		//displayNumber32(0,curdividend-(n2*curquotient));	///this should be curdividend for future cases
		displayNumber(0,curremainder);				///this should be curdividend for future cases
		//printSpaces(4);		
		//displayNumber32(0,833724077 - 597396580);	
		//fprintf(stdout,"%u",curremainder);
		printNewline();

		#ifdef DEBUG0
		fprintf(stdout,"DEBUG:");
		displayNumber(0,curdividend);
		fprintf(stdout,"/");
		displayNumber(0,n2);
		fprintf(stdout," = ");
		displayNumber(0,curquotient);
		fprintf(stdout,"\tremainder=");
		displayNumber(0,curremainder);
		fprintf(stdout,"\ttempdividend=");
		displayNumber(0,tempdividend);
		fprintf(stdout,"\tfinalquotient=");
		displayNumber(0,quotient);
		fprintf(stdout,"\n");
		#endif	
		return quotient;
	}

	////////////////////////////////////////////////
	//printNewline();
	//displayNumber(0,curquotient*n2);
	curquo[curquoCount++] = curquotient*n2; 
	///////////////////////////////////////////////
	counter = 0;
	msb = 0;
	//while(curdividendlen!=0)
	while(divCount!=tempdividendlen)
	{
		counter++;
		/*curdividend = curremainder * 10;     				 ///< update currentdividend
		msb = tempdividend/(mypow(10,getlen(tempdividend)-1));   
		curdividend = curdividend + msb;
		tempdividend = tempdividend %(mypow(10,getlen(tempdividend)-1));   ///< remove the msb of dividend
		curremainder = curdividend;
		*/

		curdividend = (curremainder*10) + arrcurdiv[divCount++]; 
		curdividendlen--;
		curremainder = curdividend;

		
		////////////////////////////////////////
		//printNewline();
		//displayNumber(0,curremainder);	
		intermed[intermedCount++] = curremainder;
		////////////////////////////////////////
		curdividendlen--;
		if(n2>curdividend)
		{
			curquotient = 0;
			curquotientlen = getlen(quotient);
			quotient = (quotient * 10) + curquotient;   ///< updates the final quotient  

			////////////////////////////////////////////////
			//printNewline();
			//displayNumber(0,curquotient*n2);
			curquo[curquoCount++] = curquotient*n2; 	
			///////////////////////////////////////////////	
			continue;
		}
		curquotient = curdividend/n2;
		curquotientlen = getlen(quotient);					
		quotient = (quotient *10) + curquotient;   ///< updates the final quotient     
		curremainder = curdividend%n2;

		/////////////////////////////////////////
		//printNewline();
		//displayNumber(0,curquotient*n2);
		curquo[curquoCount++] = curquotient*n2; 
		////////////////////////////////////////
	
	} ///< end of while

	///////////////////////////////////
	//printNewline();
	//displayNumber(0,curremainder);
	finalremainder = curremainder;
	////////////////////////////////////


	//////////////////////////////////////////////////////
	printTab();						///< tab 1
	displayNumber(formatlen-getlen(quotient),quotient);     ///<   
	printNewline(); 					///<  \n
	printTab();	
	displayString(getlen(n2)+1,"/");			
	for(i=0;i<getlen(n1)+2-1;i++)
		fprintf(stdout,"-");
	printNewline();
	printTab();						///< 11 | 11
	displayNumber(0,n2);
	displayString(1,"|");
	displayNumber(1,n1);
	printNewline();

	
	//fprintf(stdout,"  curquolen = %d ",curquoCount);
	//fprintf(stdout,"  intermedlen = %d\n",intermedCount);
	////////////////////////////////////////////////////
	counter = 0; ///< to find the initial space on left to align from right
	counter = formatlen - (getlen(curquo[0]) + getlen(n2)+3);
	
	for(i=0;i<intermedCount;i++)
	{
		
		printTab();
		predummy = formatlen-(counter+getlen(curquo[i])-i);
		//fprintf(stdout,"formatlen = %d, predummy = %d, len(curquo)=%d\n",formatlen,predummy,getlen(curquo[i]));
		printSpaces(formatlen-(counter+getlen(curquo[i])-i));
		displayNumber(0,curquo[i]);
		//fprintf(stdout,"%d",counter);
		printSpaces(formatlen - (predummy + getlen(curquo[i])) );
		//printSpaces(formatlen - (counter + getlen(curquo[i]));
		printNewline();
		printTab();
		printSpaces(getlen(n2)+3+i);
		//fprintf(stdout,"\t--------");
		printDashes(getlen(n1)-i);

		printNewline();
		printTab();
		predummy = formatlen-(counter+getlen(intermed[i])-i-1);
		printSpaces(formatlen-(counter+getlen(intermed[i])-i-1) );
		displayNumber(0,intermed[i]);
		printSpaces(formatlen - (predummy + getlen(intermed[i])) );
		//printSpaces(formatlen - (formatlen-(counter+getlen(intermed[i])-i-1) + intermed[i]) + 1);
		printNewline();

	}
		printTab();
		predummy = formatlen-(counter+getlen(curquo[i])-i);
		printSpaces(formatlen-(counter+getlen(curquo[i])-i));
		displayNumber(0,curquo[i]);
		printSpaces(formatlen - (predummy + getlen(curquo[i])) );
		printNewline();
		printTab();
		printSpaces(getlen(n2)+3+i);
		printDashes(getlen(n1)-i);
		printNewline();
		printTab();
		printSpaces(formatlen-getlen(finalremainder));
		displayNumber(0,finalremainder);
		printNewline();

	return quotient;
}


uint32_t mypow(int n1, int power)
{
	uint32_t result = 1;
	int i =0;
	for(i=0;i<power;i++)
		result=result*n1;
		
	return result;
}

/*int numlen = 0;
uint32_t getnext(uint32_t num)
{
	powervalue = mypow(10,curdividendlen);
	curdividend =tempdividend/powervalue;
	curdividendlen--;

}*/
/*
           0
        /---
    999 | 11
           0
          --
          11


            2022
        /-------
    409 | 827395
          818   
          ------
            93  
             0  
           -----
            939 
            818 
            ----
            1215
             818
             ---
             397
*/
