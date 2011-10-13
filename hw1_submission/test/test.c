#include "test.h"


#ifdef DEBUG0
int testcount = 0;

void runMulSuite()
{
	char *argv[] = {"hw1","mul","2","4"};

	///< Case 1
	uint32_t n1 = 827395;
	uint32_t n2 = 26153;
	argv[0] = "hw1";
 	argv[1] = "mul";
	argv[2] = "827395";
	argv[3] = "26153";
	testmul(4,argv,n1,n2);

	///< Case 2
	n1 = 5;
	n2 = 3;
	argv[0] = "hw1";
 	argv[1] = "mul";
	argv[2] = "5";
	argv[3] = "3";
	testmul(4,argv,n1,n2);


	///< Case 3
	n1 = 5;
	n2 = 0;
	argv[0] = "hw1";
 	argv[1] = "mul";
	argv[2] = "5";
	argv[3] = "0";
	testmul(4,argv,n1,n2);

	///< Case 4
	n1 = 0;
	n2 = 23123;
	argv[0] = "hw1";
 	argv[1] = "mul";
	argv[2] = "0";
	argv[3] = "23123";
	testmul(4,argv,n1,n2);

	///< Case 5
	n1 = 5672;
	n2 = 0;
	argv[0] = "hw1";
 	argv[1] = "mul";
	argv[2] = "5672";
	argv[3] = "0";
	testmul(4,argv,n1,n2);

	///< Case 5
	n1 = 1;
	n2 = 23123;
	argv[0] = "hw1";
 	argv[1] = "mul";
	argv[2] = "1";
	argv[3] = "23123";
	testmul(4,argv,n1,n2);

	///< Case 5
	n1 = 111111;
	n2 = 1;
	argv[0] = "hw1";
 	argv[1] = "mul";
	argv[2] = "111111";
	argv[3] = "1";
	testmul(4,argv,n1,n2);

	///< Case 6
	n1 = 13;
	n2 = 455;
	argv[0] = "hw1";
 	argv[1] = "mul";
	argv[2] = "13";
	argv[3] = "455";
	testmul(4,argv,n1,n2);

	///< Case 7
	n1 = 455;
	n2 = 13;
	argv[0] = "hw1";
 	argv[1] = "mul";
	argv[2] = "455";
	argv[3] = "13";
	testmul(4,argv,n1,n2);


}


void runDivSuite()
{
	char *argv[] = {"hw1","div","2","4"};

	///< Case 1
	uint32_t n1 = 827395;
	uint32_t n2 = 26153;
	argv[0] = "hw1";
 	argv[1] = "div";
	argv[2] = "827395";
	argv[3] = "26153";
	testdiv(4,argv,n1,n2);

	///< Case 2
	n1 = 1234;
	n2 = 12;
	argv[0] = "hw1";
 	argv[1] = "div";
	argv[2] = "1234";
	argv[3] = "12";
	testdiv(4,argv,n1,n2);

	///< Case 2
	n1 = 234234;
	n2 = 23432;
	argv[0] = "hw1";
 	argv[1] = "div";
	argv[2] = "234234";
	argv[3] = "23432";
	testdiv(4,argv,n1,n2);

	///< Case 2
	/*n1 = (uint64_t)4294967295;
	n2 = 1001;
	argv[0] = "hw1";
 	argv[1] = "div";
	argv[2] = "4294967295";
	argv[3] = "1001;";
	testdiv(4,argv,n1,n2);
	*/
}



void runTestSuite()
{
	///< malformed commandline testcase 1
	char *argv[] = {"hw1","mul","2","4"};
	testcmdline(4,argv);


	///< malformed commandline testcase 2
	argv[2]= "asdasdasdas";
	testcmdline(4,argv);	
	
	///< invalid n2 testcase 3
	argv[1]= "div";
	argv[2] ="123";
	argv[3] ="asd";
	testcmdline(4,argv);	

	///< invalid n1 and n2 testcase 4
	argv[1]= "div";
	argv[2] ="asds";
	argv[3] ="asd";
	testcmdline(4,argv);	
	
	///< invalid n1 testcase 5
	argv[1]= "div";
	argv[2] ="as";
	argv[3] ="312";
	testcmdline(4,argv);

	///< division by zero - invalid n2 testcase 6
	argv[1]= "div";
	argv[2] ="123";
	argv[3] ="0000";
	testcmdline(4,argv);

	///< too many argument testcase 7
	argv[1]= "div";
	argv[2] ="123";
	argv[3] ="000";
	argv[4] ="123";
	testcmdline(5,argv);

	///< n1 out of range testcase 8  this is classic case where atol of 4294967296 becomes 0
	argv[1]= "div";
	argv[2] ="4294967296";
	argv[3] ="56";
	testcmdline(4,argv);	


	///< n2 out of range testcase 9
	argv[1]= "div";
	argv[2] ="459";
	argv[3] ="-1";
	testcmdline(4,argv);	

	///< n2 out of range testcase 
	argv[1]= "div";
	argv[2] ="2";
	argv[3] ="42949672912"; 
	testcmdline(4,argv);		

	///< n1 out of range testcase 10
	argv[1]= "div";
	argv[2] ="4294967297";
	argv[3] ="56";
	testcmdline(4,argv);	

	///< n1 out of range testcase 11
	argv[1]= "mul";
	argv[2] ="asd7213";
	argv[3] ="56";
	testcmdline(4,argv);	
	
}


int testmul(int argc,char **argv,uint32_t n1, uint32_t n2)
{
	int status;
	testcount++;
	fprintf(stdout,"==========================================================================\n");	
	status = processArguments(argc, argv);
	if(!status) 
	{

		displayArguments(argc, argv);
		fprintf(stdout,"DEBUG:testcmdline(): Testcase %d Failed!!!!---------------------------\n\n\n",testcount);
		//displayUsage();
		return 0;
	}
	displayArguments(argc, argv);
	LongMul(n1,n2);
	fprintf(stdout,"DEBUG:testcmdline(): Testcase %d passed!!!!---------------------------\n\n\n",testcount);
	return 1;
}

int testdiv(int argc,char **argv,uint32_t n1, uint32_t n2)
{
	int status;
	testcount++;
	fprintf(stdout,"==========================================================================\n");	
	status = processArguments(argc, argv);
	if(!status) 
	{

		displayArguments(argc, argv);
		fprintf(stdout,"DEBUG:testcmdline(): Testcase %d Failed!!!!---------------------------\n\n\n",testcount);
		//displayUsage();
		return 0;
	}
	displayArguments(argc, argv);
	LongDiv(n1,n2);
	fprintf(stdout,"DEBUG:testcmdline(): Testcase %d passed!!!!---------------------------\n\n\n",testcount);
	return 1;
}


int testcmdline(int argc,char **argv)
{
	int status;

	testcount++;
	fprintf(stdout,"==========================================================================\n");	
	status = processArguments(argc, argv);
	if(!status) 
	{

		displayArguments(argc, argv);
		fprintf(stdout,"DEBUG:testcmdline(): Testcase %d Failed!!!!---------------------------\n\n\n",testcount);
		//displayUsage();
		return 0;
	}
	displayArguments(argc, argv);
	fprintf(stdout,"DEBUG:testcmdline(): Testcase %d passed!!!!---------------------------\n\n\n",testcount);
	return 1;
}

#endif
