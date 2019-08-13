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
'''  
Run main_py at port x with a buffersize of l
srv = sc.Server(x, l)
and comment the line "self.openCServerSide()" in the Server function

Then it allow you to start the main_py from a terminal get all the message error and run valgrind
while manupiling functions from python to test them
'''

def showErrors():
    print(srv.getTerminalErros())

def getnewId():
    global _id
    output = _id;
    _id = _id+1
    return output

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
    id = getnewId()
    request = sc.createRequest(task = sc.Task.t_buildTree, n=n, D=D, wrapperId = id)
    response =  srv.getResponse(request)
    
    return Graph(id, gd.parseGraph(response))

#%%
def stronglyConnectedGraph(graph, isTree=False):
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
def randomGraph(n, Ki, Ko):
    id = getnewId()
    request = sc.createRequest(task = sc.Task.t_randomGraph, n=n, wrapperId = id, Ki=Ki, Ko=Ko)
    response =  srv.getResponse(request)
    return Graph(id, gd.parseGraph(response))
#%%
def free(graph):
    request = sc.createRequest(task = sc.Task.t_free,  wrapperId = graph.id)
    srv.getResponse(request)

#%%
def petriTransformation(graph):
    id = getnewId()
    request = sc.createRequest(task = sc.Task.t_petri, 
                               wrapperId = graph.id, newWrapperId = id)
    response =  srv.getResponse(request)

    return Graph(id, gd.parsePetriNetwork(response))

#%%
def freeChoice(n, Ki, Ko, rep_vect_norm, cleanExtraMemSpace=False):
    memClean=None
    if(cleanExtraMemSpace==True):
        memClean = 1
    else:
        memClean = 0
    
    id = getnewId()
    request = sc.createRequest(task = sc.Task.t_freeChoice, n=n, wrapperId = id, Ki=Ki, Ko=Ko, rep_vect_norm=rep_vect_norm, cleanExtraMemSpace=memClean)
    response =  srv.getResponse(request)
    return Graph(id, gd.parsePetriNetwork(response))

#%%
def exit():
    srv.close()
#%%
