#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>


#include "Tools.h"

#define CHAR_ARRAY_SEPARATOR '|'
#define CHAR_LIST_SEPARATOR '/'
#define CHAR_DATA_SEPARATOR ';'

/**********************************************************************
 * 
 * 
 * 						DISPLAY FUNCTIONS
 * 
 **********************************************************************/
 
void displayGraph(pArray * list, unsigned int n)
{
	//display a linked list
	unsigned int i;
	pArray cursor;
	
	for(i=0; i<n; i++)
	{
		cursor = list[i];
		
		printf("tree[%d] = {", i);
		if(cursor==NULL)
		{
			printf(" }");
		}else
		{
			while(cursor->next!=NULL)
			{
				printf("%u ,", uIntValue(cursor));
				cursor = cursor->next;
			}
			printf("%u}", uIntValue(cursor));
		}
		printf("\n");
	}
}

void displaySimpeNodeArray(pArray p)
{
	//display a list of linked lists
	printf("{");
	if(p==NULL)
	{
		printf(" }");
	}else
	{
		while(p->next!=NULL)
		{
			printf("%u ,", uIntValue(p));
			p = p->next;
		}
		printf("%u}", uIntValue(p));
	}
	printf("\n");
}

 void displayMatrix(int a[], unsigned int num_l, unsigned int num_c)
 {
	 unsigned int i,j;
	 
	 for(i=0; i<num_l; i++)
	 {
		 for(j=0; j<num_c; j++)
		 {
			 fprintf(stderr, "%d ",a[i*num_c+j]);
		 }
		 fprintf(stderr, "\n");
		 
	 }
	 fprintf(stderr, "\n");
 }

/**********************************************************************
 * 
 * 
 * 						TO STRING FUNCTIONS
 * 
 **********************************************************************/
unsigned int getNumberOfDigit(int num)
{
	if(num==0)
		return 1;
	
	return floor(log10(abs(num))) + 1;
}

unsigned int arrayToString(char ** output, pArray p)
{
	/* Return a array of char that represent the linked list as the following
	 * format : 
	 * data_1 SEPARTOR_CHAR data_2 ... data_end
	 * where the two space between data_i, SEPARTOR_CHAR, data_(i+1) don't exists in reality
	 * For example if SEPARTOR_CHAR='-' the the output will be :
	 * 	"data_1-data_2-...data_end"
	 * 
	 * Parameters :
	 * 		output : pointer of pointer that will be used to save the reference of the output value (the linked list into a string)
	 * 		p : linked list to convert into string
	 * Output :
	 * 		output set with the reference of the linked list as string
	 * 		the size of the string
	*/
	
	//get the total amout of needed space 
	pArray cursor = p;
	unsigned int count;
	unsigned int data;
	unsigned int max_digits;
	unsigned int digit;
	
	count=0;
	max_digits = 0;
	while(cursor!=NULL)
	{
		data = uIntValue(cursor);
		digit = getNumberOfDigit(data);//number of digits in the current data number
		
		count+= digit;
		if(cursor->next!=NULL)//if there is an other element
		{
			count++;//add one element to the count for the speration between element of the linked list
		}
		
		if(digit>max_digits)
		{
			max_digits = digit;//get the maximum number of digit in the whole list (for later)
		}
		
		cursor=cursor->next;
	}
	
	//build the string
	char * string = (char *)malloc(sizeof(char)*(count+1)); //count+1 to add the end char '\0'
	assert(string);
	
	unsigned int i,k;
	char digits[max_digits+1];//+1 for the end char '\0'
	
	
	i = 0;//index of the current element in the array of char
	while(p)
	{
		sprintf(digits, "%u", uIntValue(p));//convert the data into string
		
		k=0;
		while(digits[k]!='\0')//copy the string value of the current data
		{
			string[i] = digits[k];
			
			k++; i++;
		}
		
		if(p->next!=NULL)
		{
			string[i] = CHAR_ARRAY_SEPARATOR;//add the separator
			i++;
		}
		
		p = p->next;
	}
	string[i] = '\0';
	
	//return the result
	*output = string;
	return count;//size of the string
}

unsigned int listToString(char ** output, pArray a[], unsigned int n)
{	
	char * linked_string[n];//save all the string of each element inside the list of linked list
	
	unsigned int i;
	unsigned int size;
	
	size = 0;
	for(i=0; i<n; i++)
	{
		size+= arrayToString(linked_string+i, a[i]);
		size++;//add the separator char
	}
	size--; //remove the last separator char
	
	char * string = (char *)malloc(sizeof(char)*(size+2));//size +1 for the end char '\0' and +1 for the last char separator
	assert(string);
	
	char * cursor;
	unsigned int j;
	
	j = 0;
	for(i=0; i<n; i++)
	{
		cursor = linked_string[i];
		
		while(*cursor)
		{
			string[j] = *cursor;
			
			cursor++;
			j++;
		}
		
		string[j] = CHAR_LIST_SEPARATOR;
		j++;

	}
	
	string[j] = '\0';
	
	//free the linked string list
	for(i=0; i<n ; i++)
	{
		free(linked_string[i]);
	}
	
	//return the result
	*output = string;
	return size;
}

unsigned int matrixLineToString(char ** output, int * line, unsigned int n)
{
	char * linked_string[n];
	char * cursor;
	unsigned int digit;
	unsigned int i;
	unsigned int count;
	
	int * l_cursor;
	int * size = line+n;
	
	//convert all the value inside a temporary array
	count = 0;
	for(l_cursor=line, i=0; l_cursor<size; l_cursor++, i++)
	{
		digit = getNumberOfDigit(*l_cursor);
		count+=(digit+1);//+1 for the separator char
		linked_string[i] = (char *)malloc(sizeof(char)*(digit+1));
		assert(linked_string[i]);
		sprintf(linked_string[i], "%d", *l_cursor);//convert the data into string
	}
	count--;//-1 because the last element does not need to be followed by a spearator char
	
	char * _output = (char *)malloc(sizeof(char)*(count+1));//+1 for the end char '\0'
	assert(_output);
	char * temp;
	
	for(i=0, cursor = _output; i<n; i++, cursor++)
	{
		temp = linked_string[i];
		while(*temp)
		{
			*cursor = *temp;
			temp++; cursor++;
		}
		if(i!=n-1)
		{
			*cursor = CHAR_ARRAY_SEPARATOR;
		}else
		{
			cursor--;
		}
	}
	*cursor ='\0';
	
	//free
	for(i=0; i<n; i++)
	{
		free(linked_string[i]);
	}
	
	//return results
	*output = _output;
	return count;
}

unsigned int matrixToString(char ** output, int * matrix, unsigned int num_l, unsigned int num_c)
{
	char * linked_string[num_l];
	unsigned int i;
	unsigned int count;
	
	int * cursor_l;
	unsigned int j =0;
	count = 0;
	for(i=0, cursor_l = matrix; i<num_l; i++, cursor_l+=num_c, j+=num_c)
	{
		count+= matrixLineToString(linked_string + i, cursor_l, num_c);
		count++;//separator char
	}
	count--;//one loop to far (no separator char next to the last element)
	
	char * _output = (char *)malloc(sizeof(char)*(count+1));//+1 for the end char '\0'
	assert(_output);
	char * cursor, *temp;
	
	for(i=0, cursor=_output; i<num_l; i++, cursor++)
	{
		temp = linked_string[i];
		while(*temp)
		{
			*cursor=*temp;
			temp++; cursor++;
		}
		if(i!=num_l-1)
		{
			*cursor = CHAR_LIST_SEPARATOR;
		}else
		{
			cursor--;
		}
	}
	*cursor='\0';
	
	//free
	for(i=0; i<num_l; i++)
	{
		free(linked_string[i]);
	}
	
	//return results
	*output = _output;
	return count;
}

unsigned int petriToString(char ** output, pPetri p)
{
	char * places;
	char * trans;
	unsigned int count =0;
	
	count+= matrixToString(&places, p->places, 1, p->num_pl);
	count++;//for the separator char
	count+= matrixToString(&trans, p->trans, p->num_tr, p->num_pl);
	
	char * _output = (char *)malloc(sizeof(char)*(count+1));//+1 for the end char '\0'
	assert(output);
	char * cursor, *temp_cursor;
	
	cursor = _output;
	
	temp_cursor = places;
	while(*temp_cursor)
	{
		*cursor = *temp_cursor;
		temp_cursor++;cursor++;
	}
	*cursor = CHAR_DATA_SEPARATOR;
	cursor++;
	
	temp_cursor = trans;
	while(*temp_cursor)
	{
		*cursor = *temp_cursor;
		temp_cursor++;cursor++;
	}
	*cursor='\0';
	
	//free
	free(places);
	free(trans);
	
	//return results
	*output=_output;
	return count;
}


/**********************************************************************
 * 
 * 
 * 						FILE FUNCTIONS
 * 
 **********************************************************************/
 
int listToFile(char * filename, pArray a[], unsigned int n)
{
	FILE * file = fopen(filename, "w");
	if(file==NULL)
	{
		fprintf(stderr, "Ouverture du fichier <<%s>> impossible\n", filename);
		return -1;
	}
	
	pArray p;
	unsigned int i,j;
	unsigned int max_number_digit = getNumberOfDigit(n);
	char digits[max_number_digit+1];//+1 for the end char '\0'
	
	
	for(i=0; i<n; i++)
	{
		p = a[i];
		
		while(p)//loop through the element of the linked list of the current element (index i)
		{
			sprintf(digits, "%u", uIntValue(p));//convert number into string
			//write the number into the file
			j = 0;
			while(digits[j])
			{
				fputc(digits[j],file);
				j++;
			}
			
			if(p->next!=NULL)//if there is a next element
			{
				fputc(CHAR_ARRAY_SEPARATOR, file);
			}
			
			p = p->next;
		}
		//all the element of the linked list has been written into the file
		if(i!=n-1)//if there is a next element
		{
			fputc(CHAR_LIST_SEPARATOR, file);
		}
		
	}
	fputc('\n', file);
	
	fclose(file);
	
	return 0;
}
