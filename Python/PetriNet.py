#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Mar 16 15:48:33 2019

@author: benjamin
"""

import snakes.plugins
snakes.plugins.load("gv", "snakes.nets", "nets")
from nets import *

pn=PetriNet("Petri")
pn.add_place(Place("p0"))
pn.add_place(Place("p1"))
pn.add_place(Place("p2"))
pn.add_transition(Transition("t0"))
pn.add_input("p0","t0", Variable("a"))
pn.add_input("p1","t0", Variable("b"))
pn.add_output("p2","t0",Expression("c"))
pn.draw("test.png")

def createPetriNet(graph):
    ''' graph = list of lists of successors of each node
        For example, graph[0]=[successors of the node 0]
        Then return a petri net where place are arc and transition are node
    '''
    
    