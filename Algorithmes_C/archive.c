int startPyFunctions(int argc, char ** argv)
{
	int opt; 
    unsigned int function = -1;
    
    while((opt = getopt(argc, argv, "f:")) != -1)  //-f requeries paramters so f:
    {  
        switch(opt)  
        {  
            case 'f': 
            {
				char names[][50] ={"py_createTree", "py_createStronglyConnectedGraph", ""};
				unsigned int i;
				
				i=0;
				while(names[i][0]!='\0' && strcmp(optarg,names[i]))
				{
					i++;
				}
				
				if(names[0][0]=='\0')
				{
					fprintf(stderr, "Fonction <<%s>> introuvable !", optarg);
					return -1;
				}
				function = i;
			} 
                break;  
            case ':':  
                printf("option needs a value\n");  
                break;  
            case '?':  
                printf("option %c inconnu\n", optopt); //optarg
                break;  
        }  
    }  
      
      
    if(function==-1)
    {
		return -1;
	}
	
	switch(function)
	{
		case 0:
		{
			if(argc-optind<3)
			{
				printf("Nombre d'arguments fourni incorrects !\n");
				return -1;
			}
			py_createTree(atoi(argv[optind]), atoi(argv[optind+1]), argv[optind+2]);
		}
			break;
		case 1:
		{
			if(argc-optind<3)
			{
				printf("Nombre d'arguments fourni incorrects !\n");
				return -1;
			}
			py_createStronglyConnectedGraph(atoi(argv[optind]), atoi(argv[optind+1]), argv[optind+2]);
		}
			break;
		
	}
    /*
    // optind is for the extra arguments 
    // which are not parsed 
    for(; optind < argc; optind++)
    {      
        printf("extra arguments: %s\n", argv[optind]);  
    }*/ 
    
    return 0;
}


void py_createTree(unsigned int n, unsigned int D, char * filename)
{
	pArray * tree = buildTree(n, D);
	listToFile(filename, tree, n);
	freeList(tree, n);
}

void py_createStronglyConnectedGraph(unsigned int n, unsigned int D, char * filename)
{
	pArray * tree = buildTree(n, D);
	stronglyConnectedGraph(tree, n);
	listToFile(filename, tree, n);
	freeList(tree, n);
}

void py_createStronglyConnectedGraphRandom(unsigned int n, unsigned int D, char * filename)
{
	srand(time(NULL));
	py_createStronglyConnectedGraph(n, D, filename);
}



-----------------------------------------------------------------------


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




----------------------------------------------------------------------------------


/**********************************************************************
 * 
 * 					RANDOM FOR VERTEX IN A GRAPH
 * 
 **********************************************************************/

static pArray _listVertex = NULL;



typedef struct VertexDistributionHandler * pVertexH;
typedef struct VertexDistributionHandler//wrapper
{
	int idRandVertexE;
	int idRandVertexO;
	unsigned int NumberNodeE;
	unsigned int NumberNodeO;
} vertexH;


pVertexH createVertexHandlerNode(int idE, unsigned int Ke,  int idO, unsigned int Ko)
{
	pVertexH node = (pVertexH)malloc(sizeof(vertexH));
	assert(node);
	
	node->idRandVertexE = idE;
	node->idRandVertexO = idO;
	node->NumberNodeO = Ko;
	node->NumberNodeE = Ke;
	
	return node;
}

void freeVertexHandlerNode(void * p)
{
	if(p==NULL)
		return;

	pVertexH q = (pVertexH)p;
	
	randCumulProbaEnd(q->idRandVertexE);
	randCumulProbaEnd(q->idRandVertexO);
	free(q);
}

int randVertexIni(unsigned int numberNode, unsigned int NumberNodeE, unsigned int NumberNodeO)
{
	static unsigned int id = 0;
	unsigned int tempId = id;
	id++;
	
	
	double * proba_o = (double *)malloc(sizeof(double)*numberNode);
	assert(proba_o);
	double * proba_e = (double *)malloc(sizeof(double)*numberNode);
	assert(proba_e);
	 
	unsigned int i;
	for(i=0; i<numberNode; i++)
	{
		proba_o[i] = NumberNodeO;
		proba_e[i] = NumberNodeE;
	}
	
	int idRandVertexE = randCumulProbaIni(proba_o, numberNode);
	int idRandVertexO = randCumulProbaIni(proba_e, numberNode);
	
	//set the wrapper
	pVertexH node = createVertexHandlerNode(idRandVertexE, NumberNodeE, idRandVertexO, NumberNodeO);
	wrapperAddToList(&_listVertex, wrapperCreateCustomNode(tempId, freeVertexHandlerNode, (void *)node));
	
	return tempId;
}


void randVertexEnd(unsigned int id)
{
	wrapperRemoveFromList(&_listVertex, id);
}

void tempf(pProbaH pProbaO, pProbaH pProbaE)
{
	fprintf(stderr, "probaO : \n");
	fprintf(stderr, "\t");
	displayArray(pProbaO->proba, pProbaO->size);
	fprintf(stderr, "\t");
	displayArray(pProbaO->cumulProba, pProbaO->size);
	fprintf(stderr, "probaE : \n");
	fprintf(stderr, "\t");
	displayArray(pProbaE->proba, pProbaE->size);
	fprintf(stderr, "\t");
	displayArray(pProbaE->cumulProba, pProbaE->size);
	
}

int * randVertex(unsigned int id, unsigned int current_vertex)
{
	//network represent the line of the current vertex (1 for a child, -1 for a parent, 0 for no link)
	pWrapper p = wrapperGetElem(_listVertex, id);
	if(p==NULL || p->data==NULL)
		return NULL;
	
	pVertexH q = (pVertexH)(p->data);
	
	pProbaH pProbaO = getCumulProbaHandler(q->idRandVertexO);
	if(pProbaO==NULL)
		return NULL;
	pProbaH pProbaE = getCumulProbaHandler(q->idRandVertexE);
	if(pProbaE==NULL)
		return NULL;
	
	
	double rand_val;
	int indexE, indexO, index;
	
	//fprintf(stderr, "--------------------- ID=%u, VERTEX=%u ---------------------\n", id, current_vertex);
	//tempf(pProbaO, pProbaE);
	
	
	//input
	indexE = -1;
	if(pProbaE->proba[current_vertex]!=0)//probabilite que le sommet soit une entrée pour quelqu'un d'autre (est ce qu'il lui reste de la place pour un fils)
	{
		do 
		{
			rand_val = ((double)rand())/((double)RAND_MAX)*(pProbaO->cumulProba[pProbaO->size-1]);
			index = getIndexCumulativeProba(pProbaO->cumulProba, pProbaO->size, rand_val);
			
		}while(index==current_vertex && index!=-1);
		indexE = index;
		//fprintf(stderr, "\nindexE=%u\n", indexE);
		if(indexE!=-1)
		{
			pProbaE->proba[current_vertex]--;
			if(pProbaE->proba[current_vertex]<0)
				pProbaE->proba[current_vertex] = 0;
			randCumulProbaUpdate(q->idRandVertexE, current_vertex);
			
			pProbaO->proba[indexE]--;
			if(pProbaO->proba[indexE]<0)
				pProbaO->proba[indexE] = 0;
			randCumulProbaUpdate(q->idRandVertexO, indexE);
		}
	}
	
	//output
	indexO = -1;
	if(pProbaO->proba[current_vertex]!=0)//probabilite que le sommet soit une sortie pour pour quelqu'un d'autre (est ce qu'il lui reste de la place pour un parent)
	{
		do 
		{
			rand_val = ((double)rand())/((double)RAND_MAX)*(pProbaE->cumulProba[pProbaE->size-1]);
			index = getIndexCumulativeProba(pProbaE->cumulProba, pProbaE->size, rand_val);
			
		}while(index==current_vertex && index!=-1);
		indexO = index;
		
		//fprintf(stderr, "\nindexO=%u\n", indexO);
		if(indexO!=-1)
		{
			pProbaE->proba[indexO]--;
			if(pProbaE->proba[indexO]<0)
				pProbaE->proba[indexO] = 0;
			randCumulProbaUpdate(q->idRandVertexE, indexO);
			
			pProbaO->proba[current_vertex]--;
			if(pProbaO->proba[current_vertex]<0)
				pProbaO->proba[current_vertex] = 0;
			randCumulProbaUpdate(q->idRandVertexO, current_vertex);
		}
	}
	
	//return results
	int * output = (int *)malloc(sizeof(int)*2);
	assert(output);
	
	output[0] = indexE;
	output[1] = indexO;
	
	return output;
}





------------------------------------------------------------------------------------

unsigned int * randomFixedSum(unsigned int n, int sum)
{
	unsigned int * output = (unsigned int *)malloc(sizeof(unsigned int)*n);
	assert(output);
	initializeIntArray((int *)output, n, 1);//set to one all the value
	
	int r;
	
	sum-=n;
	while(sum>=0)
	{
		r = getRandomInSegment(0, n-1);
		output[r]++;
		sum--;
	}
	
	return output;
}



unsigned int * randomFixedSum(unsigned int n, int sum)
{
	unsigned int * output = (unsigned int *)malloc(sizeof(unsigned int)*n);
	assert(output);
	
	unsigned int i;
	int count =0;
	
	for(i=0; i<n; i++)
	{
		output[i]=1 + rand()%n;
		count+=output[i];
	}
	
	float factor = ((float)sum)/((float)count);
	
	for(i=0; i<n; i++)
	{
		output[i]= ceil(output[i]*factor);
	}
	
	return output;
}
