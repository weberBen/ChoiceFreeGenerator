#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "GraphGenerator.h"
#include "Tools.h"
#include "Rand.h"
#include "Display.h"

#include <sys/types.h>
#include <unistd.h>
#include <sys/resource.h>
#include <glpk.h> 

/*
pPetri petriCreate(unsigned int nb_pl, unsigned int nb_tr);
void petriAddPlace(pPetri net, unsigned int index, unsigned int initial_marking);
void petriAddTransition(pPetri net, unsigned int index);
void petriAddlink(pPetri net, int input_type, unsigned int input, int output_type, unsigned int output, int weight);
void petriFree(pPetri p);
 */

 /*struct rlimit lim = {100000000, 100000000};

 	if (setrlimit(RLIMIT_STACK, &lim) == -1)
  		return 1;*/


int main(int argc, char ** argv)
{
pPetri net1 = generateRandomFreeChoice(10, 3, 3, 10, 0);
displayPetriNet(net1);
/*unsigned int * vect = weightsComputation(5, 4);
pPetri net1 = generateFreeChoiceWithVector(5,2,2,vect);
free(vect);*/
petriFree(net1);
return 0;
/*glp_prob *lp;
  int ia[1+1000], ja[1+1000];
  double ar[1+1000], z, x1, x2, x3;
  
  lp = glp_create_prob();
  glp_set_prob_name(lp, "short");
  glp_set_obj_dir(lp, GLP_MAX);

  const double epsilon = 1./10;
  glp_add_rows(lp, 3);
  glp_set_row_name(lp, 1, "e1");
  glp_set_row_bnds(lp, 1, GLP_LO, 5-1+epsilon, 0.0);
  glp_set_row_name(lp, 2, "e2");
  glp_set_row_bnds(lp, 2, GLP_LO, 6-1+epsilon, 0.0);
   glp_set_row_name(lp, 3, "e3");
  glp_set_row_bnds(lp, 3, GLP_LO, 3-3+epsilon, 0.0);

	int col_count = 1;
  glp_add_cols(lp, 2);
  glp_set_col_name(lp, col_count, "x1");//1
  glp_set_col_bnds(lp, col_count, GLP_LO, 0.0, 0.0);
  glp_set_obj_coef(lp, col_count, 1);
  col_count++;
  glp_set_col_name(lp, col_count, "t1");//2
  glp_set_col_bnds(lp, col_count, GLP_FR, 0.0, 0.0);
  glp_set_obj_coef(lp, col_count, 0);
  col_count++;

  glp_add_cols(lp, 2);
  glp_set_col_name(lp, col_count, "x2");//3
  glp_set_col_bnds(lp, col_count, GLP_LO, 0.0, 0.0);
  glp_set_obj_coef(lp, col_count, 1);
  col_count++;
  glp_set_col_name(lp, col_count, "t2");//4
  glp_set_col_bnds(lp, col_count, GLP_FR, 0.0, 0.0);
  glp_set_obj_coef(lp, col_count, 0);
  col_count++;

  glp_add_cols(lp, 2);
  glp_set_col_name(lp, col_count, "x3");//5
  glp_set_col_bnds(lp, col_count, GLP_LO, 0.0, 0.0);
  glp_set_obj_coef(lp, col_count, 1);
  col_count++;
  glp_set_col_name(lp, col_count, "t3");//6
  glp_set_col_bnds(lp, col_count, GLP_FR, 0.0, 0.0);
  glp_set_obj_coef(lp, col_count, 0);


  unsigned int k=1;

int input, output;
  input=1; output=2;
  ia[k] = 1, ja[k] = 2*output-1+1, ar[k] = 1.0;k++;
  ia[k] = 1, ja[k] = 2*input-1+1, ar[k] = -1.0; k++;
  ia[k] = 1, ja[k] = 2*input-1, ar[k] = 1.0;k++; 

  input=2; output=3;
  ia[k] = 2, ja[k] = 2*output-1+1, ar[k] = 1.0;k++; 
  ia[k] = 2, ja[k] = 2*input-1+1, ar[k] = -1.0; k++;
  ia[k] = 2, ja[k] = 2*input-1, ar[k] = 1.0;k++; 

  input=3; output=1;
  ia[k] = 3, ja[k] = 2*output-1+1, ar[k] = 1.0;k++; 
  ia[k] = 3, ja[k] = 2*input-1+1, ar[k] = -1.0; k++;
  ia[k] = 3, ja[k] = 2*input-1, ar[k] = 1.0;k++; 



  glp_load_matrix(lp, k-1, ia, ja, ar);

  glp_simplex(lp, NULL);

  z = glp_get_obj_val(lp);
  input=1;
  x1 = glp_get_col_prim(lp, 2*input-1);
  input=2;
  x2 = glp_get_col_prim(lp, 2*input-1);
  input=3;
  x3 = glp_get_col_prim(lp, 2*input-1);
  printf("z = %g; x1 = %g; x2 = %g; x3=%g\n", z, x1, x2, x3);

  glp_delete_prob(lp);
  glp_free_env();
  return 0;*/


	/*unsigned int size = 5;
	unsigned int Ki = 10;
	unsigned int Ko = 3;

	pDirectedGraph graph = randomGraph(size, Ki, Ko);
	stronglyConnectedGraph(graph, 0);
	
	//displayGraph(graph->links_list, size);

	pPetri petri = petriNormalizedTransformation(graph, 10);
	setInitialMarking(petri);

	//displayPetriNet(petri);
	
	directedGraphFree(graph);
	petriFree(petri);

	return 0;*/


	/*pArray2 p = NULL;
	pArray2 data;
	pArray2 l;
	p = add2(p, uInt_t, uIntCreateNode(1));
	l = p;
	p = add2(p, uInt_t, uIntCreateNode(2));
	p = add2(p, uInt_t, uIntCreateNode(3));
	data = p;
	p = add2(p, uInt_t, uIntCreateNode(4));
	

	displaySimpeNodeArray2(p);
	pArray2 tmp = removeElemArray2(&p, data);
	displaySimpeNodeArray2(l);

	freeNodeArray2(tmp);
	freeArray2(p);*/

	/*pPetriNode node = petriNodeCreate();
	pArray2 p1, p2, p, tmp;
	p = NULL;
	p = add2(p, uInt_t, uIntCreateNode(1));
	p1 = p;
	p = add2(p, uInt_t, uIntCreateNode(2));
	p2 = p;

	petriNodeAddInput(node, p1);
	petriNodeAddInput(node, p2);

	void displayNode(pPetriNode node)
	{
		pArray l;
		l = node->input_links;
		while(l)
		{
			tmp = (pArray2)(l->data);
			printf("{%d} - ", ((pInt)tmp->data)->data);
			l = l->next;
		}
		printf("\n");
	}

	displayNode(node);

	petriNodeRemoveInput(node, p1);

	displayNode(node);

	petriNodeFree(node);

	
	freeArray2(p);*/

	//srand(time(NULL));

/*
	unsigned int nb_pl = 10000;
	unsigned int nb_tr = 15000;
	int i;
	int sum=0;
	int weight1, weight2;

	pPetri net = petriCreate(nb_pl, nb_tr);

	for(i=0; i<nb_pl; i++)
	{
		petriAddPlace(net, i, rand()%50);
	}

	for(i=0; i<nb_tr; i++)
	{
		petriAddTransition(net, i);
	}
	
	for(i=0; i<5000; i++)
	{
		if(rand()%1)
		{
			petriAddlink(net, PETRI_PT_LINK, rand()%nb_pl, rand()%nb_tr, rand()%100);
		}else
		{
			petriAddlink(net, PETRI_TP_LINK, rand()%nb_tr, rand()%nb_pl, rand()%100);
		}
	}

	weight1 = petriGetWeightLink(net, PETRI_TP_LINK, 8, 2);
	weight2 = petriGetWeightLink(net, PETRI_PT_LINK, 8, 4);
	petriRemovePlace(net, 8);


	weight1 = petriGetWeightLink(net, PETRI_TP_LINK, 10, 2);
	weight2 = petriGetWeightLink(net, PETRI_PT_LINK, 10, 4);
	petriRemovePlace(net, 10);

	petriRemoveTransition(net, 20);
	//displayPetriNet(net);
	petriFree(net);
	return 0;*/

	/*
	pDirectedGraph temp = randomGraph(5, 2,  1);//create graph
	stronglyConnectedGraph(temp, 0);
	petriFree(petriNormalizedTransformation(temp, 10));
	directedGraphFree(temp);*/

	pPetri net = petriCreate(3, 3);
	petriAddPlace(net, 0, 0);
	petriAddPlace(net, 1, 0);
	petriAddPlace(net, 2, 0);

	petriAddTransition(net, 0);
	petriAddTransition(net, 1);
	petriAddTransition(net, 2);

	petriAddlink(net, PETRI_PT_LINK, 0, 1, 5);
	petriAddlink(net, PETRI_TP_LINK, 1, 1, 5);

	petriAddlink(net, PETRI_PT_LINK, 1, 2, 6);
	petriAddlink(net, PETRI_TP_LINK, 2, 2, 6);

	petriAddlink(net, PETRI_PT_LINK, 2, 0, 3);
	petriAddlink(net, PETRI_TP_LINK, 0, 0, 3);

	setInitialMarking(net);
	
	
	petriFree(net);
	return 0;

	int sum, weight1, weight2;
	
	weight2 = petriGetWeightLink(net, PETRI_PT_LINK, 0, 4);
	sum+=weight2;

	weight1 = petriGetWeightLink(net, PETRI_TP_LINK, 1, 1);
	weight2 = petriGetWeightLink(net, PETRI_PT_LINK, 1, 4);
	sum+=weight2;
	petriRemovePlace(net, 1);
	petriAddlink(net, PETRI_TP_LINK, 1, 0, weight1);

	weight1 = petriGetWeightLink(net, PETRI_TP_LINK, 2, 2);
	weight2 = petriGetWeightLink(net, PETRI_PT_LINK, 2, 4);
	sum+=weight2;
	petriRemovePlace(net, 2);
	petriAddlink(net, PETRI_TP_LINK, 2, 0, weight1);

	weight1 = petriGetWeightLink(net, PETRI_TP_LINK, 3, 3);
	weight2 = petriGetWeightLink(net, PETRI_PT_LINK, 3, 4);
	sum+=weight2;
	petriRemovePlace(net, 3);
	petriAddlink(net, PETRI_TP_LINK, 3, 0, weight1);

	printf("sum=%d\n", sum);
	//petriRemovePlace()
	//displayPetriNet(net);

	printf("initial marking =%d\n", petriGetInitialMarking(net, 0));
	petriFree(net);


	//test functions
	
	//srand(time(NULL));
	
	/*unsigned int n = 10;
	pArray * tree = buildTree(n, 3);
	stronglyConnectedGraph(tree, n, 1);
	
	pPetri p = petriTransformation(tree, n);
	char *s=NULL;
	displayMatrix(p->trans, p->num_tr, p->num_pl);
	
	petriToString(&s, p);
	
	
	fprintf(stderr, "res=%s\n", s);
	free(s);
	
	freeList(tree, n);
	petriFree(p);*//*
	srand(getpid());
	unsigned int size = 10;
	unsigned int Ki = 4;
	unsigned int Ko = 3;

	pDirectedGraph graph = randomGraph(size, Ki, Ko);
	displayGraph(graph->links_list, size);
	stronglyConnectedGraph(graph, 0);
	printf("\n----------------------------------\n");
	displayGraph(graph->links_list, size);

	directedGraphFree(graph);

	unsigned int * input = weightsComputation(10, 100);
	free(input);
	return 0;
	
	//pArray * graph = randomGraph(size, Ki, Ko);
	//displayGraph(graph, size);
	
	//freeList(graph, size);
	
	pDirectedGraph temp = randomGraph(size, Ki, Ko);//create graph
	stronglyConnectedGraph(temp, 0);
	
	displayGraph(temp->links_list, size);
	printf("\n\n-------------\n\n");
	
	pPetri p = petriTransformation(temp);
	//displayMatrix(p->trans, p->num_tr, p->num_pl);
	
	
	directedGraphFree(temp);
	petriFree(p);
	*/
	return 0;
}


