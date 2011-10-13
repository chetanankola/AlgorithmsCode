/*!
 * USCID:1895488595
 * Algorithms CSCI-570
 * # homework1
 * 
 * 
 * Description: This program is about Manipultaing Bits & Formatting Output
 * Usage:
 * The commandline syntax for running the program is as follows:
 * hw1 mul n1 n2
 * hw1 div n1 n2
 * 
 * The meaning of the commands are:
 *
 * mul:perform long-hand integer multiplication - multiply n1 by n2, where n1 and n2 are unsigned 32-bit    
 *     integers (i.e., with a maximum value of 4294967295). n1 will be referred to as the multiplicand and n2 
 *     will be referred to as the multiplier.
 *
 * div:perform long-hand integer division - divide n1 by n2 to produce a quotient and a remainder. n1 and n2 
 *     are unsigned 32-bit integers (i.e., with a maximum value of 4294967295). 
 *
 */
#include "../headers/main.h"
#include "../headers/mul.h"
#include "../headers/div.h"
#ifdef DEBUG0
#include "../test/test.h" 
int FLAG_VERBOSE=0;
#endif

int main(int argc, char *argv[])
{
	int status = 0;
	
	#ifdef DEBUG0
	int templen = 0;
	fprintf(stdout,"=========Manipulating Bits & Formatting Output=========\n"); 
	if(1)
	{

		if(!strcmp(argv[argc-1],"-DISS"))
		{	
			fprintf(stdout,"----****---display a String with spaces---***---\n");
			displayString(atol(argv[1]),argv[2]);
			fprintf(stdout,"\n");			
			return 1;
		}
		if(!strcmp(argv[argc-1],"-DISN"))
		{	
			fprintf(stdout,"----****---display a number with spaces---***---\n");
			displayNumber(atol(argv[1]),atoll(argv[2]));
			fprintf(stdout,"\n");			
			return 1;
		}
		if(!strcmp(argv[argc-1],"-LEN"))
		{	
			fprintf(stdout,"----****---Display number of digits---***---\n");
			templen = getlen(atoll(argv[1]));
			fprintf(stdout,"numofdigits in %s = %d\n",argv[1],templen);
			return 1;
		}
	
		if(!strcmp(argv[argc-1],"-D"))
		{
			FLAG_VERBOSE = 1;	
			fprintf(stdout,"----****---DEBUG MODE ENABLED---***---\n");
		}
		if(!strcmp(argv[argc-1],"-T0"))
		{
			runTestSuite(); ///< Run all the Test cases
			return 1;
		}
		if(!strcmp(argv[argc-1],"-T1"))
		{
			runMulSuite(); ///< Run all the Test cases
			return 1;
		}
		if(!strcmp(argv[argc-1],"-T2"))
		{
			runDivSuite(); ///< Run all the Test cases
			return 1;
		}
	}
	displayArguments(argc, argv);	
	#endif
	status = processArguments(argc, argv);
	if(!status) 
	{
		return 0; 		///< Program returns in case of faulty arguments.
	}
	#ifdef DEBUG0
	fprintf(stdout,"DEBUG:main(): ARGUMENTS ARE FINE!!!!\n");
	#endif

	
	if(!strcmp(argv[1],"mul"))
	{
		LongMul(atol(argv[2]),atol(argv[3]));
	}
	else if(!strcmp(argv[1],"div"))
	{
		LongDiv(atol(argv[2]),atol(argv[3]));
	}
	else
	{
		fprintf(stdout,"ARGUMENTS WERE NOT PARSED CORRECTLY - Check ProcessArguments()\n");
		return 0;
	}
	return 1; 	
}


/*!
 * function to display Usage of the program from command line
 */
int displayUsage()
{
	//fprintf(stderr,"hw1: Invalid usage\n");
	fprintf(stderr,"Usage:\n");
	fprintf(stderr,"\thw1 mul n1 n2 \n");
	fprintf(stderr,"\thw1 div n1 n2 \n");
	fprintf(stderr,"\twhere n1 and n2 are unsigned 32 bit integers\n");
	return 0;
}	


/*! This function will process cmd line args and check for invalid cmds
 * @param1 argc, number of command line args
 * @param2 argv, char array contains arguments
 * return type:integer indicating status of the check : 0 if failure 1 if success 
 *  
 */
/*!
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
		#ifdef DEBUG0
		fprintf(stdout,"isInputanInteger():DEBUG:atol(%s):%ld \n",num,atol(num));
		#endif
		/*The number can now be a special character or 0 or 00000 */
		for(i=0;i<len;i++)
		{
			if(num[i]!='0')
				return 0;
		}	
	}

	for(i=0;i<len;i++)
	{
		a[0]=num[i];
		a[1]='\0';
		if(num[i]!='0' && atol(a)==0)
			return 0;
	}

	return 1;
}

/*!
 * Function to process command line arguments 
 * return 1 if arguments are fine
 * return 0 if faulty arguments and also specify the usage etc.
 *
 */
int processArguments(int argc, char *argv[]) 
{
	if(argc>4)
	{	
		fprintf(stderr,"too many arguments\n");
		displayUsage();
		return 0; ///< too many arguments
	}
	if(argc<4)
	{	
		fprintf(stderr,"malformed command\n");
		displayUsage();
		return 0; ///< malformed command
	}
	if(!( !strcmp(argv[1],"mul") || !strcmp(argv[1],"div") ) )
	{
		fprintf(stderr,"malformed command\n");
		displayUsage();
		return 0; ///< malformed command
	}
	if(atoll(argv[2])>UINT32_MAX  || atoll(argv[2])<0)
	{
		
		#ifdef DEBUG0
		fprintf(stdout,"isInputanInteger():DEBUG:atol(%s):%lld \n",argv[2],atoll(argv[2]));
		fprintf(stdout,"TESTNUM=%lld  UINT32_MAX=%u\n",atoll(argv[2]),UINT32_MAX);
		#endif
		fprintf(stderr,"n1 out of range\n");
		return 0; ///< n1 out of range
	}

	if( atoll(argv[3])>UINT32_MAX || atoll(argv[3])<0)
	{
		#ifdef DEBUG0
		fprintf(stdout,"isInputanInteger():DEBUG:atol(%s):%lld \n",argv[3],atoll(argv[3]));
		fprintf(stdout,"TESTNUM=%lld  UINT32_MAX=%u\n",atoll(argv[2]),UINT32_MAX);
		#endif
		fprintf(stderr,"n2 out of range\n");
		return 0; ///< n2 out of range
	}
	if(!isInputanInteger(argv[2]) && !isInputanInteger(argv[3]))
	{
		fprintf(stderr,"invalid n1 and invalid n2\n");
		return 0; ///< invalid n1
	}
	if(!isInputanInteger(argv[2]))
	{
		fprintf(stderr,"invalid n1\n");
		return 0; ///< invalid n1
	}
	if(!isInputanInteger(argv[3]))
	{
		fprintf(stderr,"invalid n2\n");
		return 0; ///< invalid n2
	}
	if(atol(argv[3])==0 && !strcmp(argv[1],"div") )
	{
		fprintf(stderr,"invalid n2\n");
		return 0; ///< division by zero, invalid n2
	}
	return 1;
}

#ifdef DEBUG0
	/*!
	 * Function to display command line arguments
	 */
	void displayArguments(int argc, char *argv[])
	{
		int i;

		for(i=0;i<argc;i++)
		{
			fprintf(stdout,"DEBUG:arg[%d]=%s\n",i,argv[i]);
		}
	}
#endif
