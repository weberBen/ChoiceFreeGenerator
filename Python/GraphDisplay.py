#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Thu Mar 14 13:53:17 2019

@author: benjamin
"""

import ctypes
import numpy
import os

ARRAY_CHAR_SEPARATOR = '-'
LIST_CHAR_SEPARATOR = '|'
PARENT_FOLDER = os.path.dirname(os.getcwd())#parent folder
DEFAULT_DIR_GRAPHS = os.path.join(PARENT_FOLDER,"Graphes")


#%%,
def launchC(n, D, filename):
    #direc = os.path.join(PARENT_FOLDER, "Algorithmes_C/Librairies/lib.so")
    #lib = ctypes.CDLL(direc)
    #lib.py_createStronglyConnectedGraph(ctypes.c_uint(n), ctypes.c_uint(D), filename.encode('utf-8'))
    
    #use external gcc compilator because the one implement in pyhton makes something wrong
    direc = os.path.join(PARENT_FOLDER, "Algorithmes_C")
    function = "py_createStronglyConnectedGraph"
    args = str(n)+" "+str(D)+" " +filename
    string = os.path.join(direc, "main")+" -f "+ function +" "+args
    os.system(string)
    #./main -f py_createTree 10 5 "testo.txt"

#%%,

def readTree(filename):
    out = []
    array = []
    s_number = ""
    
    
    with open(filename) as f:
      while True:
        c = f.read(1)
        if not c:
          break
        
        if(c==ARRAY_CHAR_SEPARATOR):
            if(len(s_number)!=0):
                array.append(int(s_number))
            s_number = ""
        elif(c==LIST_CHAR_SEPARATOR or c=='\n'):
            if(len(s_number)!=0):
                array.append(int(s_number))
            out.append(array)
            s_number = ""
            array = []
        else :
            s_number = s_number + c
    
    print(out)
    return out
        
def createDirectedGraph(n,D, filename) :
    filename = os.path.join(DEFAULT_DIR_GRAPHS,filename)
    launchC(n,D,filename)#launch C function
    array = readTree(filename)
    out=[]
    
    for i in range(len(array)):
        for j in range(len(array[i])):
            out.append((i, array[i][j]))
    
    return out
    
#%%,
import networkx as nx
import matplotlib.pyplot as plt

#plt.figure(1,figsize=(8,8)) 

G = nx.DiGraph()
G.add_edges_from(createDirectedGraph(20,5,"er.txt"))

# Need to create a layout when doing
# separate calls to draw nodes and edges
pos = nx.kamada_kawai_layout(G)
nx.draw_networkx_nodes(G, pos, node_size = 200)
nx.draw_networkx_labels(G, pos)
nx.draw_networkx_edges(G, pos, width= 1.5, arrows=True)

plt.show()