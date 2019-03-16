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

import SocketCommunication as sc

#%%
ARRAY_CHAR_SEPARATOR = '-'
LIST_CHAR_SEPARATOR = '|'


#%%,

def ParseGraph(string_graph):
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
    ''' convert an standar array into the correct format for networkx graph'''
    Xgraph=[]
    
    for i in range(len(graph)):
        for j in range(len(graph[i])):
            Xgraph.append((i, graph[i][j]))
    
    return Xgraph


def plotGraph(Xgraph, nodeSize, widthArraw):
    '''plot a netwokx graph onto a new figure'''
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
def createStringlyConnectedGraph(n, D, nodeSize, widthArraw):
    ''' Python asks the C server to create the corresponding graph
        Then the response is retrieve as string
        The string is parse into an array
        The array is converted into the correct format for networkx graph
        The networkx graph is finally plot'''
    request = sc.createRequest(task = sc.Task.f_createStronglyConnectedGraph, n=n, D=D)
    response =  srv.getResponse(request)
    graph = ParseGraph(response)
    Xgraph = toNetworkxGraph(graph)
    
    f = plotGraph(Xgraph, nodeSize=nodeSize, widthArraw=widthArraw)
    
    return f#return the plot


#%%
#plt.figure(1,figsize=(8,8)) 

srv = sc.Server()#create socket communication with C server (to fully use the C functions)

f = createStringlyConnectedGraph(n=10,D=3, nodeSize=200, widthArraw=1.5)
f.show()

srv.close()
