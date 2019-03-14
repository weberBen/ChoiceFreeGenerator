#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "GraphGenerator.h"


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
    initializeArray(tree, n);

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
    n--;//soustract the root to the number of resting nodes
    
    
    while(n!=0)
    {
		if(num_suc>n)//cannot test if (n-num_suc)<0 because n and num_suc are unsigned int 
		{
			num_suc = n;
		}
		n-=num_suc;

		for(i=0; i<num_suc; i++)
		{
			tree[num_node] = add(tree[num_node], count);
			count++;
		}
		
		num_node++;
		num_suc = rand()%D;//(between 0 and D-1)
	}
	
	return tree;
}


void visiter(unsigned int u)
{
    
}

