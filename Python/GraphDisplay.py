#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Thu Mar 14 13:53:17 2019

@author: benjamin
"""

import numpy
import os
import networkx as nx
import matplotlib.pyplot as plt
#%%
CHAR_ARRAY_SEPARATOR = '|'
CHAR_LIST_SEPARATOR = '/'
CHAR_DATA_SEPARATOR =';'
CHAR_INNER_DATA_SEPARATOR = ','

#%%,        
def parseGraph(string_graph):
    ''' Input = a formatted string that represente a graph (a list of list of successors)
            the format is for an element i of the list is : 
                successors_1_of_node_i successors_2_of_node_i...
                ARRAY_CHAR_SEPARATOR successors_p_of_node_i LIST_CHAR_SEPARATOR
            For example if the string (with LIST_CHAR_SEPARATOR='|' and ARRAY_CHAR_SEPARATOR='-')
            '1-2|3-4|5|||6||' then the list is [[1,2], [3,4], [5], [], [], [6], []]
    
        return a list of lists of successors of each node
        For example, output[0]=[successors of the node 0]'''
    graph = []
    array = []
    s_number = ""
    
    for c in string_graph:
        if(c==CHAR_ARRAY_SEPARATOR):
            if(len(s_number)!=0):
                array.append(int(s_number))
            s_number = ""
        elif(c==CHAR_LIST_SEPARATOR):
            if(len(s_number)!=0):
                array.append(int(s_number))
            graph.append(array)
            s_number = ""
            array = []
        else :
            s_number = s_number + c
    
    return graph

def parsePetriNetwork(string_graph):
    '''
        For a petri net with : 
	 		n places, marked as p with the index of the place, where M0(pi) is the initial marking of the place i
			m transitions, markes as t with the index of the transition
		Then the format of the output is (with no line break : '\n' and with no space ' ') : 

	 	 (p1, M0(pi) CHAR_LIST_SEPARATOR (p2, M0(p2)) ... (pn, M0(pn)) 
		 CHAR_DATA_SEPARATOR
		 t1 CHAR_LIST_SEPARATOR t2 CHAR_LIST_SEPARATOR ... tm
		 CHAR_DATA_SEPARATOR
		 (input_node, output_node, weigth) CHAR_LIST_SEPARATOR ... where input_node is the label of the node from where
		 	the link start and output_node the label of the node where it stop (with a weight)
		 \0

		Example (with CHAR_LIST_SEPARATOR='\' and CHAR_DATA_SEPARATOR=';')
		(p0,2)/(p1,24)/(p2,22)/(p3,23)/(p4,21);t0/t1/t2/t3/t4;(t4, p4, 63)/(t3, p3, 36)/(t2, p2, 67)/(t1, p1, 26)/(t0, p0, 62)/(p3, t4, 12)/(p2, t4, 54)/(p1, t4, 52)/(p0, t4, 75)

	'''
        
    s_number = ""
    tmp = []
    places = []
    transitions = []
    pn = nx.DiGraph()
    labelNodes = {}
    labelEdges = {}
    i = 0
    
    if(len(s_number)==0):
        [pn, labelNodes, labelEdges]
    
    #get places and intial marking
    for c in string_graph:
        if(c==CHAR_DATA_SEPARATOR):
            break
        elif(c=='('):
            i = i + 1
            continue
        elif(c==CHAR_INNER_DATA_SEPARATOR):
            tmp.append(s_number)#add place
            s_number = ""
            i = i + 1
            continue
        elif(c==')'):
            tmp.append(s_number)#add place
            s_number = ""
            i = i + 1
            continue
        elif (c==CHAR_LIST_SEPARATOR):
            places.append(tmp[0])
            labelNodes[tmp[0]] = str(tmp[0])+"\n"+str(tmp[1])
            s_number = ""
            tmp = []
            i = i + 1
            continue
        
        s_number = s_number + c
        i = i + 1
    
    string_graph = string_graph[i+1:]
    
    #get last place
    if(len(tmp)!=0):
        places.append(tmp[0])
        labelNodes[tmp[0]] = str(tmp[0])+"\n"+str(tmp[1])
    s_number = ""
    tmp = []
    
    #get transitions
    i = 0
    for c in string_graph:
        if(c==CHAR_DATA_SEPARATOR):
            break
        elif (c==CHAR_LIST_SEPARATOR):
            transitions.append(s_number)
            labelNodes[s_number] = s_number
            s_number = ""
            i = i + 1
            continue
        
        s_number = s_number + c
        i = i + 1
    
    string_graph = string_graph[i+1:]
    #get last element
    if(len(s_number)!=0):
        transitions.append(s_number)
        labelNodes[s_number] = s_number
    s_number = ""
    tmp = []
    
    #get links
    i = 0
    step  = 0
    for c in string_graph:
        if(c=='('):
            i = i + 1 
            continue
        elif(c==CHAR_INNER_DATA_SEPARATOR):
            if(step==0):
                tmp.append(s_number)
            else :
                tmp.append(s_number)
            step = step + 1
            s_number = ""
            i = i + 1
            continue
        elif(c==')'):
            tmp.append(s_number)
            s_number = ""
            i = i + 1
            continue
        elif (c==CHAR_LIST_SEPARATOR):
            pn.add_edge(tmp[0], tmp[1])
            labelEdges[(tmp[0], tmp[1])] = tmp[2]
            tmp = []
            s_number = ""
            i = i + 1
            continue
        
        s_number = s_number + c
        i = i + 1
    
    if(len(tmp)!=0):
        pn.add_edge(tmp[0], tmp[1])
        labelEdges[(tmp[0], tmp[1])] = tmp[2]
    
            
    return [pn, places, transitions, labelNodes, labelEdges]
#%%

def toNetworkxGraph(graph) :
    ''' convert an standar array into the correct format for networkx graph
        graph = list of lists of successors of each node
            For example, graph[0]=[successors of the node 0]
        output = list of each relation between node (if one exists)
            output = [(i,j)] where j is a child of i
    '''
    Xgraph=[]
    
    for i in range(len(graph)):
        for j in range(len(graph[i])):
            Xgraph.append((i, graph[i][j]))
    
    return Xgraph


def plotGraph(Xgraph, nodeSize, widthArraw):
    '''plot a netwokx graph onto a new figure
       Xgraph = list of each relation between node (if one exists)
            output = [(i,j)] where j is a child of i
    '''
    G = nx.DiGraph()
    G.add_edges_from(Xgraph)
    # Need to create a layout when doing
    # separate calls to draw nodes and edges
    pos = nx.kamada_kawai_layout(G)
    
    plt.ioff()#desactive interactive mode (then figure won't pop up directly after plt.figure())
    fig = plt.figure()
    
    nx.draw_networkx_nodes(G, pos, node_size = nodeSize)
    nx.draw_networkx_labels(G, pos)
    nx.draw_networkx_edges(G, pos, width= widthArraw, arrows=True)
    
    plt.ion()#active interactive mode
    return fig

#%%

#%%

def drawPetriNetwork(petri, nodeSize, widthArraw):
    [G, pl_nodes, tr_nodes, labelNodes, labelEdges] = petri
    
    plt.ioff()#desactive interactive mode (then figure won't pop up directly after plt.figure())
    fig = plt.figure()
    
    
    pos = nx.kamada_kawai_layout(G)

    nx.draw_networkx_nodes(G, pos, nodelist=pl_nodes, node_color='red', node_shape='o', node_size=nodeSize, alpha=0.5)
    nx.draw_networkx_nodes(G, pos, nodelist=tr_nodes, node_color='grey', node_shape='^', node_size=nodeSize, alpha=0.5)

    nx.draw_networkx_labels(G, pos, labels=labelNodes, font_size=10)
    nx.draw_networkx_edges(G, pos, width= widthArraw, arrows=True)
    nx.draw_networkx_edge_labels(G,pos,edge_labels=labelEdges,font_color='red')
    
    plt.ion()#active interactive mode
    return fig
