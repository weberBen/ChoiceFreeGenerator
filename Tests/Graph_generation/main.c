#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <limits.h>

#include "choiceFreeGeneratorDev.h"

#define NB_SIMULATION 1
#define SIZE 10000
#define OUT 500
#define NUM_OVER 2
#define TREE 0

void initializeuIntArray(unsigned int * array, unsigned int size, unsigned int val)
{
    int i;
    for(i=0; i<size; i++)
    {
        array[i] = val;
    }
}


unsigned int * getNumInputs(pDirectedGraph graph)
{
    unsigned int * output = (unsigned int *)malloc(sizeof(unsigned int)*graph->nb_nodes);
    initializeuIntArray(output, graph->nb_nodes, 0);


    int i;
    int val;
    pArray p;

    for(i=0; i<graph->nb_nodes; i++)
    {
        p = graph->links_list[i];
        while(p)
        {
            val = uIntValue(p);
            output[val]++;
            p = p->next;
        }
    }

    return output;
}

double avgArray(unsigned int * array, unsigned int size)
{
    int i;
    double avg = 0;

    for(i=0; i<size; i++)
    {
        avg+=array[i];
    }

    return avg/size;
}


unsigned int maxArray(unsigned int * array, unsigned int size)
{
    int i;
    unsigned int max = array[0];

    for(i=1; i<size; i++)
    {
        if(array[i]>max)
            max = array[i];
    }

    return max;
}


unsigned int minArray(unsigned int * array, unsigned int size)
{
    int i;
    unsigned int min = array[0];

    for(i=1; i<size; i++)
    {
        if(array[i]<min)
            min = array[i];
    }

    return min;
}

unsigned int numValueOver(unsigned int * array, unsigned int size, unsigned int val)
{
    int i;
    unsigned int count = 0;

    for(i=0; i<size; i++)
    {
        if(array[i]>=val)
            count++;
    }

    return count;
}

int main()
{
	srand(time(NULL));

	//-----------------------------------------------------------------------------------------------------------------
	//										GENERATE RANDOM FREE CHOICE 
	//-----------------------------------------------------------------------------------------------------------------

	unsigned int nb_nodes = SIZE;//number of desired transition in the result Free-choice
	//unsigned int nb_input_node = IN;//average (and maximum) number of inputs for each transition
	unsigned int nb_output_node = OUT;//average (and maximum) number of outputs for each transition

    pDirectedGraph graph;


    int k;
    unsigned int max_input, tmp_max_input, num_over, min_input, tmp_min_input;
    double avg_input, tmp_avg_input, avg_num_over;
    unsigned int * tmpArray;

    avg_input = 0;
    max_input = 0;
    min_input = UINT_MAX;
    avg_num_over = 0;
    for(k=0; k<NB_SIMULATION; k++)
    {
        
        if(TREE)
        {
            graph = buildTree(nb_nodes, nb_output_node);
            stronglyConnectedGraph(graph, 1);
        }else
        {
            graph = randomOrientedGraph(nb_nodes, nb_output_node, nb_output_node);
            stronglyConnectedGraph(graph, 0);
        }
        tmpArray = getNumInputs(graph);

        tmp_avg_input = avgArray(tmpArray, graph->nb_nodes);
        avg_input+=tmp_avg_input;

        tmp_max_input = maxArray(tmpArray, graph->nb_nodes);
        if(tmp_max_input>max_input)
            max_input = tmp_max_input;

        tmp_min_input = minArray(tmpArray, graph->nb_nodes);
        if(tmp_min_input<min_input)
            min_input = tmp_min_input;

        num_over = numValueOver(tmpArray, graph->nb_nodes, NUM_OVER);
        avg_num_over+=num_over;


        fprintf(stderr, "\n-------------------\n");
        fprintf(stderr, "\nGeneration : %u/%u\n", k, NB_SIMULATION);
        fprintf(stderr, "avg_input=%lf   |  max_input=%u   |  min_input=%u  |  num_over %u=%u", tmp_avg_input, tmp_max_input, tmp_min_input,  NUM_OVER, num_over);
        fprintf(stderr, "\n-------------------\n");

        directedGraphFree(graph); 
        free(tmpArray); 
    }
	
    fprintf(stderr, "\navg_input=%lf   |   max_input=%u   |   min_input=%u   | avg_num_over %u=%lf  | pourcent num_over = %lf\n",avg_input/NB_SIMULATION, max_input, min_input, 
                                                                                                                        NUM_OVER, avg_num_over/NB_SIMULATION, avg_num_over/NB_SIMULATION/SIZE*100);

	return 0;
}
