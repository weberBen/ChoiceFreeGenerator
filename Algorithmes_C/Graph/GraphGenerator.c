#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "GraphGenerator.h"

/**********************************************************************
 * 
 * 						GRAPH FUNCTIONS
 * 
 * 
 * *******************************************************************/

 unsigned int connectTree(int network[], unsigned int n, unsigned int root, unsigned int node)
 {
	if(node==root || network[node*n + root]==-1)
	{
		return -1;
	}
	
	unsigned int i;
	
	i=0;
	while(i<n && network[node*n+i]!=-1)
	{
		i++;
	}
	
	if(i==n)
	{
		network[node*n+root] = -1;
		network[root*n+node] = 1;
		
		return node;
	}
	
	return connectTree(network, n, root, i);
 }
 
 
 pArray * graphIntoTree(int network[], unsigned int n)
 {
	unsigned int root = 0; //root of the tree
	
	//convert the matrix into a list of sucessors
	pArray * output = (pArray *)malloc(sizeof(array)*n);
	assert(output);
	initializeList((void *)output, n);
	
	unsigned int i,j,k;
	unsigned int node;
	
	//convert the matrix into a list of sucessors
	for(i=0, k=0; i<n; i++)
	{
		if( (node=connectTree(network, n, root, i ))!=-1 )
		{
			output[root] = add(output[root], uInt_t, uIntCreateNode(node));
		}
		
		for(j=0; j<n; j++)
		{
			if(network[k]==1)
			{
				output[i] = add(output[i], uInt_t, uIntCreateNode(j));
			}
			
			k++;
		}
	} 
	 
	return output;
 }
 
 pArray * matrixIntoList(int network[], unsigned int n)
 {
	//convert the matrix into a list of sucessors
	pArray * output = (pArray *)malloc(sizeof(array)*n);
	assert(output);
	initializeList((void *)output, n);
	
	unsigned int i,j,k;
	
	//convert the matrix into a list of sucessors
	for(i=0, k=0; i<n; i++)
	{
		for(j=0; j<n; j++)
		{
			if(network[k]==1)
			{
				output[i] = add(output[i], uInt_t, uIntCreateNode(j));
			}
			
			k++;
		}
	} 
	
	return output;
 }
 
 
 pArray * randomGraph(unsigned int n, unsigned int D)
 {
	unsigned int size = n*n;
	
	/* Temporarily all the links between nodes will be saved into a matrix
	 *  		| node 1 | node 2 | node 3 | ... | node n
	 * 	node 1  |
	 *  node 2  |
	 *    . 
	 *    .
	 *    .
	 *  node 3  |
	 * 
	 * The line i represents all the links between the node i and the other nodes
	 * The coefficient (i,j) is :
	 * 			* 1 if the node j is a sucessor of the node i
	 * 			* -1 if the node i is a sucessor of the node j
	 * 			* 0 is there is no link between the node i and j
	 */
	int network[size];
	initializeIntArray(network, size, 0);
	
	unsigned int i, j;
	unsigned int p_node, node;
	unsigned int num_suc;
	
	for(i=0; i<n; i++)
	{
		p_node = i;
		
		num_suc = rand()%D;//(between 0 and D-1)
		unsigned int r1 = randIni(0, n-1);
		for(j=0; j<num_suc; j++)
		{
			//find a new node to add
			do
			{
				node = randArray(r1);
				
			}while( (node!=-1) && (node==p_node || network[p_node*n+node]!=0));//while we pick the current node as a child or we pick a node that is already a sucessors
			
			//add node as child of the current
			if(node!=-1)
			{
				network[p_node*n + node] = 1;
				network[node*n + p_node] = -1;
			}
			
		}
		randEnd(r1);
	}
	
	
	
	//add links between some nodes to insure the existance of node with muliple entry
	unsigned int num_pred;
	unsigned int count;
	
	//Set the number of node to apply the transformation
	unsigned int num_t; //number of node to apply the transformation
	unsigned int factor = 2;// (n/factor) where n is the number of node
	if(n/factor==0)
	{
		num_t = 1;
	}else
	{
		num_t = (n/factor)  + rand()%(n-n/factor) ;
		//if the factor is a then the number is [n/a, (n-1) - n/a + 1]=[n/2, n-n/a]}
	}
	
	
	unsigned int r1 = randIni(0, n-1);
	for(i=0; i<num_t; i++)
	{
		p_node = randArray(r1);
		
		//get the number of predecessors of the current node
		count =0;
		for(j=0; j<n; j++)
		{
			count+= (network[p_node*n+j]==-1)?1:0;
		}
		
		//add new predecessors if necessary
		num_pred = rand()%(n-count);
		unsigned int r2 = randIni(0, n-1);
		for(j=0; j<num_pred; j++)
		{
			//find a new node to add
			do
			{
				node = randArray(r2);
				
			}while( (node!=-1) && (node==p_node || network[p_node*n+node]==-1));
			
			
			//add node as child of the current
			if(node!=-1)
			{
				network[p_node*n + node] = -1;
				network[node*n + p_node] = 1;
			}
		}
		randEnd(r2);
	}
	randEnd(r1);
	
	return matrixIntoList(network, n);
 }
 
 
 
 
/**********************************************************************
 * 
 * 						TREE FUNCTIONS
 * 
 * 
 * *******************************************************************/
pArray * buildTree(unsigned int n, unsigned int D)
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
    
    
    pArray * tree = (pArray *)malloc(sizeof(array)*n);
    assert(tree);
    initializeList((void *)tree, n);

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
			
			while(lengthArray(tree[i])>=D)
			{
				i--;
			}
			tree[i] = add(tree[i], uInt_t, uIntCreateNode(count));
			
			//update value
			num_suc -= ((num_suc==0)?0:1);//tyhe variable is an unsigned int which cannot be negative
			count++;//increase the newest node to add
		}
		//add node as child of the current nodedd
		for(i=0; i<num_suc; i++)
		{
			tree[num_node] = add(tree[num_node], uInt_t, uIntCreateNode(count));
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

void connect(unsigned int u, pArray * tree, enum colorTag color[], int arrival_time[], int path_time[], unsigned int time);
 
void stronglyConnectedGraph(pArray * tree, unsigned int n, int isTree)
{
	unsigned int i;
	//initialise color array
	enum colorTag color[n];
	for(i=0; i<n; i++)
	{
		color[i] = WHITE;
	}
	//initialize time arrays
	int arrival_time[n];//when set for the first time remain constant
	/* for each node i, the element i of the array represents
	 * the time its take to discovered (to arrive to) this node
	*/
	int path_time[n];//suceptible to change over time
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
	
	for(i=0; i<n; i++)
	{
		arrival_time[i] = -1;
		path_time[i] =-1;
	}
	
	//initialize time origin
	unsigned int time = 1;
	
	if(isTree)//non zero value
	{
		connect(0, tree, color, arrival_time, path_time, time);
		//since all the node are connected to the root, we just have one call as the initialization
	}else
	{
		for(i=0; i<n; i++)
		{
			connect(i, tree, color, arrival_time, path_time, time);
		}
	}
}

void connect(unsigned int u, pArray * tree, enum colorTag color[], int arrival_time[], 
			 int path_time[], unsigned int time)
{
	//update configuration
	color[u] = GRAY;
	arrival_time[u] = time; time++;//first meet of the node
	path_time[u] = arrival_time[u];
	
	//run into the child of the current node
	unsigned int v;
    pArray cursor = tree[u];
    
    while(cursor)
    {
		v = uIntValue(cursor);
		if(color[v]==WHITE)
		{
			connect(v, tree, color, arrival_time, path_time, time);
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
		tree[start_node] = add(tree[start_node], uInt_t, uIntCreateNode(end_node));
		path_time[u] = end_time;
	}
}


/**********************************************************************
 * 
 * 						PETRI FUNCTIONS
 * 
 * 
 * *******************************************************************/
/* Transformation d'un graphe fortement connexe à un réseau de Pétri
 * On parcourt le graphe. À chaque sommet correspond une place et à chaque arc une transition.
 * Pour chaque sommet, on crée une ligne de la matrice place transition avec +W(ou-) comme coefficient à la colonne du numero de la place vers laquelle on va.
 * Si il y a déjà une transition vers cette place, on rajoute -W sur l autre colonne.
 */
 /*
int estdanstab(int u, int[] tab, int taille){
  for(int i=0; i<taille; i++){
    if(tab[i]==u)
      return 1; //True
  }
  return 0; //False
}
void transformation(pArray * graph, int nbrsommet, int * mat[][]){
  mat=malloc(sizeof(int*));
  int nbr=0;
  int places[]; //Tableaux des places ayant déjà au moins une entrée
  places=malloc(sizeof(int));
  mat[nbr]=malloc(nbrsommet*sizeof(int));
  for(int i=0; i<nbrsommet; i++){
    mat[nbr][i]=0;
  }
  mat[nbr][...]=+ou-1; //Pour l instant je met le poids à 1
  places[nbr]=...
  //On passe au prochain sommet
  if(!estdanstab(...,places, nbr)){
  nbr++;
  mat=realloc((nbr)*sizeof(int*));
  places=realloc((nbr)*sizeof(int));
}
*/

/*
pPetri transformation(pArray * graph, int size){
  pPetri graphpetri=malloc(sizeof(petri));
  graphpetri->num_pl=size;
  graphpetri->places=malloc(size*sizeof(int));
  int i, j;
  for(i=0; i<size; i++){
    graphpetri->places[i]=0; //Pour l'instant rien sur les places
  }
  graphpetri->num_tr=size;
  graphpetri->trans=malloc(size*sizeof(int*));
  for(i=0; i<size; i++){
    graphpetri->trans[i]=malloc(size*sizeof(int));
  }
  for(i=0; i<size; i++){
    for(j=0; j<size; j++){
      graphpetri->trans[i][j]=0;
    }
  }
  for(i=0; i<size; i++){
    graphpetri->trans[i][i]=1; //poids egal a 1 pour l instant
    pArray graphi=graph[i];
    while(graphi!=NULL){
      graphpetri->trans[uIntValue(graphi)][i]=-1; 
      graphi=graphi->next;
    }
  }
  return graphpetri;
}
* */

pPetri petriTransformation(pArray *graph, unsigned int size)
{
	const unsigned int num_pl = 6;
	const unsigned int num_tr = 4;
	
	int * places = (int *)malloc(sizeof(int)*num_pl);
	assert(places);
	initializeIntArray(places, num_pl, 0);
	
	int  temp[24] = {1,-1,-1,0,0,0,
					  0,-1,1,0,0,0,
					  0,0,-1,0,0,1,
					  0,1,0,1,-1,-1};
	
	int * trans = (int *)malloc(sizeof(int)*24);
	assert(trans);
	unsigned int i;
	for(i=0; i<24; i++)
	{
		trans[i] = temp[i];
	}
						
	return petriCreateNode(places, num_pl, trans, num_tr);
}
