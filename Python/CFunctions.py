#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Mar 23 17:09:33 2019

@author: benjamin
"""
import GraphDisplay as gd
import SocketCommunication as sc

#%%
_id = 0
srv = sc.Server()

#%%
class Graph:
    def __init__(self,id, obj):
        self.id = id
        self.obj = obj  
#%%

def buildTree(n, D):
    ''' Python asks the C server to create the corresponding graph
        Then the response is retrieve as string
        The string is parse into an array
        The array is converted into the correct format for networkx graph
        The networkx graph is finally plot'''
    global _id
    id = _id
    request = sc.createRequest(task = sc.Task.t_buildTree, n=n, D=D, wrapperId = id)
    response =  srv.getResponse(request)
    _id = _id +1
    
    return Graph(id, gd.parseGraph(response))

#%%
def stronglyConnectedGraph(graph, isTree):
    tree=None
    if(isTree==True):
        tree = 1
    else:
        tree = 0
    request = sc.createRequest(task = sc.Task.t_stronglyConnectedGraph, 
                               wrapperId = graph.id, isTree=tree)
    response =  srv.getResponse(request)

    graph.obj = gd.parseGraph(response)
    return graph

#%%
def randomGraph(n, D):
    global _id
    id = _id
    request = sc.createRequest(task = sc.Task.t_randomGraph, n=n, D=D, wrapperId = id)
    response =  srv.getResponse(request)
    _id = _id +1
    
    return Graph(id, gd.parseGraph(response))
#%%
def free(graph):
    request = sc.createRequest(task = sc.Task.t_free,  wrapperId = graph.id)
    srv.getResponse(request)
    
#%%

def createStringlyConnectedGraph(n, D, nodeSize, widthArraw):
    ''' Python asks the C server to create the corresponding graph
        Then the response is retrieve as string
        The string is parse into an array
        The array is converted into the correct format for networkx graph
        The networkx graph is finally plot'''
    request = sc.createRequest(task = sc.Task.f_createStronglyConnectedGraph, n=n, D=D)
    response =  srv.getResponse(request)
    print(response)
    graph = gd.parseGraph(response)
    print(graph)
    Xgraph = gd.toNetworkxGraph(graph)
    
    f = gd.plotGraph(Xgraph, nodeSize=nodeSize, widthArraw=widthArraw)
    
    return f,graph, Xgraph#return the plot

#%%
def exit():
    srv.close()
#%%
