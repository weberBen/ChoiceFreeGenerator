#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Mar 15 20:43:54 2019

@author: benjamin
"""

"""Premier exemple avec Tkinter.

On crée une fenêtre simple qui souhaite la bienvenue à l'utilisateur.

"""


from snakes.nets import *
n = PetriNet('First net')
n.add_place(Place('p', [0]))
n.add_transition(Transition('t', Expression('x<5')))
n.add_input('p', 't', Variable('x'))
n.add_output('p', 't', Expression('x+1'))




import snakes.plugins
snakes.plugins.load("gv", "snakes.nets", "nets")
from nets import *

pn=PetriNet("hello world in SNAKES")
pn.add_place(Place("hello", ["hello","salut"]))
pn.add_place(Place("world", ["world","le monde"]))
pn.add_place(Place("sentence"))
pn.add_transition(Transition("concat"))
pn.add_input("hello","concat",Variable("h"))
pn.add_input("world","concat",Variable("w"))
pn.add_output("sentence","concat",Expression("h + ' ' + w"))
pn.draw("hello1.png")
modes=pn.transition("concat").modes()
# modes = [Substitution(h=’salut’, w=’world’),
#    Substitution(h=’salut’, w=’le monde’),
#    Substitution(h=’hello’, w=’world’),
#    Substitution(h=’hello’, w=’le monde’)]
pn.transition("concat").fire(modes[2])
pn.draw("hello2.png")



#%%

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
