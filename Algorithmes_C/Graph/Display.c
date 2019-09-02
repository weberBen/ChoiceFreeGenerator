#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "Display.h"

 
 /**********************************************************************
 * 
 * 
 * 						DISPLAY STANDARD OUTPUT
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


void displaySimpeNodeArray2(pArray2 p)
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
			printf("%u ,", ((pInt)(p->data))->data);
			p = p->next;
		}
		printf("%u}", ((pInt)(p->data))->data);
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
			 printf("%d ",a[i*num_c+j]);
		 }
		 printf("\n");
		 
	 }
	 printf("\n");
 }

 void displayArray(double * p, unsigned int size)
 {
		unsigned int i;
		printf("{");
		for(i=0; i<size-1; i++)
		{
			printf("%lf | ", p[i]);
		}
		printf("%lf}", p[size-1]);
		printf("\n");
 }


 void displayPartitionSet(pPartitionSet p)
 {
	 printf("----------- PARTITION SET ------------------\n");

	 if(p==NULL)
	 {
		 printf("[NULL]\n");
	 }else
	 {
		printf("Nb_partition = %u\n", p->nb_partitions);

		unsigned int i;
		for(i=0; i<p->nb_partitions; i++)
		{
			printf("\t* Parition %u : \n", i+1);
			printf("\t");
			displayMatrix((int*)(p->partitions[i]), 1, p->size_partitions[i]);
		}
	 }

	 printf("--------------------------------------------\n");
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
	
	unsigned int count = floor(log10(abs(num))) + 1;
	if(num<0)
	{
		count++;//take the char '-' for negative input
	}
	
	return count;
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
		*cursor = CHAR_ARRAY_SEPARATOR;
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
		*cursor = CHAR_LIST_SEPARATOR;
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
 

void displayPetriElem(pPetriElem p)
{
    if(p==NULL)
	{
		printf("Elem[NULL]");
		return;
	}

    if(p->type==PETRI_PLACE_TYPE)
        printf("Place");
    else
        printf("Transition");

    printf("[label=%d", p->label);

    if(p->type==PETRI_PLACE_TYPE)
        printf(", marquage=%d", p->val);
    
    printf("]");
} 

void displayPetriLink(pPetriLink p)
{
    if(p==NULL)
	{
		printf("Link[NULL]");
		return;
	}

    printf("Lien[");
    if(p->input->type==PETRI_PLACE_TYPE)
        printf("Place");
    else
        printf("Transition");
    printf("(%d)-->", p->input->label);

    if(p->output->type==PETRI_PLACE_TYPE)
        printf("Place");
    else
        printf("Transition");
    printf("(%d)", p->output->label);
    printf(", poids=%d]", p->weight);
}

void displayPetriNode(pPetriNode p, unsigned int index)
{
    if(p==NULL)
	{
		printf("Node %d : [NULL]\n", index);
		return;
	}
    
    printf("Noeud %d : \n", index);
    printf("\tnb_input=%d\n", p->nb_inputs);
    pArray a = p->input_links;
    pPetriLink link;
    while(a)
    {
        //curent node is the output inside the link
        link = petriNodeGetLinkFromArrayNode(a);
        if(link->input->type==PETRI_PLACE_TYPE)
            printf("Place");
        else
            printf("Transition");
        printf("(%d)  ", link->input->label);

        a = a->next;
    }
    printf("\n\tnb_output=%d\n", p->nb_outputs);

    a = p->output_links;
    while(a)
    {
        //curent node is the input inside the link
        link = petriNodeGetLinkFromArrayNode(a);
        if(link->output->type==PETRI_PLACE_TYPE)
            printf("Place");
        else
            printf("Transition");
        printf("(%d)  ", link->output->label);

        a = a->next;
    }
    printf("\n");
}

void displayPetriNet(pPetri net)
{
     printf("------------------------------------------------------------------------\n");
     printf("Reseau de Pétri : \n");

     if(net==NULL)
     {
        printf("NULL\n");
        return;
     }

     printf("\t ** Nombre de place : %d\n", net->nb_pl);
     
     int i;
     for(i=0; i<net->nb_pl; i++)
     {
         printf("\t\t");
         displayPetriElem(net->pl_elems[i]);
         printf("\n");
     }

     printf("\t ** Nombre de transitions : %d\n", net->nb_tr);

     for(i=0; i<net->nb_tr; i++)
     {
         printf("\t\t");
         displayPetriElem(net->tr_elems[i]);
         printf("\n");
     }

    printf("\t ** Nombre de liens : %d\n", net->nb_links);
    pArray2 links = net->links;

    while(links)
    {
        printf("\t\t");
        displayPetriLink((pPetriLink)(links->data));
        printf("\n");
        links = links->next;
    }

    printf("\t ** Noeuds du reseau (Places) \n");
    for(i=0; i<net->nb_pl; i++)
    {
         displayPetriNode(net->places[i], i);
    }

    printf("\t ** Noeuds du reseau (Transitions) \n");
    for(i=0; i<net->nb_tr; i++)
     {
         displayPetriNode(net->transitions[i], i);
     }

     printf("------------------------------------------------------------------------\n");

}


/**********************************************************************
 * 
 * 
 * 						PNML FUNCTIONS
 * 
 **********************************************************************/

 #define PNML_NET_INDENT "\t"
 #define PNML_NET_ELEM_INDENT PNML_NET_INDENT"\t"
 #define PNML_PAGE_INDENT PNML_NET_ELEM_INDENT
 #define PNML_PAGE_ELEM_INDENT PNML_PAGE_INDENT"\t"

 #define PNML_START_DOC(pnml_version, xml_version) \
    ("<?xml version=\""#xml_version"\"?>\n" \
    "<pnml xmlns=\"http://www.pnml.org/version-"#pnml_version"/grammar/pnml\">\n" \
    PNML_NET_INDENT"<net id=\"net\" type=\"http://www.pnml.org/version-"#pnml_version"/grammar/ptnet\">\n")

#define PNML_END_DOC \
    PNML_NET_INDENT"</net>\n" \
    "</pnml>\n" 

#define PNML_END_PAGE \
    PNML_PAGE_INDENT"</page>\n"

 void pnmlNameNetwork(char * buffer, char * name)
 {
    sprintf(buffer, PNML_NET_ELEM_INDENT"<name><text>%s</text></name>\n", name);
 }
 void pnmlStartPage(char * buffer, char * id, char * page_name)
 {
     sprintf(buffer, PNML_PAGE_INDENT"<page id=\"%s\">\n" \
                        PNML_PAGE_ELEM_INDENT"<name><text>%s</text></name>\n", id, page_name);
 }

 void pnmlPLaceId(char * buffer, unsigned int index)
 {
      sprintf(buffer, "p%d", index);
 }

 void pnmlTransitionId(char * buffer, unsigned int index)
 {
      sprintf(buffer, "t%d", index);
 }

 void pnmlLinkId(char * buffer, char * input_id, char * output_id)
 {
     sprintf(buffer, "a-%s-to-%s", input_id, output_id);
 }

 void pnmlPlace(char * buffer, char * id, unsigned int initialMarking)
 {
     sprintf(buffer, PNML_PAGE_ELEM_INDENT"<place id=\"%s\">\n" \
			            PNML_PAGE_ELEM_INDENT"\t""<name><text>%s</text></name>\n" \
				        PNML_PAGE_ELEM_INDENT"\t""<initialMarking><text>%u</text></initialMarking>\n" \
	                PNML_PAGE_ELEM_INDENT"</place>\n", id, id, initialMarking);

 }

 void pnmlTransition(char * buffer, char * id)
 {
     sprintf(buffer, PNML_PAGE_ELEM_INDENT"<transition id=\"%s\">\n" \
			            PNML_PAGE_ELEM_INDENT"\t""<name><text>%s</text></name>\n" \
	                PNML_PAGE_ELEM_INDENT"</transition>\n", id, id);
 }

 void pnmlLink(char * buffer, char * input_id, char * output_id, int weight)
 {
     char id[BUFFER_SIZE];
     pnmlLinkId(id, input_id, output_id);

     sprintf(buffer, PNML_PAGE_ELEM_INDENT"<arc id=\"%s\" source=\"%s\" target=\"%s\">\n" \
				        PNML_PAGE_ELEM_INDENT"\t""<inscription><text>%d</text></inscription>\n" \
	                 PNML_PAGE_ELEM_INDENT"</arc>\n", id, input_id, output_id, weight);
 }

 void _petriToPnml(pPetri net, char * network_name, int fileDescriptor)
 {
    int saved_stdout = dup(1);//save stdout output to re-establish the output as previously later
    dup2(fileDescriptor, 1);//redirect stdout to the file (more convenient)
    fflush(stdout);
    

    char buffer[BUFFER_SIZE];
    char input_id[BUFFER_SIZE], output_id[BUFFER_SIZE];

    //start document
    printf("%s", PNML_START_DOC(2009, 1.0));
    pnmlNameNetwork(buffer, network_name);
    printf("%s", buffer);
    pnmlStartPage(buffer, "page-01", "page 1 reseau");
    printf("%s", buffer);

    if(net!=NULL)
    {
        //add places and transitions
        int i;
        int limit_inf = min(net->nb_pl, net->nb_tr);
        int limit_sup = max(net->nb_pl, net->nb_tr);
        pPetriElem elem;

        for(i=0; i<limit_inf; i++)
        {
            //place
            elem = net->pl_elems[i];
            if(elem!=NULL)
            {
                pnmlPLaceId(input_id, i);
                pnmlPlace(buffer, input_id, elem->val);
                printf("%s", buffer);
            }

            //transition
            elem = net->tr_elems[i];
            if(elem!=NULL)
            {
                pnmlTransitionId(output_id, i);
                pnmlTransition(buffer, output_id);
                printf("%s", buffer);
            }
        }

		for(i=limit_inf; i<limit_sup; i++)//rest of places
		{
			if(i<net->nb_pl)
			{
				elem = net->pl_elems[i];
				if(elem!=NULL)
				{
					pnmlPLaceId(input_id, i);
					pnmlPlace(buffer, input_id, elem->val);
					printf("%s", buffer);
				}
			}

			if(i<net->nb_tr)
			{
				elem = net->tr_elems[i];
                if(elem!=NULL)
                {
                    pnmlTransitionId(output_id, i);
                    pnmlTransition(buffer, output_id);
                    printf("%s", buffer);
                }
			}
		}
        
        //add link
        pArray2 p = net->links;
        pPetriLink link;

        while(p)
        {
            link = (pPetriLink)(p->data);
            //get input id
            if(link->input->type==PETRI_PLACE_TYPE)
                pnmlPLaceId(input_id, link->input->label);
            else
                pnmlTransitionId(input_id, link->input->label);
            //get output id
            if(link->output->type==PETRI_PLACE_TYPE)
                pnmlPLaceId(output_id, link->output->label);
            else
                pnmlTransitionId(output_id, link->output->label);


            pnmlLink(buffer, input_id, output_id, link->weight);
            printf("%s", buffer);
            p = p->next;
        }
    
    }//end petri pointer not null

    //end document
    printf("%s", PNML_END_PAGE);
    printf("%s", PNML_END_DOC);

    dup2(saved_stdout, 1);//restore stdout to shell
 }
 
 void petriToPnmlDisplay(pPetri net, char * network_name)
 {
     _petriToPnml(net, network_name, dup(1));
 }


 int fileExists(const char * filename)
 {
    /* Return 0 if the file given by its path exists, else return not-null value*/
    struct stat sb;
	return !(stat(filename,&sb)==-1);
 }

 void petriToPnmlFile(pPetri net, char * network_name, char* filename)
 {
     int file;

    //initial writes to file (create or truncate existing file with new value)
    if(!fileExists(filename))
    {//create new file
        file = open(filename, O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR);
    }else
    {//truncate file
        file = open(filename, O_TRUNC | O_WRONLY);
    }

    if(file==-1)
    {
        fprintf(stderr, "Impossible d'ouvrir/de creer le fichier %s\n", filename);
        return ;
    }

    //write initial value to the opened file

    _petriToPnml(net, network_name, file);

    close(file);
 }

 /**********************************************************************
 * 
 * 
 * 						PETRI FUNCTIONS
 * 
 **********************************************************************/

 void petriWrite(pPetri net, int fileDescriptor)
 {
	 /* For a petri net with : 
	 		n places, marked as p with the index of the place, where M0(pi) is the initial marking of the place i
			m transitions, markes as t with the index of the transition
		Then the format of the output is (with no line break : '\n' and with no space ' ') : 

	 	 (p1 CHAR_INNER_DATA_SEPARATOR M0(pi) CHAR_LIST_SEPARATOR ... (pn CHAR_INNER_DATA_SEPARATOR M0(pn)) 
		 CHAR_DATA_SEPARATOR
		 t1 CHAR_LIST_SEPARATOR t2 CHAR_LIST_SEPARATOR ... tm
		 CHAR_DATA_SEPARATOR
		 (input_node CHAR_INNER_DATA_SEPARATOR output_node CHAR_INNER_DATA_SEPARATOR weigth) CHAR_LIST_SEPARATOR ... (input_node CHAR_INNER_DATA_SEPARATOR output_node CHAR_INNER_DATA_SEPARATOR weigth)
		 	where input_node is the label of the node from where
		 	the link start and output_node the label of the node where it stop (with a weight)
		 \0

		Example (with CHAR_LIST_SEPARATOR='\' and CHAR_DATA_SEPARATOR=';' and CHAR_INNER_DATA_SEPARATOR=',')
		(p0,2)/(p1,24)/(p2,22)/(p3,23)/(p4,21);t0/t1/t2/t3/t4;(t4,p4,63)/(t3,p3,36)/(t2,p2,67)/(t1,p1,26)/(t0,p0,62)/(p3,t4,12)/(p2,t4,54)/(p1,t4,52)/(p0,t4,75)

	*/
	 if(net==NULL)
	 {
	 	write(fileDescriptor, '\0', sizeof(char));
		return ;
	 }

	 int i;
	 int size;
	 char buffer[BUFFER_SIZE];

	//write places
	 for(i=0; i<net->nb_pl-1; i++)
	 {
		 if(net->pl_elems[i]==NULL)
			continue;
		
		 size = sprintf(buffer, "(p%u%c%u)%c", i, CHAR_INNER_DATA_SEPARATOR, net->pl_elems[i]->val, CHAR_LIST_SEPARATOR);
		 write(fileDescriptor, buffer, size);
	 }

	if(net->pl_elems[i]!=NULL)
	{
		 size = sprintf(buffer, "(p%u%c%u)", i, CHAR_INNER_DATA_SEPARATOR, net->pl_elems[i]->val);
		 write(fileDescriptor, buffer, size);
	}
	 
	 //data separator
	 buffer[0] = CHAR_DATA_SEPARATOR; buffer[1] = '\0';
	 write(fileDescriptor, buffer, sizeof(char));

	 //write transitions
	 for(i=0; i<net->nb_tr-1; i++)
	 {
		 if(net->tr_elems[i]==NULL)
			continue;
		
		 size = sprintf(buffer, "t%u%c", i, CHAR_LIST_SEPARATOR);
		 write(fileDescriptor, buffer, size);
	 }

	if(net->tr_elems[i]!=NULL)
	{
		 size = sprintf(buffer, "t%u", i);
		 write(fileDescriptor, buffer, size);
	}

	//data separator
	buffer[0] = CHAR_DATA_SEPARATOR; buffer[1] = '\0';
	write(fileDescriptor, buffer, sizeof(char));

	//write links
	pArray2 p = net->links;
	pPetriLink link;
	char tmp1, tmp2;

	while(p->next)
	{
		link = (pPetriLink)(p->data);

		if(link->input->type==PETRI_PLACE_TYPE)
			tmp1 = 'p';
		else
			tmp1  = 't';

		if(link->output->type==PETRI_TRANSITION_TYPE)
			tmp2 = 't';
		else
			tmp2  = 'p';
		
		//(id_input, id_output, weight) ex : (p0, t1, 2) : link from place p0 to transition t1 with a weight of 2
		size = sprintf(buffer, "(%c%u%c%c%u%c%u)%c", tmp1, link->input->label, CHAR_INNER_DATA_SEPARATOR,
													 tmp2, link->output->label, CHAR_INNER_DATA_SEPARATOR, 
													 link->weight, CHAR_LIST_SEPARATOR);
		write(fileDescriptor, buffer, size);
		p = p->next;
	}

	link = (pPetriLink)(p->data);

	if(link->input->type==PETRI_PLACE_TYPE)
		tmp1 = 'p';
	else
		tmp1  = 't';

	if(link->output->type==PETRI_TRANSITION_TYPE)
		tmp2 = 't';
	else
		tmp2  = 'p';

	size = sprintf(buffer, "(%c%u%c%c%u%c%u)", tmp1, link->input->label, CHAR_INNER_DATA_SEPARATOR,
											   tmp2, link->output->label, CHAR_INNER_DATA_SEPARATOR, link->weight);
	write(fileDescriptor, buffer, size);


	//end of data
	buffer[0] = '\0';
	write(fileDescriptor, buffer, sizeof(char));
 }
