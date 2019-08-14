#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Mar 23 17:31:35 2019

@author: benjamin
"""
import CFunctions as c
import GraphDisplay as gd

''' need to compile C source (main_py) '''

#%%

numberNode = 5
numberInputNode = 2
NumberOutputNode = 2

net = c.freeChoice(n=numberNode, Ki=numberInputNode, Ko=NumberOutputNode, rep_vect_norm=10, cleanExtraMemSpace=False)
f_net = gd.drawPetriNetwork(net.obj, nodeSize=500, widthArraw=1.5)
f_net.show()

c.free(net)
c.exit()#free all the non-free object


'''
numberNode = 10
numberChildNode = 4
graph1 = c.buildTree(numberNode,numberChildNode)
graph1 = c.stronglyConnectedGraph(graph1, isTree=True)
Xgraph1 = gd.toNetworkxGraph(graph1.obj)

f = gd.plotGraph(Xgraph1, nodeSize=200, widthArraw=1.5)
f.show()

c.free(graph1)
'''

'''
numberNode = 5
numberInputNode = 2
NumberOutputNode = 2
graph2 = c.randomGraph(numberNode,numberInputNode,NumberOutputNode)
graph2 = c.stronglyConnectedGraph(graph2, isTree=False)
Xgraph2 = gd.toNetworkxGraph(graph2.obj)

f1 = gd.plotGraph(Xgraph2, nodeSize=200, widthArraw=1.5)
f1.show()


graph3 = c.petriTransformation(graph2)
f2 = gd.drawPetriNetwork(graph2.obj, 500, 1.5)
f2.show()

c.free(graph3)
c.free(graph2)
'''


'''
graph = "4|3/3|0/4|0/2|1/2|1/"
f = gd.drawGraphFromString(graph, nodeSize=200, widthArraw=1.5)
f.show()

petri = "(p0,10)/(p1,7)/(p2,0)/(p3,6)/(p4,0);t0/t1/t2/t3/t4;(t2,p1,3)/(t0,p3,6)/(t0,p0,6)/(t2,p4,3)/(t1,p2,2)/(p4,t1,4)/(t4,p4,3)/(p3,t3,4)/(t4,p3,3)/(p2,t0,12)/(t3,p2,2)/(p1,t4,6)/(t3,p1,2)/(p0,t2,6)/(t1,p0,2)"
fp = gd.drawPetriFromString(petri, nodeSize=500, widthArraw=1.5)
fp.show()
'''