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
ARRAY_CHAR_SEPARATOR = '-'
LIST_CHAR_SEPARATOR = '|'


#%%,

def ParseGraph(string_graph):
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
        if(c==ARRAY_CHAR_SEPARATOR):
            if(len(s_number)!=0):
                array.append(int(s_number))
            s_number = ""
        elif(c==LIST_CHAR_SEPARATOR or c=='\n'):
            if(len(s_number)!=0):
                array.append(int(s_number))
            graph.append(array)
            s_number = ""
            array = []
        else :
            s_number = s_number + c
    
    return graph


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

