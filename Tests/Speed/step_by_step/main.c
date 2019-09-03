#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <limits.h>

#include "choiceFreeGeneratorDev.h"

#define NB_SIMULATION 100
#define SIZE 100
#define DENSITY 0.5
#define AVG 5
#define NB_STEP 8

void initializeDArray(double * array, unsigned int size, double val)
{
    int i;
    for(i=0; i<size; i++)
    {
        array[i] = val;
    }
}

void displayVal(double * vals, char ** label, unsigned int size)
{
    double tot = 0;
    int i;
    for(i=0; i<size-1; i++)
    {
        fprintf(stderr, "%s=%lf  |", label[i], vals[i]);
        tot+= vals[i];
    }
    fprintf(stderr, "%s=%lf\n", label[i], vals[i]);
    tot+= vals[i];

    fprintf(stderr, "\tTotal time=%lf\n", tot);
}


int main()
{
	srand(time(NULL));

    clock_t start, end;
    double cpu_time_used;


	unsigned int nb_transition = SIZE;

    double density = DENSITY;
    unsigned int vect_norm = AVG*SIZE;
    int cleanExtraMemSpace = 0;
    unsigned int real_vect_norm;
    unsigned int * repetition_vect;

    double avg[NB_STEP], tmp_avg[NB_STEP];
    char * label[NB_STEP] = {"avg_random_graph", "avg_strongly_connected", "avg_SDF_conversion", "avg_weight", "avg_normalization",
                             "avg_init_marking", "avg_ChoiceFree_transformation", "avg_free"};

    int k, i;

    initializeDArray(avg, NB_STEP, 0.);
    for(k=0; k<NB_SIMULATION; k++)
    {
        i = 0;

        printf("Generation of a random oriented graph...\n");
        start = clock();
        pDirectedGraph graph = randomConnectedGraph(nb_transition, density);
        end = clock();

        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        tmp_avg[i] = cpu_time_used; 
        avg[i]+= tmp_avg[i]; i++;


        printf("Conversion to strongly connected graph...\n");
        start = clock();
        stronglyConnectedGraph(graph, 0);
        end = clock();

        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        tmp_avg[i] = cpu_time_used;
        avg[i]+= tmp_avg[i]; i++;


        printf("Conversion to SDF...\n");
        start = clock();
        pPetri net = petriTransformation(graph);
        end = clock();

        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        tmp_avg[i] = cpu_time_used;
        avg[i]+= tmp_avg[i]; i++;


        printf("Weight generation...\n");
        start = clock();
        repetition_vect = weightsComputation(&real_vect_norm, net->nb_tr, vect_norm);
        end = clock();

        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        tmp_avg[i] = cpu_time_used; 
        avg[i]+= tmp_avg[i]; i++;


        printf("Normalization...\n");
        start = clock();
        normalizationPetriNetwork(net, repetition_vect);
        end = clock();

        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        tmp_avg[i] = cpu_time_used;
        avg[i]+= tmp_avg[i]; i++;


        printf("Computation of the initial marking...\n");
        start = clock();
        setInitialMarking(net);
        end = clock();

        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        tmp_avg[i] = cpu_time_used;
        avg[i]+= tmp_avg[i]; i++;


        printf("Conversion to Free-choice...\n");
        start = clock();
        sdfToChoiceFree(net, cleanExtraMemSpace);
        end = clock();

        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        tmp_avg[i] = cpu_time_used;
        avg[i]+= tmp_avg[i]; i++;


        start = clock();
        directedGraphFree(graph);
        petriFree(net);
        free(repetition_vect);
        end = clock();

        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        tmp_avg[i] = cpu_time_used;
        avg[i]+= tmp_avg[i]; i++;


        fprintf(stderr, "\n-------------------\n");
        fprintf(stderr, "\nGeneration : %u/%u\n", k, NB_SIMULATION);
        displayVal(tmp_avg, label, NB_STEP);
        fprintf(stderr, "\n-------------------\n");
    }

    for(k=0; k<NB_STEP; k++)
    {
        avg[i]/=NB_SIMULATION;
    }
	
    displayVal(tmp_avg, label, NB_STEP);

	return 0;
}
