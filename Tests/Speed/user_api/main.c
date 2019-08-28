#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>

#include "freeChoiceGenerator.h"

#define NB_SIMULATION 100
#define SIZE 100
#define IN  3
#define OUT 3
#define AVG_RT 5

int getMax(unsigned int * array, unsigned int size)
{
    if(array==NULL || size==0)
        return -1;

    int i;
    unsigned int max = array[0];
    for(i=1; i<size; i++)
    {
        if(max<array[i])
            max = array[i];
    }

    return max;
}


unsigned int initialMarkingSum(pPetri net)
{
    unsigned int init_marking_sum = 0;
    int i;

    for(i=0; i<net->nb_pl; i++)
    {
        if(net->pl_elems[i]!=NULL)
            init_marking_sum+=net->pl_elems[i]->val;
    }

    return init_marking_sum;
}

int main()
{
	srand(time(NULL));

	//-----------------------------------------------------------------------------------------------------------------
	//										GENERATE RANDOM FREE CHOICE 
	//-----------------------------------------------------------------------------------------------------------------

    clock_t start, end;
    double cpu_time_used;

	unsigned int nb_transition = SIZE;//number of desired transition in the result Free-choice
	unsigned int nb_input_node = IN;//average (and maximum) number of inputs for each transition
	unsigned int nb_output_node = OUT;//average (and maximum) number of outputs for each transition
	unsigned int vect_norm = AVG_RT*nb_transition;//norm of the repetition vector to generate
    unsigned int real_vect_norm = 0;
	int cleanExtraMem = 0;//the transformation from SDF to Free-choice leaves extra empty memory space, the cleaning process is optional because it's time consuming


    unsigned int * vect;
    pPetri net;
    unsigned int tmp_max, max_Rt;
    int k;
    double avg_time;
    double max_avg_Rt, tmp_d;
    unsigned int avg_init_markintg_sum = 0;
    unsigned int init_marking_sum;

    max_Rt = 0;
    avg_time = 0;
    max_avg_Rt = 0;
    for(k=0; k<NB_SIMULATION; k++)
    {
        start = clock();

        vect = weightsComputation(&real_vect_norm, nb_transition, vect_norm);
        net = generateFreeChoiceWithVector(nb_transition, nb_input_node, nb_output_node, vect, cleanExtraMem);
        //net = generateRandomFreeChoice(&real_vect_norm, nb_transition, nb_input_node, nb_output_node, vect_norm, cleanExtraMem);

        end = clock();


        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        avg_time += cpu_time_used;

        tmp_max = getMax(vect, nb_transition);
        if(tmp_max>max_Rt)
            max_Rt = tmp_max;

        tmp_d = 1.*real_vect_norm/nb_transition;
        if(tmp_d>max_avg_Rt)
            max_avg_Rt =  tmp_d;
        
        init_marking_sum = initialMarkingSum(net);
        avg_init_markintg_sum+=init_marking_sum;
        
        fprintf(stderr, "\n-------------------\n");
        fprintf(stderr, "\nGeneration : %u/%u\n", k, NB_SIMULATION);
        fprintf(stderr, "time=%lf   |  avg(rt)=%lf   |  max=%u    |  avg_sum=%u",cpu_time_used, tmp_d, tmp_max, init_marking_sum);
        fprintf(stderr, "\n-------------------\n");

        free(vect);
        petriFree(net);
	    
    }
	
    fprintf(stderr, "\navg time=%lf  |  max avg Rt=%lf  | max Rt=%u  | avg_sum=%lf\n", avg_time/NB_SIMULATION, max_avg_Rt, max_Rt, 1.*avg_init_markintg_sum/NB_SIMULATION);

	return 0;
}
