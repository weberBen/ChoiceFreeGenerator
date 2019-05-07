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
    s_number = ""
    places=[]
    trans=[]
    
    i=0
    
    for c in string_graph:
        if(c==CHAR_DATA_SEPARATOR):
            break
        elif(c==CHAR_ARRAY_SEPARATOR):
            if(len(s_number)!=0):
                places.append(int(s_number))
            s_number=""
        else :
            s_number = s_number + c
        i = i+1
    
    string_graph=string_graph[(i+1):]
    
    trans=parseGraph(string_graph)
            
    return [places, trans]
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
def toPetriNetwork(graph):
    places = graph[0]
    trans=graph[1]
    pn = nx.DiGraph()
    
    
    #add links between transition and places
    labels={}
    for i in range(len(trans)):
        for j in range(len(trans[i])):
            val = trans[i][j]
            pl = "p"+str(j)+"\n"+str(places[j])
            tr = "t"+str(i)
            if(val>0):
                e = (pl, tr)
                labels.update({e:str(val)})
                pn.add_edge(*e)
            elif(val<0):
                e = (tr, pl)
                labels.update({e:str(abs(val))})
                pn.add_edge(*e)
            
                
    return [pn, labels]

def drawPetriNetwork(petri, nodeSize, widthArraw):
    G = petri[0]
    labels=petri[1]
    color_map=[]
    for node in G:
        if(node[0]=='p'):
            color_map.append('red')
        else:
            color_map.append('grey')
    
    pos = nx.kamada_kawai_layout(G)
    
    plt.ioff()#desactive interactive mode (then figure won't pop up directly after plt.figure())
    fig = plt.figure()
    
    nx.draw_networkx_nodes(G, pos, node_size = nodeSize, node_color = color_map, alpha=0.5)
    nx.draw_networkx_labels(G, pos)
    nx.draw_networkx_edges(G, pos, width= widthArraw, arrows=True)
    nx.draw_networkx_edge_labels(G,pos,edge_labels=labels,font_color='red')
    
    plt.ion()#active interactive mode
    return fig
