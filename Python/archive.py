#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Mar 15 13:20:17 2019

@author: benjamin
"""


import ctypes
import os

PARENT_FOLDER = os.path.dirname(os.getcwd())#parent folder


direc = os.path.join(PARENT_FOLDER, "Algorithmes_C/Librairies/lib.so")
lib = ctypes.CDLL(direc)
#lib.py_createStronglyConnectedGraph(ctypes.c_uint(n), ctypes.c_uint(D), filename.encode('utf-8'))

class Array(ctypes.Structure):
    pass
Array._fields_ = [("data", ctypes.c_int), ("next", ctypes.POINTER(Array))]

filename="test.txt"

lib.py_createTree.restype = ctypes.POINTER(Array)

list_a = ctypes.POINTER(Array)*10
list_a = lib.py_createTree(ctypes.c_uint(10), ctypes.c_uint(5), filename.encode('utf-8'))

p = Array()
p = list_a[0]
#print(int(p.data))


#%%
class Array(ctypes.Structure):
    pass
Array._fields_ = [("data", ctypes.c_int), ("next", ctypes.POINTER(Array))]

def toCharArray(string):
    return string.encode('utf-8')

class StringArray(ctypes.Structure):
    _fileds_=[("data", ctypes.c_char_p),("size", ctypes.c_uint)]
    
#%%
class Person:
  def __init__(self, name, age):
    self.name = name
    self.age = age
  def test(self):
      print(self.age)

p1 = Person("John", 36)

p1.test()

#%%

import snakes.plugins
snakes.plugins.load("gv", "snakes.nets", "nets")
import nets
import numpy
import os
import networkx as nx
import matplotlib.pyplot as plt


pn=nets.PetriNet("Petri")
pn.add_place(nets.Place("p0",["test"]))
pn.add_place(nets.Place("p1"))
pn.add_place(nets.Place("p2"))
pn.add_transition(nets.Transition("t0"))
pn.add_input("p0","t0", nets.Variable("Poids1"))
pn.add_input("p1","t0", nets.Variable("b"))
pn.add_output("p2","t0",nets.Expression("Poids_1"))
pn.draw("test.png")

def createPetriNet(graph):
    ''' graph = list of lists of successors of each node
        For example, graph[0]=[successors of the node 0]
        Then return a petri net where place are arc and transition are node
    '''
    pn = nets.PetriNet("Petri")
    places = graph[0]
    trans=graph[1]
    
    #add all the places
    for i in range(len(places)):
        pn.add_place(nets.Place(str(i),["p"+str(places[i])]))
        
    #add all the transitions
    for i in range(len(trans)):
        pn.add_transition(nets.Transition("t"+str(i)))
        
    #add links between transition and places
    for i in range(len(trans)):
        for j in range(len(trans[i])):
            val = trans[i][j]
            if(val>0):
               pn.add_output("p"+str(j),"t"+str(i),nets.Expression(str(val))) 
            elif(val<0):
                pn.add_input("p"+str(j),"t"+str(i), nets.Variable(str(val)))
                
    return pn
    


G = nx.erdos_renyi_graph(20,0.1)
color_map = []
for node in G:
if node <10:
    color_map.append('blue')
else: color_map.append('green')      
nx.draw(G,node_color = color_map,with_labels = True)
plt.show()



G = nx.DiGraph()   # or DiGraph, MultiGraph, MultiDiGraph, etc
G.add_node(1)
G.add_node('Hello')
G.add_node('3')
e=(1, 'Hello')
G.add_edge(*e) 
for node in G:
    print(node)
pos = nx.kamada_kawai_layout(G)
nx.draw_networkx_nodes(G, pos, node_size = 60)
nx.draw_networkx_labels(G, pos)
nx.draw_networkx_edges(G, pos, width= 1.5, arrows=True)
label = {(1,'Hello'):'AB'}
nx.draw_networkx_edge_labels(G,pos,edge_labels=label,font_color='red')

#%%