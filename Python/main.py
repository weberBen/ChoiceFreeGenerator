#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Mar 23 17:31:35 2019

@author: benjamin
"""
import CFunctions as c
import GraphDisplay as gd
import Statistic as st
import matplotlib.pyplot as plt


#%%
#create graph 1 (as tree)
'''
graph1 = c.buildTree(10,4)
Xgraph1 = gd.toNetworkxGraph(graph1.obj)

f = gd.plotGraph(Xgraph1, nodeSize=200, widthArraw=1.5)
f.show()'''


#create graph 1 (as random graph)
#graph2 = c.randomGraph(n=10,Ki=4,Ko=2)
#graph2 = c.stronglyConnectedGraph(graph2, False)
'''Xgraph2 = gd.toNetworkxGraph(graph2.obj)
f = gd.plotGraph(Xgraph2, nodeSize=200, widthArraw=1.5)
f.show()'''

'''
graph2 = c.randomGraph(n=3,Ki=2,Ko=2)
graph2 = c.stronglyConnectedGraph(graph2, False)
Xgraph2 = gd.toNetworkxGraph(graph2.obj)
f1 = gd.plotGraph(Xgraph2, nodeSize=200, widthArraw=1.5)
f1.show()

graph2 = c.petriTransformation(graph2)
f2 = gd.drawPetriNetwork(graph2.obj, 500, 1.5)
f2.show()

c.free(graph2)'''
net = c.freeChoice(n=10, Ki=4, Ko=2, rep_vect_norm=50)
f_net = gd.drawPetriNetwork(net.obj, 500, 1.5)
f_net.show()

c.exit()

'''
graph = "4|3/3|0/4|0/2|1/2|1/"
f = gd.drawGraphFromString(graph, nodeSize=200, widthArraw=1.5)
f.show()

petri = "(p0,3)/(p1,1)/(p2,1)/(p3,2)/(p4,3)/(p5,0)/(p6,2)/(p7,0)/(p8,0)/(p9,1);t0/t1/t2/t3/t4;(p9,t1,1)/(t4,p9,3)/(p8,t2,3)/(t4,p8,3)/(p7,t1,1)/(t3,p7,1)/(p6,t2,3)/(t3,p6,1)/(p5,t0,3)/(t2,p5,3)/(p4,t4,3)/(t2,p4,3)/(p3,t0,3)/(t1,p3,1)/(p2,t3,1)/(t1,p2,1)/(p1,t3,1)/(t0,p1,3)/(p0,t4,3)/(t0,p0,3)"
fp = gd.drawPetriFromString(petri, nodeSize=500, widthArraw=1.5)
fp.show()'''

'''
petri = c.petriTransformation(graph2)
Xpetri = gd.toPetriNetwork(petri.obj)
f = gd.drawPetriNetwork(Xpetri, 500, 1.5)
f.show()

c.free(graph2)
c.free(petri)
'''
'''
#convert graph 1 into a strongly connected graph
graph1 = c.stronglyConnectedGraph(graph1, True) #True to specify that the graph is a tree
#display
Xgraph = gd.toNetworkxGraph(graph1.obj)
f = gd.plotGraph(Xgraph, nodeSize=200, widthArraw=1.5)
f.show()
'''

'''
#convert graph 2 into a strongly connected graph
graph2 = c.stronglyConnectedGraph(graph2, isTree=False)

#create petri network from an existing strongly connected graph
petri = c.petriTransformation(graph2)
#show
Xpetri = gd.toPetriNetwork(petri.obj)
f = gd.drawPetriNetwork(Xpetri, 500, 1.5)
f.show()


#free memory
c.free(graph1)
c.free(graph2)
c.free(petri)

#exit
c.exit()'''

#%%
'''
string = '0|1|2|3|4|5|/;1|-1|-1|0|0|0|/0|-1|1|0|0|0|/0|0|-1|0|0|1|/0|1|0|1|-1|-1|/'
p=gd.parsePetriNetwork(string)
pn=gd.toPetriNetwork(p)
f = gd.drawPetriNetwork(pn, 700, 2)
f.show()'''
