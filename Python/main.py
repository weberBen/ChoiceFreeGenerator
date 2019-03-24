#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Mar 23 17:31:35 2019

@author: benjamin
"""
import CFunctions as c
import GraphDisplay as gd

#%%
#create graph 1 (as tree)
graph1 = c.buildTree(10,5)
Xgraph1 = gd.toNetworkxGraph(graph1.obj)

f = gd.plotGraph(Xgraph1, nodeSize=200, widthArraw=1.5)
f.show()

#create graph 1 (as random graph)
graph2 = c.randomGraph(20,7)
Xgraph2 = gd.toNetworkxGraph(graph2.obj)

f = gd.plotGraph(Xgraph2, nodeSize=200, widthArraw=1.5)
f.show()

#convert graph 1 into a strongly connected graph
graph1 = c.stronglyConnectedGraph(graph1, True) #True to specify that the graph is a tree
#convert graph 2 into a strongly connected graph
graph2 = c.stronglyConnectedGraph(graph2, False)

#free memory
c.free(graph1)
c.free(graph2)

#exit
c.exit()

#%%

string = '0|1|2|3|4|5|/;1|-1|-1|0|0|0|/0|-1|1|0|0|0|/0|0|-1|0|0|1|/0|1|0|1|-1|-1|/'
p=gd.parsePetriNetwork(string)
pn=gd.toPetriNetwork(p)
f = gd.drawPetriNetwork(pn, 700, 2)
f.show()
