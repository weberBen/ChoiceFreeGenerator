# Creation of random free choice graph generator


# Sommaire
- [Description](#description)
- [Interface](#Interface)
  - [Requires](#Requires)
  - [Commands](#Commands)

# Description <a name="Description"/>

A free choice graph is a subclass of petri net where each place has exactly one output and transitions have one input and multiple outputs as shown in the following figure

<img src="Annexes/Images/FreeChoiceDef.png" width="70%"  align="middle">

# Main script <a name="MainScript"/>

## Requires <a name="Requires"/>
    - libglpk-dev (version 4.61-1)
    - gcc (version 4:6.3.0-4)
    
## Description <a name="Requires"/>

*NAME*

freeChoiceGenerator - a random generator of living Free-choice

SYNOPSIS

```freeChoiceGenerator [nb_transition] [nb_input_node] [vect_norm] -OPTIONS```

*DESCRIPTION*

freeChoiceGenerator allows user to generate random living Free-choice network given the desired number of transitions ```[nb_transition]``` in the final petri network. 

The generation of the final network is based uppon the one of a strongly connected graph where the number of input ```[nb_input_node]``` and output ```[nb_output_node]``` per node is set by the user (with the following constraint ```[nb_input_node]>=[nb_output_node]```). By default ```[nb_input_node]=[nb_output_node]```.

The ```[vect_norm]``` is the desired norm of the repetition vector to get during the generation of a random one for the SDF

*OPTIONS*

**-o** ```[nb_output_node]```  : set ```[nb_output_node]```

**-c**  : resize the petri net after the transformation from SDF to Free-choice (can be time consuming)

**-f**  : ```[filename]```   save the Free-choice to a file to ```[filename]``` as PNML format

**-s**  : ```[stack_size]```   set the new stack size to use

**-h**  : get help

## Example
```freeChoiceGenerator 10 5 20 -f output.pnml``` produce a ramdom Free-choice with 10 transitions in which the norm of the repetiton vector is equal to 20 and then write the net into the file *output.pnml*

# API <a name="Api"/>
  
  ## Create a random Free-choice based on repetition vector norm
  
``` C 
#include "GraphGenerator.h"
#include "Display.h"
```

``` C
srand(time(NULL));

unsigned int nb_transition = 10;//number of desired transition in the result Free-choice
unsigned int nb_input_node = 3;//average (and maximum) number of inputs for each transition
unsigned int nb_output_node = 3;//average (and maximum) number of outputs for each transition
unsigned int vect_norm = 10;//norm of the repetition vector to generate
int cleanExtraMem = 0;//the transformation from SDF to Free-choice leaves extra empty memory space, the cleaning process is optional because it's time consuming

//create random Free-choice from its repetition vector norm
pPetri net1 = generateRandomFreeChoice(nb_transition, nb_input_node, nb_output_node vect_norm, cleanExtraMem);
//write Free-choice named "net1" to file "net1.pnml"
petriToPnmlFile(net1, "net1", "net1.pnml");
//displat Free-choice to the shell
petriToPnmlDisplay(net1, "net1");
petriFree(net1);
```
  ## Create a random Free-choice based on repetition vector

```C
//create random Free-choice from a given repetition vector
unsigned int * vect = weightsComputation(nb_transition, vect_norm);
pPetri net2 = generateFreeChoiceWithVector(nb_transition, nb_input_node, nb_output_node, vect, cleanExtraMem);
//write Free-choice named "net2" to file "net2.pnml"
petriToPnmlFile(net2, "net2", "net2.pnml");
free(vect);
petriFree(net2);
```

  ## Petri net strcuture
 A simple tutorial to understand how the structure used to save Free-choice works
 
  ## Creation of a Petri net
 ```C
unsigned int nb_place = 5;
unsigned int nb_transitions = 5;
int i;

//initialize structure
pPetri net3 =  petriCreate(nb_place, nb_transitions);

//add places
petriAddPlace(net3, 0, 1);//add place 0 with an initial marking of 1
petriAddPlace(net3, 3, 2);//add place 3 with an initial marking of 2
petriAddPlace(net3, 4, 3);//add place 4 with an initial marking of 3
petriAddPlace(net3, 1, 4);//add place 1 with an initial marking of 4
//the place 2 will not be set inside the petri structure (it value inside the fixed size array will be set to NULL)

//add transitions
for(i=0; i<nb_transitions; i++)
{
	petriAddTransition(net3, i);//add transition i
}

//add links
petriAddlink(net3, PETRI_PT_LINK, 0, 1, 12);//add link between the place 0 and the transition 1 with a weight of 12
petriAddlink(net3, PETRI_PT_LINK, 1, 3, 0);//add link between the place 1 and the transition 3 with a weight of 0

petriAddlink(net3, PETRI_TP_LINK, 4, 3, 4);//add link between the transition 4 and the place 3 with a weight of 4
petriAddlink(net3, PETRI_TP_LINK, 3, 2, 1);//add link between the transition 3 and the place 2 with a weight of 1
petriAddlink(net3, PETRI_TP_LINK, 3, 0, 5);//add link between the transition 3 and the place 0 with a weight of 5


petriAddlink(net3, PETRI_PT_LINK, 2, 1, 12);//not possible because place 2 does not exists
petriAddlink(net3, PETRI_TP_LINK, 3, 2, 12);//not possible because place 2 does not exists

pPetriLink link = petriGetLink(net3, PETRI_PT_LINK, 0, 1);//get link between the place 0 and the transition 1
printf("Weight of link between place 0 and transition 1 : %d\n", link->weight);
printf("Weight of link between transition 4 and place 3 : %d\n", petriGetWeightLink(net3, PETRI_TP_LINK, 4, 3));
```
### Getter functions

```C
pPetriElem elem = petriGetPlace(net3, 0);//get place 0
printf("Initial marking of place %u : %d\n", elem->label, elem->val);
printf("Initial marking of place 4 : %d\n", petriGetInitialMarking(net3, 4));

printf("Places : { ");
for(i=0; i<net3->nb_pl; i++)
{
	displayPetriElem(net3->pl_elems[i]);
	printf(" ");
}
printf("}\n");

//get all the link for a node
pPetriNode node = net3->places[0];
pArray p = node->input_links;
printf("Inputs of node 0 : \n");
while(p)
{
	printf("\t");
	link = petriNodeGetLinkFromArrayNode(p);
	elem = link->input;
	displayPetriElem(elem);
	printf("\n");

	p = p->next;
}
printf("Outputs of node 0 : \n");
p = node->output_links;
while(p)
{
	printf("\t");
	link = petriNodeGetLinkFromArrayNode(p);
	elem = link->output;
	displayPetriElem(elem);
	printf("\n");

	p = p->next;
}

```
### Manipulation of the Petri net
```C
//remove place 3
petriRemovePlace(net3, 3);
printf("Places : { ");
for(i=0; i<net3->nb_pl; i++)
{
	displayPetriElem(net3->pl_elems[i]);
	printf(" ");
}
printf("}\n");

//clear extra memory space
petriClearPlaces(net3);

printf("Places : { ");
for(i=0; i<net3->nb_pl; i++)
{
	displayPetriElem(net3->pl_elems[i]);
	printf(" ");
}
printf("}\n");

//free memory
petriFree(net3);
```
# Interface <a name="Interface"/>

All the functions about generating and modifying graph are written in C. But all the display parts are python scripts (because python can use networkx for oriented graph and SNAKES for petri net). Thus, we had to connect C functions with python. So the simplest solution for now is to create a server socket in C that collects python request, process the request and send back the result to python; ctypes was kind of unsatisfying. All the described process can be visualized on the following figure :

<img src="Annexes/Images/Py_interface.png" width="60%"  align="middle">

  ## Requires <a name="Requires"/>

**Version** : Python 3.5

**Used modules** :
  - ctypes
  - networkx
  - [SNAKES](https://snakes.ibisc.univ-evry.fr/)
  - graphviz (pay attention that you probably will need to install it for python and for the OS)

  ## Commands to use python interface <a name="Commands"/>
The module *SocketCommunication* contains all the needed classes and functions to communicated with the server side. We can underline :

  - *Task* (class) that store all the values of request. Each request starts with a specific task that help the server to know which result needed to be sent back
  - *Request* (ctypes structure) which is the formatted request to send
    - *createRequest* (function) create a new request while formatting the arguments into the correct ctypes
  For example, to create a new request use the command ```request = createRequest(task = Task.f_createStronglyConnectedGraph, n=n, D=D)``` with n, D specific arguments
  - *Server* (class) the main class that starts the server at the begining and then sends the needed request and return the response as string
  For example, to start the server side use ```src=Server(port=5112, buffersize=512)``` or ```src=Server()``` to fill automatically the arguments. Port is the communication port (which can be found automatically if not specified) and buffersize is the size of the buffer used between the clent and the server to transmit data.
  Be aware of the closing of the server : at the end of the script the server need to be closed. For that purpose,use the command *srv.close()*
 
The module *GraphDisplay* takes the formatted response of the server and convert it into a usable graph. The module contains :
  
  - *ParseGraph* (function) that takes the formatted string of the server response and converts it into an iterable object  The graph is sent back as an array of array of child for each node. For example, if the array is graphed, the graph[0] = [...] contains all the successors of the node 0
  - *toNetworkxGraph* (function) converts the graph (following the previous format) into an object usable by the library networkx
  - *plotGraph* (function) plot into a figure a networkx graph
  - *createStronglyConnectedGraph* (function) takes two main arguments *n* which is the desired number of nodes and *D* which is the maximum number of successors allowed for a node. The other arguments are for the graphical display.
  The function return a n-uplet (figure, graph, networkx graph) where figure is the figure where the networkx graph was plot
  
An example of the use of the functions :
``` 
import SocketCommunication as sc

srv = sc.Server()

res = createStringlyConnectedGraph(n=7,D=3, nodeSize=200, widthArraw=1.5)
f=res[0]
graph = res[1]
networkx_graph = res[2]
f.show()

srv.close()
```

 ### About the tasks value
...later
