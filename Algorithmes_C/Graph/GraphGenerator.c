#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


#include <sys/types.h>
#include <unistd.h>
#include <sys/resource.h>


#include "GraphGenerator.h"

/**********************************************************************
 * 
 * 						GRAPH FUNCTIONS
 * 
 * 
 * *******************************************************************/

 static pDirectedGraph matrixIntoList(int network[], unsigned int n)
 {
	/* For a matrix line that store all the links between nodes of the graph as following
	 *  		| node 1 | node 2 | node 3 | ... | node n
	 * 	node 1  |
	 *  node 2  |
	 *    . 
	 *    .
	 *    .
	 *  node 3  |
	 * 
	 * Line i represents all the outputs of node i and columun i represent all inputs of node i
	 * The coefficient (i,j) is :
	 * 			* 1 if node j is a child of node i
	 * 			* 0 is there is no link between nodes i and j
	 * 
	 * Then the function return a list of list of children for each node
	 */
	pDirectedGraph output = directedGraphCreate(n);
	
	unsigned int i,j,k;
	
	//convert the matrix into a list of sucessors
	for(i=0, k=0; i<n; i++)
	{
		for(j=0; j<n; j++)
		{
			if(network[k]==1)
			{
				directedGraphAddLink(output, i, j);
			}
			k++;
		}
	} 
	
	return output;
 }
 
pDirectedGraph randomGraph(unsigned int n, int Ki,  int Ko)
 {
	 /* Build a random graph with a given number of node. For each node 
	  * Ki represent the number of input node for each node and Ko the number of
	  * output node for each node. The function then construct as much as possible a graph
	  * that follow these constraint of input and ouput for a node
	  * If the number of input or output does not need to be set, then set the value to -1
	 */
	 
	 /*****************************************************************
	  * 					SET PARAMETERS
	  ****************************************************************/
	  
	 //check which constraints need to be followed
	 const int setI = (Ki>0)?1:0;
	 //If the number of output or input node is not set, then he must be at the same value as the other parameters
	 if(!setI)
		Ki = max(Ki, Ko);
	 const int setO = (Ko>0)?1:0;
	 if(!setO)
		Ko = max(Ki, Ko);
		
	const unsigned int sum = (setI)?Ki:0 + (setO)?Ko:0;
	 
	 
	 /*****************************************************************
	  * 				CREATION OF THE NETWORK
	  ****************************************************************/
	  
	/* In order to save all the link between the nodes and be able to
	 * randomly pick one node that has no link with the desired one 
	 * we use a matrix. Line i represents outputs of the node i and columun i
	 * represents inputs of the node i 
	 * matrix[i][j] is 0 if there is no link between i and j, and, 1 if i is
	 * a parent of j (or in other words, if j is a child of i)
	*/
	unsigned int size = n*n;
	int network[size];
	initializeIntArray(network, size, 0);//set all the value to 0 (no links)
	 
	 /*****************************************************************
	  * 				PROBABILITY HANDLER
	  ****************************************************************/
	  
	 /* To randomly pick a node we associate a probability for each node
	  * to be picked as an output and as an input. At the begining, each
	  * node as the same probability to be picked (probability of 1 = Ki/Ki = Ko/Ko)
	  * Then if a node is select as an output for another its probability to be picked
	  * next time decrease ((1-Ki)/Ki). When the probability is 0, then the node does not 
	  * need any ohter node as input or as output or both.
	  * 
	  * Then we have two probabilities array :
	  * 	* one that represents the probability for a node to be picked as
	  * an input for another, that is the output array
	  * 	* one that represents the probability for a node to be picked as
	  * an output for another, that is the input array
	  * 
	  * Indeed, if node (a) become (a) prent of node b ( (a)--->(b) ), then 
	  * probability of node (a) to be picked as an output decrease because it
	  * has for now one output link and for node (b) probability to be picked as 
	  * an input decrease. 
	  * Then when we need to pick a node as child of another node (find an output), we need to see
	  * the probability of the input array : for a node (a) we search a node that can be a child 
	  * of (a). In other words, we search a node that have not enough input.
	  * And to pick a node as parent of another (find an input), we need to see the probability of the output parent
	  * 
	  * 
	  * For example, if Ko=5, in others workds the number of output node per node is 5,
	  * let us say that : 
	  * 		* a+(i) represents number of outputs for node i
	  * 		* p+(i) represents the associate probability for output array
	  * 		* p-(i) represents the associate probability for input array
	  * 
	  * Then :
	  * a+(i)=0 ==> p+(i)=5/5=1
	  * a+(i)=1 ==> p+(i)=4/5
	  * a+(i)=2 ==> p+(i)=3/5
	  * ...
	  * a+(i)=5 ==> p+(i)=0/5=0
	  * 
	  * Then let us say that a+(i)=2 and we connect the node (b), with a-(b)=3, as a child of the node i
	  * So, (i)---->(b)
	  * 	* a+(i)<---3  ===> p+(i)=2/5
	  * 	* a-(b)<---4  ===> p-(b)=1/5
	  * 
	  * Then to randmoly select a node we use cumulative probability :
	  * |-------|----------------|-------|...|---------------|
	  * 0     p+(1)*Ko		(p+(1) + p+(2))*Ko				(p+(1)+...p+(n))*Ko
	  * Then a randomly select a integer between 0 included and (p+(1)+...p+(n))*Ko excluded
	  * Since all the probability are reduced by the same factor Ko, Ki we can set the probability without that factor
	  * 
	  * Moreover, even if for each node we have the probability of input and ouput, for a particular node we don't want
	  * to link itself with itself or to link a node that is already linked with the current node. In other words, if we pick
	  * node (b) as an output of the current node, we don't want to pick the node (b) after for the current node.
	  * Thus, we have to superimpose a mask over the real probability : when we pick a node, then we temporarily set to 0
	  * its probability. Then the node will no longer be picked. 
	 */
	 
	 double proba_o[n];//output array for probability
	 double * temp_proba_o = (double *)malloc(sizeof(double)*n);//temporary mask for the output probability array
	 assert(temp_proba_o);
	 
	 double proba_i[n];//input array for probability
	 double * temp_proba_i = (double *)malloc(sizeof(double)*n);//temporary mask for the input probability array
	 assert(temp_proba_i);
	 
	 //set to 1 all the probability for input and output
	 unsigned int i;
	 for(i=0; i<n; i++)
	 {
		 proba_o[i] = Ko;//represent the probability multiplied by Ko (p=1=Ko/Ko =>Ko*p=Ko)
		 proba_i[i] = Ki;//represent the probability multiplied by Ki (p=1=Ki/Ki =>Ki*p=Ki)
	 }
	
	 /* Since we will use a temporarily mask over the real probability, we set the 
	  * ramdom cumulative probability with that array and not the real probability array
	 */
	 int idRandVertexI = randCumulProbaIni(temp_proba_i, n);
	 int idRandVertexO = randCumulProbaIni(temp_proba_o, n);
	
	 
	 /*****************************************************************
	  * 				CREATION OF THE RELATION
	  ****************************************************************/
	 
	 unsigned int k,l;
	 unsigned int vertex, temp_l, temp_c, cursor_l, cursor_c;
	 int index;
	 int id;
	 
	 for(vertex=0; vertex<n; vertex++)//loop through the vertex of the graph
	 {
		 /* Create the probability mask for the current node*/
		 cursor_c = vertex*n;//line of the current vertex in the network matrix
		 cursor_l = vertex;
		 for(k=0, temp_c=cursor_c, temp_l=cursor_l; k<n; k++, temp_c++, temp_l+=n)
		 {
			 //copy the current probability
			 temp_proba_o[k] = proba_o[k];
			 temp_proba_i[k] = proba_i[k];
				 
			 if(k==vertex)
			 {
				 /* if the current node is the current vertex the we set its probability
				  * to 0 for input and ouput array (we don't want to select the current vertex as an
				  * input or as an output for the current vertex)
				 */
				 temp_proba_o[k] = 0;
				 temp_proba_i[k] = 0;
			 }else if(network[temp_c]==1)//check output of the current vertex
			 {
				 /* If the current node is a child of the current vertex, we set its input probability to 0, 
				  * we don't want to pick that node as a new child of the current vertex*/
				 temp_proba_i[k] = 0;

			 }else if(network[temp_l]==1)//check input of the current vertex
			 {
				 /* If the current node is a parent of the current vertex, we set its output probability to 0, 
				  * we don't want to pick that node as a new parent of the current vertex*/
				 temp_proba_o[k] = 0;
			 }
		 }
		 //update the probability array from begining
		 randCumulProbaUpdate(idRandVertexI, 0);
		 randCumulProbaUpdate(idRandVertexO, 0);
		 
		 
		 //get all the inputs and outputs for the current vertex
		 for(l=0; l<sum; l++)
		 { 
			 /** OUTPUT OF THE CURRENT NODE **/
			 
			 if(proba_o[vertex]!=0 && setO)//if the current vertex need new output and if the user set the output
			 {
				 //get child node (get nodes for which the current node will be it parent) 
				id = idRandVertexI;
				index = randCumulProba(id);
				/* To get a child of the current vertex we look for nodes that can accept new input
				 * we check the input probability
				 * 
				 * At that step, we picked node can not be the current vertex itself, neither a node that is 
				 * already a child of the current vertex
				*/
				
				if(index!=-1)//a node that verify the constraints exists
				{
					//update global probability
					proba_o[vertex]--;
					proba_i[index]--;
						
					//register the new link between the nodes
					network[cursor_c + index] = 1;//set the link as an output of the current node
					temp_proba_i[index] = 0;//temporarily set the probability of the picked node to have new input to 0 (then for the current vertex that node cannot be picked)
					randCumulProbaUpdate(id, index);//update cumulative probability
					
				}
			}
			 
			/** INPUT OF THE CURRENT NODE **/
			
			if(proba_i[vertex]!=0 && setI)//if the current vertex need new input and if the user set the input
			{
				 //get parent node
				id = idRandVertexO;
				index = randCumulProba(id);
				
				//the previous index could no be the current node or a node that is already a parent of the current node because of their nul probability
				if(index!=-1)
				{
					//update global probability
					proba_i[vertex]--;
					proba_o[index]--;
					
					//register the new link between the nodes
					network[index*n + vertex] = 1;//set the link as an output of picked node
					temp_proba_o[index] = 0;
					randCumulProbaUpdate(id, index);
				}
				
			}
			
		}//end loop through the parent and child of a particular node of the graph
		
	 }//end loop through node of the graph
	 
	 
	 
	 /*****************************************************************
	  * 				FREE MEMORY
	  ****************************************************************/
	 //stop the random cumulative probability generator
	 randCumulProbaEnd(idRandVertexO);
	 randCumulProbaEnd(idRandVertexI);
	 
	 return matrixIntoList(network, n);//convert the network matrix into a list of list of children
 }
 
 
 
 
/**********************************************************************
 * 
 * 						TREE FUNCTIONS
 * 
 * 
 * *******************************************************************/
pDirectedGraph buildTree(unsigned int n, unsigned int D)
{
    /* Return a tree of n vertex with a maximun number of child of D
     * The output is a list of child sucessor. Each element i (list[i]) 
     * of the list is the vertex i and all the child of the vertix i are saved
     * as a linked list (list[i] = {id_child_number_1_of_i, id_child_number_2_of_i, ..}
     * 
     * Parameters : 
     * 		n : number of node in the tree
     * 		D : maximum number of child for a node
     * 
     * Output :
     * 	list of linked list that represent the child of each node i saved in the element of index i
     * 	[{...}, {...} , ... ]
    */
    
    
    pDirectedGraph tree = directedGraphCreate(n);

    unsigned int i;
    unsigned int num_suc;
    unsigned int count;
    unsigned int num_node;
    
    
    /* tree = [NULL, NULL, ... ]
     * For each node i (saved as tree[i]), we select a random number of node 
     * and then add these node as childs of the node i
     * 
     * For example if we select 3 nodes for the root, then
     * tree[0] = {1,2,3}
     * Then we select the number of nodes for the node 1, 2, 3 ...
    */
	
    //initialization
    num_node = 0;
    count = num_node + 1;
    /* id of the sucessor od the latest node created
     * For example if tree=[{1,2,3}, {4}, {}, {5,6}]
     * then count will be equal to (6+1) where 6 is the id of the latest created node in the tree
    */
    num_suc = 1 + rand()%D; //number succesor of root (between 1 and D-1)
    
    
    do
    {	
		
		if((count-1) + num_suc>=n)
		{
			/*         		 	(u) <-- node u
			 *           		 |
			 * --------------------------------------------
			 * | (count)  (count+1)  .... (count+num_suc) |  <-child of node u
			 * 						   ^
			 * 						   |
			 * 						(n-1) = maximun possible index for a node
			 * (because n is the total number of node)
			 * 
			 * If the current number of child of the current node u exceed 
			 * to total nomber of node, we need to reduce the number of child 
			 * of the current node (u)
			*/
			num_suc = n-count;//= (n-1)-count+1
		}
		
		if(num_node==count)//here num_node>=1 because we start with num_node=0 and count=1
		{
			/* In fact, when 0 appears too much time as the number of child for a following of nodes,
			 * the the curent node can be the next node to add (which make no sens, because
			 * that node will no be connect to the rest of the tree)
			 * 
			 * For example that condition appears in the following situation
			 * tree[0] = {2 ,1} <-- at the end count = 3
			 * tree[1] = { } <-- 0 child, count = 3
			 * tree[2] = { } <-- 0 child, count = 3
			 * tree[3] = {?} <-- 0...D child, count = 3
			 * 
			 * To solve that problem, we look at the previous node to see which one
			 * has place for that node as child (and there is in all the case one node)
			*/
			
			int i = num_node-1;//check into the previous nodes of the current one
			
			while(lengthArray(tree->links_list[i])>=D)
			{
				i--;
			}
			directedGraphAddLink(tree, i, count);
			
			//update value
			num_suc -= ((num_suc==0)?0:1);//tyhe variable is an unsigned int which cannot be negative
			count++;//increase the newest node to add
		}
		//add node as child of the current nodedd
		for(i=0; i<num_suc; i++)
		{
			directedGraphAddLink(tree, num_node, count);
			count++;
		}
		
		num_node++;
		num_suc = rand()%D;//(between 0 and D-1)
		
	}while(count<n);
	
	
	return tree;
}


/**********************************************************************
 * 
 * 				STRONGLY CONNECTED GRAPH FUNCTIONS
 * 
 * 
 * *******************************************************************/
enum colorTag {WHITE, GRAY};

void connect(unsigned int u, pDirectedGraph graph, enum colorTag color[], int arrival_time[], int path_time[], unsigned int time);
 
void stronglyConnectedGraph(pDirectedGraph graph, int isTree)
{
	unsigned int size = graph->nb_nodes;

	unsigned int i;
	//initialise color array
	enum colorTag color[size];
	for(i=0; i<size; i++)
	{
		color[i] = WHITE;
	}
	//initialize time arrays
	int arrival_time[size];//when set for the first time remain constant
	/* for each node i, the element i of the array represents
	 * the time its take to discovered (to arrive to) this node
	*/
	int path_time[size];//suceptible to change over time
	/* for each node i, the element i of the array represents 
	 * the time its take to find a node j as a child of the node i is directly
	 * connected to the node i (j is a child of i)
	 * For example,
	 * 					(0)
	 * 				(i)-----(j) <-- node i and j not necessarily at the same level
	 * 			  | ? |    | ? |
	 * 				   | ?  (k)  ? | 
	 * 
	 * Then, if we decided that the node k (child of the node j) must be connected to the node i 
	 * (following the order of the tree : node j -> node i), then path_time[j] = arrival_time[i]
	 * From the root, it will take arrival_time[i] unity of time to find the node i, which is connected
	 * (indirectly) to the node j as a child
	 * In other words, that time represents how far the node j is connected to the root
	 * (the size of the value does not imply that the node is more or less connected at the root
	 * because the time we associate to the node when we first meet it depends on how we run
	 * into the tree, but a relative comparaison between the current value and the previous value
	 * will do the work)
	 * 
	*/
	
	for(i=0; i<size; i++)
	{
		arrival_time[i] = -1;
		path_time[i] =-1;
	}
	
	//initialize time origin
	unsigned int time = 1;
	
	if(isTree)//non zero value
	{
		connect(0, graph, color, arrival_time, path_time, time);
		//since all the node are connected to the root, we just have one call as the initialization
	}else
	{
		for(i=0; i<size; i++)
		{
			connect(i, graph, color, arrival_time, path_time, time);
		}
	}
}

void connect(unsigned int u, pDirectedGraph graph, enum colorTag color[], int arrival_time[], 
			 int path_time[], unsigned int time)
{
	//update configuration
	color[u] = GRAY;
	arrival_time[u] = time; time++;//first meet of the node
	path_time[u] = arrival_time[u];
	
	//run into the child of the current node
	unsigned int v;
    pArray cursor = graph->links_list[u];
    
    while(cursor)
    {
		v = uIntValue(cursor);
		if(color[v]==WHITE)
		{
			connect(v, graph, color, arrival_time, path_time, time);
		}	
		
		if(path_time[v]<path_time[u])
		{
			path_time[u] = path_time[v]; //get the minimum value
		}
		
		cursor = cursor->next;
	}
	
	//create a connection if one is needed
	unsigned int start_time, end_time;
	unsigned int start_node, end_node;
	unsigned int i;
	int b1, b2;
	
	
	if(path_time[u]==arrival_time[u])
	{//if there is no child of the current node in the previous nodes (node from the root until the current one)
		
		start_time = getRandomInSegment(arrival_time[u], time-1);//between arrival_time[u] and (time-1)
		/* get time of child of the current node */
		
		end_time = getRandomInSegment(1, arrival_time[u]-1);//between 1 and (arrival_time[u]-1)
		/* get time of a node that will be connected to the corresponding node of start_time
		 * as a child of its.
		 * Then a child of the current node will be connected to a previous one
		*/
		
		
		/* search to find the start_node as arrival_time[start_node] = start_time
		 * and arrival_time[end_node] = node to connect the two nodes
		*/
		
		b1=1; b2=1; //booleans
		i = 0;
		while(b1!=0 || b2!=0)
		{
			if(arrival_time[i]==start_time)
			{
				start_node = i;
				b1 = 0;
			}
			
			if(arrival_time[i]==end_time)
			{
				end_node = i;
				b2 =0;
			}
			
			i++;
		}
		
		if(start_node==end_node)//root
		{ 
			/* can only be connected to itself because 0 is the root, 
			 * then the condition at the start time and the end time 
			 * produces start_time=1=end_time 
			 * (and 1 is the origin of the time which represents the root)
			*/
			return;
		}
		
		//connected the two nodes (start_node to end_node)
		directedGraphAddLink(graph, start_node, end_node);
		path_time[u] = end_time;
	}
}



/**********************************************************************
 * 
 * 						WEIGHTS COMPUTATION
 * 
 * 
 * *******************************************************************/
unsigned int * weightsComputation(unsigned int nb_transition, unsigned int repetition_vect_norm)
{
	/* Fist the function generate an array of n integers, where n is the number of transitions in the Petri net, such as
		the sum of each element of the array is approximativly equal to the nomr of the T-semiflow.
		Then to make sure there is no useless tokens we ensure that the gcm of the array is 1
	*/

	if(repetition_vect_norm<nb_transition)
	{
		fprintf(stderr, "Norm of the vector does not match the constraint of number of transition in the petri net" \
						"Number of transitions : %u   | Vector nomr : %u\n", nb_transition, repetition_vect_norm);
		
		return NULL;
	}

	//generate array of random integers with a fixed sum
	unsigned int * random_weights = randomFixedSum(nb_transition, repetition_vect_norm);

	//ensure gcm of the array is 1
	int gcd_val = gcd_array(random_weights, nb_transition);//get gcm of the array
	if(gcd_val<=0)
	{
		fprintf(stderr, "Error during the computation of the gcd for the repetition vector\n");
		exit(EXIT_FAILURE);
	}

	if(gcd_val!=1)
	{
		unsigned int temp[3];
		temp[0]=gcd_val;
		//could had been all different index in the array (not exclusively 0 and 1)
		temp[1]=random_weights[0];
		temp[2]=random_weights[1];

		while(gcd_array(temp, 3)!=1)
		{
			temp[1]--;
			/* The value cannot be inferior to 1, because when finally if it reach 1, then the gcm will be 1 */
			temp[2]++;//keep intact the fixed sum
		}

		random_weights[0]=temp[1];
		random_weights[1]=temp[2];
	}

	return random_weights;
}


void normalizationPetriNetwork(pPetri net, unsigned int * repetition_vect)
{
	int lcm_val = lcm_array(repetition_vect, net->nb_tr);
	if(lcm_val==-1)
	{
		fprintf(stderr, "Cannot generate a correct repetition vector (check the given arguments)\n");
		return ;
	}
	
	int i;
	pPetriNode node;
	pPetriLink link;
	pArray pi, po;
	int normalized_value;

	for(i=0; i<net->nb_tr; i++)
	{
		node = net->transitions[i];
		if(node==NULL)
			continue;
		
		normalized_value = lcm_val/repetition_vect[i];
		
		pi = node->input_links;
		po = node->output_links;
		while(pi || po)
		{
			if(pi)
			{
				link = petriNodeGetLinkFromArrayNode(pi);
				link->weight = normalized_value;
				pi = pi->next;
			}
			if(po)
			{
				link = petriNodeGetLinkFromArrayNode(po);
				link->weight = normalized_value;
				po = po->next;
			}
		}
	}
}

/**********************************************************************
 * 
 * 						PETRI FUNCTIONS
 * 
 * 
 * *******************************************************************/

pPetri _petriTransformation(pDirectedGraph graph, int normalize, unsigned int repetition_vect_norm)
{
	/* edge <-> place and node <-> transition */
	pPetri net = petriCreate(graph->nb_edges, graph->nb_nodes);

	int lcm_val = -1;
	unsigned int * repetition_vect = NULL;
	if(normalize)
	{
		repetition_vect =  weightsComputation(net->nb_tr, repetition_vect_norm);
		lcm_val = lcm_array(repetition_vect, net->nb_tr);
		if(lcm_val==-1)
		{
			fprintf(stderr, "Cannot generate a correct repetition vector (check the given arguments)\n");
			petriFree(net);
			return NULL;
		}
	}

	//add nodes and transition
	int i;
	int limit_inf = min(net->nb_pl, net->nb_tr);
	int limit_sup = max(net->nb_pl, net->nb_tr);

	for(i=0; i<limit_inf; i++)
	{
		petriAddPlace(net, i, 0);
		petriAddTransition(net, i);
	}

	for(i=limit_inf; i<limit_sup; i++)
	{
		if(i<net->nb_pl)
			petriAddPlace(net, i, 0);
		if(i<net->nb_tr)
			petriAddTransition(net, i);
	}

	//add links
	pArray p;
	int nb_link;
	unsigned int id_tr;
	int normalized_value_i, normalized_value_o;

	nb_link = 0;
	for(i=0; i<graph->nb_nodes; i++)
	{	
		normalized_value_i = (normalize)?(lcm_val/repetition_vect[i]):0;

		p = graph->links_list[i];
		while(p)
		{	
			//i : index of the current transition
			//nb_link : index of the current place

			//add link between the current transition and a "random" place
			petriAddlink(net, PETRI_TP_LINK, i, nb_link, normalized_value_i);
			//add link the place and the output of the current node in the graph (which is the destination transition for the link)
			id_tr = uIntValue(p);//get id of the transition which is an output of the current node of the graph
			normalized_value_o = (normalize)?(lcm_val/repetition_vect[id_tr]):0;
			petriAddlink(net, PETRI_PT_LINK, nb_link, id_tr, normalized_value_o);

			p = p ->next;
			nb_link++;
		}
	}
	
	if(repetition_vect!=NULL)
		free(repetition_vect);

	return net;
}

pPetri petriTransformation(pDirectedGraph graph)
{
	return _petriTransformation(graph, 0, 0);
}

pPetri petriNormalizedTransformation(pDirectedGraph graph, unsigned int repetition_vect_norm)
{
	return _petriTransformation(graph, 1, repetition_vect_norm);
}

/**********************************************************************
 * 
 * 						INITIAL MARKING COMPUTATION
 * 
 * 
 * *******************************************************************/

void setInitialMarking(pPetri net)
{
	glp_prob *lp;
	int ia[GLPK_SIZE], ja[GLPK_SIZE];
	double ar[GLPK_SIZE];
	const double epsilon = 1./(net->nb_pl);

	//create variables
	char id[128];
	int i, k;
	int row_count, col_count;
	int pos_xi, pos_ti, pos_to;
	pPetriNode node;
	pPetriLink i_link, o_link;
	double bound;

	// create problem 
	lp = glp_create_prob();
	glp_set_prob_name(lp, "short");
	glp_set_obj_dir(lp, GLP_MIN);
	// fill problem 
	glp_add_cols(lp, 2*net->nb_pl);


	k = 1;
	row_count = 1;
	col_count = 1;
	for(i=0; i<net->nb_pl; i++)
	{
		node = net->places[i];
		if(node==NULL)
			continue;
		if(node->nb_inputs!=1 || node->nb_outputs!=1)
		{
			fprintf(stderr, "Error during the transformation from SDF to Free-choice (SDF format incorect)\n");
			return;
		}
		/*
			Add the following euquation :
				transition(ti) -------> place(pi) -------> transition(to)
								  Zi    M0(pi)=xi	 Zk
			
			to - tj + xi >= Zo - gcd(Zo, Zi) + epsilon
	    */

		//set equation row
		sprintf(id, "e%d", row_count);
		glp_add_rows(lp, 1);
		glp_set_row_name(lp, row_count, id);

		//set bounds equation
		i_link = petriNodeGetLinkFromArrayNode(node->input_links);
		o_link = petriNodeGetLinkFromArrayNode(node->output_links);

		bound = o_link->weight - gcd(i_link->weight, o_link->weight) + epsilon;//Zo - gcd(Zo, Zi) + epsilon
		glp_set_row_bnds(lp, row_count, GLP_LO, bound, 0.0);

		//set variables

		//add to column xi | ti (for i starting at 1)
		//to get the column index of a place i : 2*i-1
		//to get the column index of a transition i : (2*i-1)+1=2*i
		sprintf(id, "x%d", col_count);
		pos_xi = 2*(o_link->input->label+1)-1;
		glp_set_col_name(lp, pos_xi, id);
		glp_set_col_bnds(lp, pos_xi, GLP_LO, 0.0, 0.0);
		glp_set_obj_coef(lp, pos_xi, 1);

		sprintf(id, "t%d", col_count);
		pos_to = 2*(o_link->output->label+1);
		glp_set_col_name(lp, pos_to, id);
		glp_set_col_bnds(lp, pos_to, GLP_FR, 0.0, 0.0);
		glp_set_obj_coef(lp, pos_to, 0);

		pos_ti = 2*(i_link->input->label+1);//+1 because label start to 0

		//set other coefficient
		ia[k] = row_count, ja[k] = pos_ti, ar[k] = -1.0; k++; // a[row_count, pos_ti] = -1
		ia[k] = row_count, ja[k] = pos_to, ar[k] = 1.0; k++; // a[row_count, pos_to] = 1
		ia[k] = row_count, ja[k] = pos_xi, ar[k] = 1.0; k++; // a[row_count, pos_xi] = 1

		row_count++;
	}

	
	glp_load_matrix(lp, k-1, ia, ja, ar);
	// solve problem 
	if(glp_simplex(lp, NULL)!=0)//fail to solve
	{
		fprintf(stderr, "Fail to solve the initial marking problem\n");
		// housekeeping
		glp_delete_prob(lp);
		glp_free_env();

		return;
	}

	// recover and display results
	if(row_count>=1)
	{
		double xi;
		int gcd_val;
		int m0;

		col_count = 1;
		for(i=0; i<net->nb_pl; i++)
		{
			if(net->places[i]==NULL)
				continue;

			node = net->places[i];
			i_link = petriNodeGetLinkFromArrayNode(node->input_links);
			o_link = petriNodeGetLinkFromArrayNode(node->output_links);
			gcd_val = gcd(i_link->weight, o_link->weight);//gcd(to, ti)

			xi = glp_get_col_prim(lp, 2*col_count-1);
			m0 = ceil(xi/gcd_val)*gcd_val;//initial marking

			i_link->output->val = m0;


			col_count++;
		}
	}else
	{
		fprintf(stderr, "No variable founded for the initial marking computation\n");
	}
	// housekeeping
	glp_delete_prob(lp);
	glp_free_env();
}




 /**********************************************************************
 * 
 * 						FREE CHOICE
 * 
 * 
 * *******************************************************************/

 void sdfToFreeChoice(pPetri net, int resizeNetAfter)
 {
	 int i;
	 int i_weight;
	 int m0;
	 pPetriNode node, input_node_pl;
	 pPetriLink link_pt, link_tr;
	 pPetriElem input_pl, init_input_pl;
	 unsigned int id_tr;
	 pArray p, cursor;

	 for(i=0; i<net->nb_tr; i++)
	 {
		node = net->transitions[i];
		if(node==NULL)
			continue; 
		if(node->nb_inputs<=1)
			continue;
		

		p = node->input_links;

		//manage the first place
		link_pt = petriNodeGetLinkFromArrayNode(p);
		init_input_pl = link_pt->input;
		if(init_input_pl->type!=PETRI_PLACE_TYPE)
		{
			fprintf(stderr, "The petri net does not follow the standards (link Transition-Transition founded)\n");
			return ;
		}


		link_pt->weight = (node->nb_inputs)*(link_pt->weight);//set weight of the place-transition link
		m0 = init_input_pl->val;//initialize initial marking

		p = p->next;

		//apply the transformation on the other nodes
		while(p)
		{
			/* Because at the end of the loop the element referenced by the pointer on array, p, will be removed
			   we could not check the following element in the array based on that reference (because it will no longer exist).
			   Thus we save the reference to the next element before the removal of the current one
			 */
			cursor = p->next;
			
			link_pt = petriNodeGetLinkFromArrayNode(p);
			input_pl = link_pt->input;
			if(input_pl->type!=PETRI_PLACE_TYPE)
			{
				fprintf(stderr, "The petri net does not follow the standards (link Transition-Transition founded)\n");
				return ;
			}

			m0 += input_pl->val;//get initial marking of the input node
			
			//get weight of the links
			input_node_pl = net->places[input_pl->label];//get input node from the current transition
			if(input_node_pl->nb_inputs!=1)
			{
				fprintf(stderr, "The petri net is not correct for the transformation from SDF to Free-Choice\n");
				return ;
			}

			link_tr = petriNodeGetLinkFromArrayNode(input_node_pl->input_links);//get the only transition from the input node
			id_tr = link_tr->input->label;
			i_weight = link_tr->weight;

			petriRemovePlace(net, input_pl->label);
			//after the removal the link betwwen the input place and its transition does not exists anymore (link_tr does reference nothing)
			petriAddlink(net, PETRI_TP_LINK, id_tr, init_input_pl->label, i_weight);

			
			p = cursor;
		}

		init_input_pl->val = m0;//set final initial marking
	 }

	 if(resizeNetAfter)
	 	petriClearPlaces(net);//optionnal (can be remove to optimize computation time)
 }



pPetri _generateFreeChoice(unsigned int nb_transition, unsigned int nb_input_node, unsigned int nb_output_node, unsigned int repetition_vect_norm, unsigned int * repetition_vect, int cleanExtraMemSpace)
{
	if(nb_transition<2)
	{
		fprintf(stderr, "The number of transitions must be superior or equal to 2\n");
		return NULL;
	}
	if(nb_input_node>nb_output_node)
	{
		fprintf(stderr, "The number of input per node (%u) cannot be greater than the one " \
						"for the output (%u) to generate a Free-choice\n", nb_input_node, nb_output_node);
		return NULL;
	}

	printf("Generation of a random oriented graph...\n");
	pDirectedGraph graph = randomGraph(nb_transition, nb_input_node, nb_output_node);
	
	printf("Conversion to strongly connected graph...\n");
	stronglyConnectedGraph(graph, 0);

	printf("Conversion to SDF...\n");
	pPetri net;
	if(repetition_vect==NULL)
	{
		net = petriNormalizedTransformation(graph, repetition_vect_norm);
	}else
	{
		net = petriTransformation(graph);
		normalizationPetriNetwork(net, repetition_vect);
	}
	directedGraphFree(graph);

	printf("Computation of the initial marking...\n");
	setInitialMarking(net);

	printf("Conversion to Free-choice...\n");
	sdfToFreeChoice(net, cleanExtraMemSpace);

	return net;
}


pPetri generateRandomFreeChoice(unsigned int nb_transition, unsigned int nb_input_node, unsigned int nb_output_node, unsigned int repetition_vect_norm, int cleanExtraMemSpace)
{
	if(repetition_vect_norm<nb_transition)
	{
		fprintf(stderr, "Norm of the vector does not match the constraint of number of transition in the petri net" \
						"Number of transitions : %u   | Vector nomr : %u\n", nb_transition, repetition_vect_norm);
		return NULL;
	}
	return _generateFreeChoice(nb_transition, nb_input_node, nb_output_node, repetition_vect_norm, NULL, cleanExtraMemSpace);
}


pPetri generateFreeChoiceWithVector(unsigned int nb_transition, unsigned int nb_input_node, unsigned int nb_output_node, unsigned int * repetition_vect, int cleanExtraMemSpace)
{
	if(repetition_vect==NULL)
	{
		fprintf(stderr, "cannot generate a normalized SDF from a NULL repetition vector\n");
		return NULL;
	}
	return _generateFreeChoice(nb_transition, nb_input_node, nb_output_node, 0, repetition_vect, cleanExtraMemSpace);
}