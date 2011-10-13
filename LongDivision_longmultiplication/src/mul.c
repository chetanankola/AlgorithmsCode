#include "../headers/mul.h"
/*!
 * This function multiplies 2 unsigned 32 bit integer and returns a unsigned 64 integer
 * @param 1: unsigned 32 bit integer
 * @param 2: unsigned 32 bit integer
 * Return: unsigned 64 bit integer
Eg:      827395
    x     26153
    -----------
        2482185
       4136975
       827395 
     4964370
    1654790 
    -----------
    21638861435 

*/
/*!
 * Function to display the number onto the standard output
 * @param: numofspaces tells teh number of spaces to be printed before displaying the number
 */
void displayNumber(int numofspaces,uint64_t num)
{
	int i =0;

	for(i=0;i<numofspaces;i++)
		fprintf(stdout," ");
	fprintf(stdout,"%lld",num);


}

void displayNumber32(int numofspaces,uint32_t num)
{
	int i =0;

	for(i=0;i<numofspaces;i++)
		fprintf(stdout," ");
	fprintf(stdout,"%d",num);


}
void displayString(int numofspaces,char *str)
{
	int i =0;
	for(i=0;i<numofspaces;i++)
		fprintf(stdout," ");
	fprintf(stdout,"%s",str);
}

/*!
 * function returns number of digits in the number
 */
int getlen(uint64_t num)
{
	int len = 0;	
	if(num == 0)
	{
		len = 1;
		return len;	
	}
	while(num!=0)
	{
		num = num/10;
		len++;
	}
	return len;
}

/*!
 * Function multiplies two unsigned 32 bit integers and returns a unsigned 64 bit integer
 * Function also displays the result in formatted manner.
 * @param1: n1 : unsigned 32 bit integer - Multiplicand
 * @param2: n2 : unsigned 32 bit integer - Multiplier
 * return: result of long multiplication type: unsigned 64 bit integer
 */

uint64_t LongMul(uint32_t n1,uint32_t n2)
{
	uint64_t product = 0;				///< final multiplication product.
	uint32_t tempn2 = n2;				///< temporary n2 which gets divided by 10 everytime in the loop
	uint32_t temp = 0;					///< holds each integer temporarily from n1 for multiplication
	int outputlen = 0;				///< formatting purposes
	uint64_t tempresults[MAXLENOFOUTPUT]= {0};	///< holds the intermediate results of each multiplication
	int productlen = 0; 				///< total number of digits of the product
	int n1len = 0;
	int n2len = 0;
	int i = 0;
	int j = 0;
	uint64_t tens = 1;
	int formatlen = 0;
	/*TO DO CHECK IF N1 or N2 is zero*/


	while(tempn2!=0)
	{
		temp = tempn2%10;
		tempn2 = tempn2/10;
		tempresults[outputlen] = (uint64_t)temp * (uint64_t)n1;		///< type casting is so important :--/ 
		#ifdef DEBUG0	
		fprintf(stderr,"DEBUG:LongMul(uint32_t,uint32_t):intermediate=%d * %d = %lld ",temp,n1,tempresults[outputlen]);
		fprintf(stderr,"temp = %d    tempn2=%d\n",temp,tempn2);
		#endif	
		product = product + (tempresults[outputlen]*tens);
		tens = tens *10;
		outputlen++;	
	}

	#ifdef DEBUG0
	fprintf(stdout,"DEBUG:LongMul(uint32_t,uint32_t):n1:%d  x n2:%d = %lld\n",n1,n2,product);
	#endif	

	productlen = getlen(product);
	n1len = getlen(n1);
	n2len = getlen(n2);
	formatlen = productlen;
	if(n1len>=formatlen) formatlen = n1len + 2;
	if(n2len>=formatlen) formatlen = n2len + 2;	

 	if(n2len+2 > productlen && n2len < productlen) 					///< if the difference between n2 and product is 1 with n2<product
		formatlen = n2len+2;

	fprintf(stdout,"\t");
	displayNumber((formatlen-n1len),n1); 						///<    n1
	fprintf(stdout,"\n");								
	fprintf(stdout,"\t");
	displayString(0,"x"); 								///<  x n2
	fprintf(stdout," "); 								
	displayNumber((formatlen-n2len-2),n2);						
	fprintf(stdout,"\n");
	fprintf(stdout,"\t");
	for(i=0;i<formatlen;i++)
		fprintf(stdout,"-");							///< ------


	if(n1==0 || n2 ==0)
	{
		fprintf(stdout,"\n");
		fprintf(stdout,"\t");
		displayNumber(formatlen-1,product);
		fprintf(stdout,"\n");
		return product;
	}


	for(i=0;i<outputlen;i++)
	{
		fprintf(stdout,"\n");							///<   289
		fprintf(stdout,"\t");							///<  212
		displayNumber(formatlen-getlen(tempresults[i])-i,tempresults[i]);	///< 123
		for(j=0;j<i;j++)
			fprintf(stdout," ");
		
	}
	fprintf(stdout,"\n");
	fprintf(stdout,"\t");
	for(i=0;i<formatlen;i++)
		fprintf(stdout,"-");							///< -------
	fprintf(stdout,"\n");
	fprintf(stdout,"\t");
	if(formatlen>productlen)
		displayNumber(formatlen-productlen,product);
	else
		fprintf(stdout,"%lld",product);						///< product
	fprintf(stdout,"\n");
return product;
}


